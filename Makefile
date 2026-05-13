.PHONY: run build build2 package dist xcframework check_swiftsyntax generate-stubs split-generate-stubs gendocs docs-html deploy-docs split-build split-dist split-package split-smoke split-validate split-validate-built split-validate-matrix

# Allow overriding common build knobs.
CONFIG ?= Release
HOST_ARCH ?= $(shell uname -m)
DESTINATIONS ?= generic/platform=iOS generic/platform=iOS\ Simulator platform=macOS,arch=arm64 platform=macOS,arch=x86_64
DERIVED_DATA ?= $(CURDIR)/.xcodebuild
WORKSPACE ?= .swiftpm/xcode/package.xcworkspace
SCHEME ?= GodotApplePlugins
FRAMEWORK_NAMES ?= GodotApplePlugins
SPLIT_FRAMEWORK_NAMES ?= GodotApplePluginsAVFoundation GodotApplePluginsFoundation GodotApplePluginsGameCenter GodotApplePluginsStoreKit GodotApplePluginsAuthenticationServices GodotApplePluginsARKit
SPLIT_RUNTIME_FRAMEWORK ?= SwiftGodotRuntime
XCODEBUILD ?= xcodebuild
XCODEBUILD_LOG_ON_ERROR ?=
XCODEBUILD_LOG_DIR ?=
CC ?= cc
GODOT ?= ~/cvs/master-godot/editor/bin/godot.macos.editor.dev.arm64

STUB_BASE_DIR ?= $(CURDIR)
STUB_OUTPUT_DIR ?= $(STUB_BASE_DIR)/Generated/GodotApplePluginsStub
STUB_HEADERS_DIR ?= $(CURDIR)/.build/checkouts/SwiftGodot/Sources/GDExtension/include
STUB_ENTRY_SYMBOL ?= godot_apple_plugins_start
STUB_LIBRARY_NAME ?= godot_apple_plugins_stub
STUB_FILES ?=
STUB_GENERATOR ?= swift run GodotApplePluginsStubGenerator
SPLIT_SELECTED_FRAMEWORK_NAMES ?= $(SPLIT_FRAMEWORK_NAMES)

empty :=
space := $(empty) $(empty)
comma := ,
SPLIT_SELECTED_FRAMEWORKS_ARG = $(subst $(space),$(comma),$(strip $(SPLIT_SELECTED_FRAMEWORK_NAMES)))

ifeq ($(shell uname -s),Darwin)
STUB_SHARED_EXT ?= dylib
STUB_SHARED_FLAGS ?= -dynamiclib
else
STUB_SHARED_EXT ?= so
STUB_SHARED_FLAGS ?= -shared -fPIC
endif

STUB_SHARED_LIB ?= $(STUB_OUTPUT_DIR)/$(STUB_LIBRARY_NAME).$(STUB_SHARED_EXT)
STUB_FILE_ARGS = $(foreach file,$(STUB_FILES),--file $(file))

run:
	@echo -e "Run make xcframework to produce the binary payloads for all platforms"

generate-stubs:
	@set -e; \
	echo "Generating stub sources from $(STUB_BASE_DIR)"; \
	$(STUB_GENERATOR) "$(STUB_BASE_DIR)" --output "$(STUB_OUTPUT_DIR)" --entry-symbol "$(STUB_ENTRY_SYMBOL)" --library-name "$(STUB_LIBRARY_NAME)" $(STUB_FILE_ARGS); \
	echo "Compiling stub library to $(STUB_SHARED_LIB)"; \
	$(CC) -std=c11 -Wall -Wextra -I "$(STUB_HEADERS_DIR)" $(STUB_SHARED_FLAGS) "$(STUB_OUTPUT_DIR)/$(STUB_LIBRARY_NAME).c" -o "$(STUB_SHARED_LIB)"

