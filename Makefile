.PHONY: run xcframework check_swiftsyntax

# Allow overriding common build knobs.
CONFIG ?= Release
DESTINATIONS ?= generic/platform=iOS platform=macOS,arch=arm64 platform=macOS,arch=x86_64
DERIVED_DATA ?= $(CURDIR)/.xcodebuild
WORKSPACE ?= .swiftpm/xcode/package.xcworkspace
SCHEME ?= GodotApplePlugins
FRAMEWORK_NAMES ?= GodotApplePlugins
XCODEBUILD ?= xcodebuild

run:
	@echo -e "Run make xcframework to produce the binary payloads for all platforms"

build:
	set -e; \
	swift build; \
	for dest in $(DESTINATIONS); do \
		suffix=`echo $$dest | sed 's,generic/platform=[a-zA-Z]*,,' | sed 's,platform=[a-zA-Z]*,,' | sed 's/,arch=//'`; \
		platform_name=`echo $$dest | sed -n 's/.*platform=\([a-zA-Z0-9_]*\).*/\1/p'`; \
		if [ -z "$$platform_name" ]; then \
			platform_name="iOS"; \
		fi; \
		platform_lc=`echo $$platform_name | tr '[:upper:]' '[:lower:]'`; \
		arch_name=`echo $$dest | sed -n 's/.*arch=\([a-zA-Z0-9_]*\).*/\1/p'`; \
		if [ -z "$$arch_name" ] && [ "$$platform_lc" = "ios" ]; then \
			arch_name="arm64"; \
		fi; \
		if [ -z "$$arch_name" ] && [ "$$platform_lc" = "macos" ]; then \
			arch_name=`uname -m`; \
		fi; \
		echo HERE: $$suffix; \
	    for framework in $(FRAMEWORK_NAMES); do \
		$(XCODEBUILD) \
			-workspace '$(WORKSPACE)' \
			-scheme $$framework \
			-configuration '$(CONFIG)' \
			-destination "$$dest" \
			-derivedDataPath "$(DERIVED_DATA)$$suffix" \
			build; \
		if [ "$$platform_lc" = "ios" ] || [ "$$platform_lc" = "macos" ]; then \
			$(CURDIR)/relink_without_swiftsyntax.sh \
				--derived-data "$(DERIVED_DATA)$$suffix" \
				--config "$(CONFIG)" \
				--framework $$framework \
				--platform $$platform_lc \
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

dist:
	for framework in $(FRAMEWORK_NAMES); do \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework.xcframework; \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework*.framework; \
		$(XCODEBUILD) -create-xcframework \
			-framework $(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$$framework.framework \
			-output $(CURDIR)/addons/$$framework/bin/$${framework}.xcframework; \
		rsync -a $(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework; \
		rsync -a $(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}.framework; \
		rsync -a doc_classes/ $(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework/Resources/doc_classes/; \
		rsync -a doc_classes/ $(CURDIR)/addons/$$framework/bin/$${framework}.framework/Resources/doc_classes/; \
	done

XCFRAMEWORK_GODOTAPPLEPLUGINS ?= $(CURDIR)/addons/GodotApplePlugins/bin/GodotApplePlugins.xcframework

justgen:
	(cd test-apple-godot-api; ~/cvs/master-godot/editor/bin/godot.macos.editor.dev.arm64 --headless --path . --doctool .. --gdextension-docs)

gendocs: justgen
	./fix_doc_enums.sh
	$(MAKE) -C doctools html

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
