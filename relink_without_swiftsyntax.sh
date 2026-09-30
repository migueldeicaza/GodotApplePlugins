#!/usr/bin/env bash
# Relink a framework binary without SwiftSyntax objects.
# Xcode incorrectly links SwiftSyntax into runtime payloads when using macro
# packages. This script rebuilds the already-compiled binary using the same
# object files but filters SwiftSyntax objects out of the link file list.

set -euo pipefail

IOS_DEPLOYMENT_TARGET=${IOS_DEPLOYMENT_TARGET:-17.0}
MACOS_DEPLOYMENT_TARGET=${MACOS_DEPLOYMENT_TARGET:-14.0}

usage() {
	cat <<'EOF'
Usage: relink_without_swiftsyntax.sh --derived-data <path> --config <cfg> --framework <name> --platform <ios|macos> --arch <arch>

Environment overrides:
  IOS_DEPLOYMENT_TARGET  (default: 17.0)
  MACOS_DEPLOYMENT_TARGET (default: 14.0)
EOF
}

DERIVED_DATA=""
CONFIG=""
FRAMEWORK=""
PLATFORM=""
ARCH=""

while [[ $# -gt 0 ]]; do
	case "$1" in
		--derived-data)
			DERIVED_DATA="$2"
			shift 2
			;;
		--config)
			CONFIG="$2"
			shift 2
			;;
		--framework)
			FRAMEWORK="$2"
			shift 2
			;;
		--platform)
			PLATFORM="$2"
			shift 2
			;;
		--arch)
			ARCH="$2"
			shift 2
			;;
		-h|--help)
			usage
			exit 0
			;;
		*)
			echo "Unknown argument: $1" >&2
			usage
			exit 1
			;;
	esac
done

if [[ -z "$DERIVED_DATA" || -z "$CONFIG" || -z "$FRAMEWORK" || -z "$PLATFORM" || -z "$ARCH" ]]; then
	echo "Missing required arguments" >&2
	usage
	exit 1
fi

if [[ ! -d "$DERIVED_DATA/Build/Products" ]]; then
	echo "Derived data path looks invalid: $DERIVED_DATA" >&2
	exit 1
fi

platform_lc=$(printf "%s" "$PLATFORM" | tr '[:upper:]' '[:lower:]')
case "$platform_lc" in
	ios)
		SDK="iphoneos"
		CONFIG_SUFFIX="${CONFIG}-iphoneos"
		TARGET_VERSION="$IOS_DEPLOYMENT_TARGET"
		TRIPLE_BASE="ios"
		;;
	macos|macosx)
		SDK="macosx"
		CONFIG_SUFFIX="${CONFIG}"
		TARGET_VERSION="$MACOS_DEPLOYMENT_TARGET"
		TRIPLE_BASE="macosx"
		platform_lc="macos"
		;;
	*)
		echo "Unsupported platform: $PLATFORM" >&2
		exit 1
		;;
esac

PRODUCTS_DIR="$DERIVED_DATA/Build/Products/$CONFIG_SUFFIX"
FRAMEWORK_DIR="$PRODUCTS_DIR/PackageFrameworks/$FRAMEWORK.framework"
if [[ ! -d "$FRAMEWORK_DIR" ]]; then
	FRAMEWORK_DIR="$PRODUCTS_DIR/$FRAMEWORK.framework"
fi
FRAMEWORK_BINARY="$FRAMEWORK_DIR/$FRAMEWORK"
FRAMEWORK_VERSIONED_BINARY=""

if [[ "$platform_lc" = "macos" ]]; then
	for candidate in \
		"$FRAMEWORK_DIR/Versions/Current/$FRAMEWORK" \
		"$FRAMEWORK_DIR/Versions/A/$FRAMEWORK"; do
		if [[ -f "$candidate" ]]; then
			FRAMEWORK_VERSIONED_BINARY="$candidate"
			break
		fi
	done
fi

# On macOS, the framework binary at $FRAMEWORK_DIR/$FRAMEWORK is typically a symlink
# to Versions/Current/$FRAMEWORK. We want to replace the real file, not the symlink,
# otherwise we'd destroy the framework's versioned layout.
FRAMEWORK_BINARY_REAL=""
if [[ -n "$FRAMEWORK_VERSIONED_BINARY" ]]; then
	FRAMEWORK_BINARY_REAL="$FRAMEWORK_VERSIONED_BINARY"
elif [[ -e "$FRAMEWORK_BINARY" ]]; then
	FRAMEWORK_BINARY_REAL=$(python3 - <<'PY' "$FRAMEWORK_BINARY"
import os, sys
print(os.path.realpath(sys.argv[1]))
PY
	)
fi