build build2:
	set -e; \
	run_xcodebuild() { \
		if [ -n "$(XCODEBUILD_LOG_ON_ERROR)" ]; then \
			log_dir="$(XCODEBUILD_LOG_DIR)"; \
			if [ -n "$$log_dir" ]; then \
				mkdir -p "$$log_dir"; \
			fi; \
			log_file=$$(mktemp "$${log_dir:-$${TMPDIR:-/tmp}}/xcodebuild.XXXXXX"); \
			if ! "$$@" >"$$log_file" 2>&1; then \
				echo "xcodebuild failed; dumping $$log_file"; \
				cat "$$log_file"; \
				return 1; \
			fi; \
			rm -f "$$log_file"; \
		else \
			"$$@"; \
		fi; \
	}; \
	swift build; \
	for dest in $(DESTINATIONS); do \
		platform_name=`echo "$$dest" | sed -n 's/.*platform=\([^,]*\).*/\1/p'`; \
		if [ -z "$$platform_name" ]; then \
			platform_name="iOS"; \
		fi; \
		platform_lc=`echo "$$platform_name" | tr '[:upper:]' '[:lower:]' | tr -d '[:space:]'`; \
		arch_name=`echo $$dest | sed -n 's/.*arch=\([a-zA-Z0-9_]*\).*/\1/p'`; \
		suffix=""; \
		if [ -z "$$arch_name" ] && [ "$$platform_lc" = "ios" ]; then \
			arch_name="arm64"; \
		fi; \
		if [ -z "$$arch_name" ] && [ "$$platform_lc" = "iossimulator" ]; then \
			arch_name="$(HOST_ARCH)"; \
		fi; \
		if [ -z "$$arch_name" ] && [ "$$platform_lc" = "macos" ]; then \
			arch_name=`uname -m`; \
		fi; \
		if [ "$$platform_lc" = "iossimulator" ]; then \
			suffix="simulator"; \
		fi; \
		if [ "$$platform_lc" = "macos" ]; then \
			suffix="$$arch_name"; \
		fi; \
	    for framework in $(FRAMEWORK_NAMES); do \
			run_xcodebuild $(XCODEBUILD) \
				-workspace '$(WORKSPACE)' \
				-scheme $$framework \
				-configuration '$(CONFIG)' \
				-destination "$$dest" \
				-derivedDataPath "$(DERIVED_DATA)$$suffix" \
				build; \
			if [ "$$platform_lc" = "ios" ] || [ "$$platform_lc" = "macos" ]; then \
				relink_platform="$$platform_lc"; \
				$(CURDIR)/relink_without_swiftsyntax.sh \
					--derived-data "$(DERIVED_DATA)$$suffix" \
					--config "$(CONFIG)" \
					--framework $$framework \
					--platform $$relink_platform \
					--arch $$arch_name; \
			else \
				echo "Skipping SwiftSyntax relink for $$framework on $$dest (unsupported platform)"; \
			fi; \
	    done;  \
	done; \

check_swiftsyntax:
	set -e; \
	pattern='SwiftSyntax|SwiftParser|SwiftDiagnostics|SwiftParserDiagnostics|SwiftBasicFormat|_SwiftSyntaxCShims'; \
	failed=0; \
	check_one() { \
		sdk="$$1"; bin="$$2"; label="$$3"; \
		if [ ! -f "$$bin" ]; then \
			echo "SKIP: $$label (missing: $$bin)"; \
			return 0; \
		fi; \
		if xcrun --sdk "$$sdk" nm -gU "$$bin" 2>/dev/null | grep -Eq "$$pattern"; then \
			echo "FAIL: $$label still contains SwiftSyntax-related symbols"; \
			failed=1; \
		else \
			echo "OK:   $$label"; \
		fi; \
	}; \
	for framework in $(FRAMEWORK_NAMES); do \
		check_one iphoneos "$(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$$framework.framework/$$framework" "iOS/$$framework"; \
		check_one macosx "$(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$$framework.framework/$$framework" "macOS arm64/$$framework"; \
		check_one macosx "$(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$$framework.framework/$$framework" "macOS x86_64/$$framework"; \
	done; \
	test "$$failed" -eq 0

package: build dist