if [[ -z "$FRAMEWORK_BINARY_REAL" || ! -f "$FRAMEWORK_BINARY_REAL" ]]; then
	if [[ "$platform_lc" = "macos" && -n "$FRAMEWORK_VERSIONED_BINARY" ]]; then
		FRAMEWORK_BINARY_REAL="$FRAMEWORK_VERSIONED_BINARY"
	else
		FRAMEWORK_BINARY_REAL="$FRAMEWORK_BINARY"
	fi
fi

if [[ -z "$FRAMEWORK_BINARY_REAL" || ! -f "$FRAMEWORK_BINARY_REAL" ]]; then
	echo "Framework binary not found at $FRAMEWORK_BINARY (resolved: ${FRAMEWORK_BINARY_REAL:-<none>}), skipping relink." >&2
	exit 0
fi

# Locate the link file list that still references SwiftSyntax objects.
link_file=""
while IFS= read -r -d '' candidate; do
	if grep -q 'SwiftSyntax' "$candidate"; then
		link_file="$candidate"
		break
	fi
done < <(find "$DERIVED_DATA/Build/Intermediates.noindex" \
	-path "*Objects-normal/$ARCH/$FRAMEWORK.LinkFileList" -print0 2>/dev/null || true)

if [[ -z "$link_file" ]]; then
	echo "No SwiftSyntax references found for $FRAMEWORK ($platform_lc/$ARCH); nothing to relink."
	exit 0
fi

filtered_file=$(mktemp "/tmp/${FRAMEWORK}.NoSwiftSyntax.LinkFileList.XXXXXX")
tmp_binary=""
trap 'rm -f "$filtered_file" "${tmp_binary:-}"' EXIT
python3 - <<'PY' "$link_file" "$filtered_file"
import sys
source, dest = sys.argv[1:3]
patterns = (
    "SwiftSyntax",
    "SwiftParser",
    "SwiftDiagnostics",
    "SwiftParserDiagnostics",
    "SwiftBasicFormat",
    "_SwiftSyntaxCShims",
    "ExtensionApi"
)
with open(source) as src, open(dest, "w") as dst:
    for line in src:
        if not any(token in line for token in patterns):
            dst.write(line)
PY

if ! grep -q '[^[:space:]]' "$filtered_file"; then
	echo "Filtered link file list is empty, aborting." >&2
	exit 1
fi

# Locate the directory that contains module artifacts for the framework.
module_dir=""
module_file=$(find "$DERIVED_DATA/Build/Intermediates.noindex" \
	-path "*Objects-normal/$ARCH/$FRAMEWORK.swiftmodule" -print -quit 2>/dev/null || true)
if [[ -n "$module_file" ]]; then
	module_dir=$(dirname "$module_file")
fi

linker_resp=""
if [[ -n "$module_dir" ]]; then
	if [[ -f "$module_dir/$FRAMEWORK-linker-args.resp" ]]; then
		linker_resp="$module_dir/$FRAMEWORK-linker-args.resp"
	fi
fi

lto_file=""
dep_info=""
if [[ -n "$module_dir" ]]; then
	[[ -f "$module_dir/${FRAMEWORK}_lto.o" ]] && lto_file="$module_dir/${FRAMEWORK}_lto.o"
	[[ -f "$module_dir/${FRAMEWORK}_dependency_info.dat" ]] && dep_info="$module_dir/${FRAMEWORK}_dependency_info.dat"
fi

SDK_PATH=$(xcrun --sdk "$SDK" --show-sdk-path)
PLATFORM_PATH=$(xcrun --sdk "$SDK" --show-sdk-platform-path)
CLANG=$(xcrun --sdk "$SDK" --find clang)
SWIFTC_PATH=$(xcrun --find swiftc)
TOOLCHAIN_DIR=$(cd "$(dirname "$SWIFTC_PATH")/.." && pwd)
SWIFT_LIB_DIR="$TOOLCHAIN_DIR/lib/swift/$SDK"

EAGER_TBD_DIR="$DERIVED_DATA/Build/Intermediates.noindex/EagerLinkingTBDs/$CONFIG_SUFFIX"
PLATFORM_DEV_LIB="$PLATFORM_PATH/Developer/usr/lib"
DEVELOPER_FRAMEWORKS="$PLATFORM_PATH/Developer/Library/Frameworks"
SDK_DEVELOPER_FRAMEWORKS="$SDK_PATH/Developer/Library/Frameworks"

TARGET_TRIPLE="${ARCH}-apple-${TRIPLE_BASE}${TARGET_VERSION}"
INSTALL_NAME=$(otool -D "$FRAMEWORK_BINARY_REAL" 2>/dev/null | sed -n '2p' || true)
if [[ -z "$INSTALL_NAME" ]]; then
	if [[ "$platform_lc" = "macos" ]]; then
		INSTALL_NAME="@rpath/$FRAMEWORK.framework/Versions/A/$FRAMEWORK"
	else
		INSTALL_NAME="@rpath/$FRAMEWORK.framework/$FRAMEWORK"
	fi