split-generate-stubs:
	@set -e; \
	for framework in $(SPLIT_FRAMEWORK_NAMES); do \
		case "$$framework" in \
			GodotApplePluginsAVFoundation) \
				entry_symbol="godot_apple_plugins_avfoundation_start"; \
				library_name="godot_apple_plugins_avfoundation_stub"; \
				files="AVAudioSession"; \
				;; \
			GodotApplePluginsFoundation) \
				entry_symbol="godot_apple_plugins_foundation_start"; \
				library_name="godot_apple_plugins_foundation_stub"; \
				files="Foundation AppleURL AppleFilePicker"; \
				;; \
			GodotApplePluginsGameCenter) \
				entry_symbol="godot_apple_plugins_game_center_start"; \
				library_name="godot_apple_plugins_game_center_stub"; \
				files="GameCenterManager GKAccessPoint GKAchievement GKAchievementChallenge GKAchievementDescription GKChallenge GKChallengeDefinition GKGameCenterViewController GKGameActivity GKGameActivityDefinition GKLocalPlayer GKLeaderboard GKLeaderboardEntry GKLeaderboardScore GKLeaderboardSet GKInvite GKMatch GKMatchmaker GKMatchmakerViewController GKMatchRequest GKNotificationBanner GKScoreChallenge GKTurnBasedExchange GKTurnBasedExchangeReply GKTurnBasedMatch GKTurnBasedMatchmakerViewController GKTurnBasedParticipant GKVoiceChat GKPlayer GKSavedGame GKError"; \
				;; \
			GodotApplePluginsStoreKit) \
				entry_symbol="godot_apple_plugins_storekit_start"; \
				library_name="godot_apple_plugins_storekit_stub"; \
				files="ProductView StoreProduct StoreProductPurchaseOption StoreProductSubscriptionOffer StoreProductPaymentMode StoreProductSubscriptionPeriod StoreSubscriptionInfo StoreSubscriptionInfoStatus StoreSubscriptionInfoRenewalInfo StoreTransaction StoreKitManager StoreView SubscriptionOfferView SubscriptionStoreView"; \
				;; \
			GodotApplePluginsAuthenticationServices) \
				entry_symbol="godot_apple_plugins_authentication_services_start"; \
				library_name="godot_apple_plugins_authentication_services_stub"; \
				files="ASAuthorizationAppleIDCredential ASPasswordCredential ASAuthorizationController ASWebAuthenticationSession"; \
				;; \
			GodotApplePluginsARKit) \
				entry_symbol="godot_apple_plugins_arkit_start"; \
				library_name="godot_apple_plugins_arkit_stub"; \
				files="ARSession ARWorldTrackingConfiguration ARFrame ARCamera ARLightEstimate ARPointCloud ARAnchor ARPlaneAnchor ARRaycastQuery ARRaycastResult ARTrackedRaycast ARImageAnchor ARMeshAnchor ARFaceAnchor ARWorldMap ARBodyTrackingConfiguration ARBodyAnchor ARBodySkeleton ARHandAnchor ARHandSkeleton ARCoachingOverlay AREnvironmentProbeAnchor ARGeoTrackingConfiguration ARGeoAnchor ARCollaborationData"; \
				;; \
			*) \
				echo "Unknown split framework $$framework" >&2; \
				exit 1; \
				;; \
		esac; \
		output_dir="$(CURDIR)/Generated/$${framework}Stub"; \
		echo "Generating stub sources for $$framework into $$output_dir"; \
		args=""; \
		for file in $$files; do \
			args="$$args --file $$file"; \
		done; \
		eval $(STUB_GENERATOR) '"$(CURDIR)"' --output '"$$output_dir"' --entry-symbol '"$$entry_symbol"' --library-name '"$$library_name"' $$args; \
	done

split-build:
	$(MAKE) build FRAMEWORK_NAMES="$(SPLIT_FRAMEWORK_NAMES)"