fi

tmp_binary=$(mktemp "$FRAMEWORK_DIR/.relinked-$ARCH-XXXXXX")

link_args=(
	"$CLANG"
	"-Xlinker" "-reproducible"
	"-target" "$TARGET_TRIPLE"
	"-dynamiclib"
	"-isysroot" "$SDK_PATH"
	"-O0"
	"-filelist" "$filtered_file"
	"-install_name" "$INSTALL_NAME"
	"-Xlinker" "-rpath" "-Xlinker" "/usr/lib/swift"
	"-dead_strip"
	"-rdynamic"
	"-Xlinker" "-no_deduplicate"
	"-Xlinker" "-headerpad_max_install_names"
	"-fobjc-link-runtime"
	"-Wl,-no_warn_duplicate_libraries"
	"-Wl,-make_mergeable"
	"-Xlinker" "-dead_strip"
	"-o" "$tmp_binary"
)

# Library and framework search paths
if [[ -d "$EAGER_TBD_DIR" ]]; then
	link_args+=("-L$EAGER_TBD_DIR" "-F$EAGER_TBD_DIR")
fi
if [[ -d "$PRODUCTS_DIR" ]]; then
	link_args+=("-L$PRODUCTS_DIR" "-F$PRODUCTS_DIR")
fi
if [[ -d "$PRODUCTS_DIR/PackageFrameworks" ]]; then
	link_args+=("-F$PRODUCTS_DIR/PackageFrameworks" "-Xlinker" "-rpath" "-Xlinker" "$PRODUCTS_DIR/PackageFrameworks")
elif [[ -d "$(dirname "$FRAMEWORK_DIR")" ]]; then
	parent_dir="$(dirname "$FRAMEWORK_DIR")"
	link_args+=("-F$parent_dir" "-Xlinker" "-rpath" "-Xlinker" "$parent_dir")
fi
if [[ -d "$PLATFORM_DEV_LIB" ]]; then
	link_args+=("-L$PLATFORM_DEV_LIB")
fi
if [[ -d "$SWIFT_LIB_DIR" ]]; then
	link_args+=("-L$SWIFT_LIB_DIR")
fi
link_args+=("-L/usr/lib/swift")
if [[ -d "$DEVELOPER_FRAMEWORKS" ]]; then
	link_args+=("-iframework" "$DEVELOPER_FRAMEWORKS")
fi
if [[ -d "$SDK_DEVELOPER_FRAMEWORKS" ]]; then
	link_args+=("-iframework" "$SDK_DEVELOPER_FRAMEWORKS")
fi

if [[ -n "$lto_file" ]]; then
	link_args+=("-Xlinker" "-object_path_lto" "-Xlinker" "$lto_file")
fi
if [[ -n "$dep_info" ]]; then
	link_args+=("-Xlinker" "-dependency_info" "-Xlinker" "$dep_info")
fi

if [[ -n "$module_dir" ]]; then
	for ext in swiftmodule swiftdoc swiftinterface swiftsourceinfo abi.json swiftconstvalues; do
		for candidate in "$module_dir"/$FRAMEWORK*".$ext"; do
			if [[ -f "$candidate" ]]; then
				link_args+=("-Xlinker" "-add_ast_path" "-Xlinker" "$candidate")
			fi
		done
	done
fi

if [[ -n "$linker_resp" ]]; then
	link_args+=("@$linker_resp")
fi

echo "Relinking $FRAMEWORK_BINARY_REAL without SwiftSyntax (${platform_lc}/${ARCH})..."
before_size=$(stat -f%z "$FRAMEWORK_BINARY_REAL" 2>/dev/null || stat -c%s "$FRAMEWORK_BINARY_REAL")

"${link_args[@]}"
mv "$tmp_binary" "$FRAMEWORK_BINARY_REAL"
chmod +x "$FRAMEWORK_BINARY_REAL"

if [[ "$platform_lc" = "macos" && "$FRAMEWORK_BINARY" != "$FRAMEWORK_BINARY_REAL" ]]; then
	rm -f "$FRAMEWORK_BINARY"
	ln -s "Versions/Current/$FRAMEWORK" "$FRAMEWORK_BINARY"
fi

after_size=$(stat -f%z "$FRAMEWORK_BINARY_REAL" 2>/dev/null || stat -c%s "$FRAMEWORK_BINARY_REAL")
echo "Relinked successfully: $before_size bytes -> $after_size bytes"