split-dist:
	$(MAKE) split-generate-stubs
	$(MAKE) dist FRAMEWORK_NAMES="$(SPLIT_FRAMEWORK_NAMES)"
	rm -rf $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK).xcframework; \
	rm -rf $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK).framework; \
	rm -rf $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK)_x64.framework; \
	mkdir -p $(CURDIR)/addons/GodotApplePluginsRuntime/bin; \
	runtime_ios_device="$(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework"; \
	runtime_ios_sim="$(DERIVED_DATA)simulator/Build/Products/$(CONFIG)-iphonesimulator/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework"; \
	if [ ! -d "$$runtime_ios_device" ] || [ ! -d "$$runtime_ios_sim" ]; then \
		echo "Missing $(SPLIT_RUNTIME_FRAMEWORK) iOS build products. Run make split-build before make split-dist." >&2; \
		echo "  $$runtime_ios_device" >&2; \
		echo "  $$runtime_ios_sim" >&2; \
		exit 1; \
	fi; \
	$(XCODEBUILD) -create-xcframework \
		-framework "$$runtime_ios_device" \
		-framework "$$runtime_ios_sim" \
		-output $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK).xcframework; \
	if [ ! -d "$(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework" ]; then \
		echo "Missing $(SPLIT_RUNTIME_FRAMEWORK) macOS x86_64 build product. Run split-build for platform=macOS,arch=x86_64 before split-dist." >&2; \
		exit 1; \
	fi; \
	rsync -a $(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework/ $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK)_x64.framework; \
	if [ ! -d "$(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework" ]; then \
		echo "Missing $(SPLIT_RUNTIME_FRAMEWORK) macOS arm64 build product. Run split-build for platform=macOS,arch=arm64 before split-dist." >&2; \
		exit 1; \
	fi; \
	rsync -a $(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$(SPLIT_RUNTIME_FRAMEWORK).framework/ $(CURDIR)/addons/GodotApplePluginsRuntime/bin/$(SPLIT_RUNTIME_FRAMEWORK).framework; \
	for framework in $(SPLIT_FRAMEWORK_NAMES); do \
		binary_arm="$(CURDIR)/addons/$$framework/bin/$$framework.framework/Versions/A/$$framework"; \
		binary_x64="$(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework/Versions/A/$$framework"; \
		if [ -f "$$binary_arm" ]; then \
			install_name_tool -delete_rpath "$(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks" "$$binary_arm" 2>/dev/null || true; \
			install_name_tool -add_rpath "@loader_path/../../../../../GodotApplePluginsRuntime/bin" "$$binary_arm"; \
		fi; \
		if [ -f "$$binary_x64" ]; then \
			install_name_tool -delete_rpath "$(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks" "$$binary_x64" 2>/dev/null || true; \
			install_name_tool -add_rpath "@loader_path/../../../../../GodotApplePluginsRuntime/bin" "$$binary_x64"; \
		fi; \
	done

split-package: split-build split-dist

split-validate-built:
	@set -e; \
	project_dir=$$(mktemp -d "$${TMPDIR:-/tmp}/godot-split-validate.XXXXXX"); \
	trap 'rm -rf "$$project_dir"' EXIT INT TERM; \
	rsync -a --delete --exclude addons test-apple-godot-api/ "$$project_dir"/; \
	mkdir -p "$$project_dir/addons"; \
	ln -sfn "$(CURDIR)/addons/GodotApplePluginsRuntime" "$$project_dir/addons/GodotApplePluginsRuntime"; \
	for addon in $(SPLIT_SELECTED_FRAMEWORK_NAMES); do \
		ln -sfn "$(CURDIR)/addons/$$addon" "$$project_dir/addons/$$addon"; \
	done; \
	$(GODOT) --headless --path "$$project_dir" --scene res://split_validation.tscn -- --frameworks=$(SPLIT_SELECTED_FRAMEWORKS_ARG)

split-validate: split-package split-validate-built

split-validate-matrix: split-package
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsAVFoundation"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsFoundation"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsGameCenter"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsStoreKit"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsAuthenticationServices"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="GodotApplePluginsARKit"
	$(MAKE) split-validate-built SPLIT_SELECTED_FRAMEWORK_NAMES="$(SPLIT_FRAMEWORK_NAMES)"

split-smoke: split-validate

dist:
	for framework in $(FRAMEWORK_NAMES); do \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework.xcframework; \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework*.framework; \
		if [ -d "$(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$$framework.framework" ] && [ -d "$(DERIVED_DATA)simulator/Build/Products/$(CONFIG)-iphonesimulator/PackageFrameworks/$$framework.framework" ]; then \
			$(XCODEBUILD) -create-xcframework \
				-framework $(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$$framework.framework \
				-framework $(DERIVED_DATA)simulator/Build/Products/$(CONFIG)-iphonesimulator/PackageFrameworks/$$framework.framework \
				-output $(CURDIR)/addons/$$framework/bin/$${framework}.xcframework; \
		fi; \
		if [ -d "$(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework" ]; then \
			rsync -a $(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework; \
			rsync -a doc_classes/ $(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework/Resources/doc_classes/; \
		fi; \
		if [ -d "$(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework" ]; then \
			rsync -a $(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}.framework; \
			rsync -a doc_classes/ $(CURDIR)/addons/$$framework/bin/$${framework}.framework/Resources/doc_classes/; \
		fi; \
	done

XCFRAMEWORK_GODOTAPPLEPLUGINS ?= $(CURDIR)/addons/GodotApplePlugins/bin/GodotApplePlugins.xcframework
DOCS_REMOTE ?= origin
DOCS_BRANCH ?= docs
DOCS_DEPLOY_DIR ?= docs

justgen:
	(cd test-apple-godot-api; ~/cvs/master-godot/editor/bin/godot.macos.editor.dev.arm64 --headless --path . --doctool .. --gdextension-docs)

gendocs docs-html: justgen
	./fix_doc_enums.sh
	$(MAKE) -C doctools html $(if $(GODOT_DOCS_SOURCE),GODOT_DOCS_SOURCE="$(GODOT_DOCS_SOURCE)") $(if $(HTML_OUTPUT),HTML_OUTPUT="$(HTML_OUTPUT)")

deploy-docs:
	@set -eu; \
	site_dir="$$(mktemp -d "$${TMPDIR:-/tmp}/godotappleplugins-site.XXXXXX")"; \
	worktree_dir="$$(mktemp -d "$${TMPDIR:-/tmp}/godotappleplugins-worktree.XXXXXX")"; \
	cleanup() { \
		git worktree remove --force "$$worktree_dir" >/dev/null 2>&1 || true; \
		rm -rf "$$site_dir" "$$worktree_dir"; \
	}; \
	trap cleanup EXIT INT TERM; \
	$(MAKE) docs-html HTML_OUTPUT="$$site_dir" $(if $(GODOT_DOCS_SOURCE),GODOT_DOCS_SOURCE="$(GODOT_DOCS_SOURCE)"); \
	git fetch "$(DOCS_REMOTE)" "$(DOCS_BRANCH)" >/dev/null 2>&1 || true; \
	if ! git rev-parse --verify --quiet "$(DOCS_REMOTE)/$(DOCS_BRANCH)" >/dev/null; then \
		echo "Missing branch $(DOCS_REMOTE)/$(DOCS_BRANCH)"; \
		exit 1; \
	fi; \
	git worktree add --detach "$$worktree_dir" "$(DOCS_REMOTE)/$(DOCS_BRANCH)"; \
	mkdir -p "$$worktree_dir/$(DOCS_DEPLOY_DIR)"; \
	rsync -a --delete "$$site_dir"/ "$$worktree_dir/$(DOCS_DEPLOY_DIR)/"; \
	if [ -z "$$(git -C "$$worktree_dir" status --short -- "$(DOCS_DEPLOY_DIR)")" ]; then \
		echo "No docs changes to publish."; \
		exit 0; \
	fi; \
	git -C "$$worktree_dir" add "$(DOCS_DEPLOY_DIR)"; \
	git -C "$$worktree_dir" commit -m "Update generated docs for $$(git rev-parse --short HEAD)"; \
	git -C "$$worktree_dir" push "$(DOCS_REMOTE)" HEAD:refs/heads/$(DOCS_BRANCH)

#
# Quick hacks I use for rapid iteration
#
# My hack is that I build on Xcode for Mac and iPad first, then I
# iterate by just rebuilding in one platform, and then running
# "make o" here over and over, and my Godot project already has a
# symlink here, so I can test quickly on desktop against the Mac 
# API.
o:
	rm -rf '$(XCFRAMEWORK_GODOTAPPLEPLUGINS)'; \
	rm -rf addons/GodotApplePlugins/bin/GodotApplePlugins.framework; \
	$(XCODEBUILD) -create-xcframework \
		-framework ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug-iphoneos/PackageFrameworks/GodotApplePlugins.framework/ \
		-output '$(XCFRAMEWORK_GODOTAPPLEPLUGINS)'
	cp -pr ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug/PackageFrameworks/GodotApplePlugins.framework addons/GodotApplePlugins/bin/GodotApplePlugins.framework
	rsync -a doc_classes/ addons/GodotApplePlugins/bin/GodotApplePlugins.framework/Resources/doc_classes/
#
# This I am using to test on the "Exported" project I placed
#
XCFRAMEWORK_EXPORT_PATH=test-apple-godot-api/demo/output/dylibs/addons/GodotApplePlugins/bin/GodotApplePlugins.xcframework
make oo:
	rm -rf $(XCFRAMEWORK_EXPORT_PATH)
	$(XCODEBUILD) -create-xcframework \
		-framework ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug-iphoneos/PackageFrameworks/GodotApplePlugins.framework/ \
		-framework ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug/PackageFrameworks/GodotApplePlugins.framework/ \
		-output '$(XCFRAMEWORK_EXPORT_PATH)'	
