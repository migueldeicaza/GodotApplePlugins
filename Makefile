PHONY: run xcframework

# Allow overriding common build knobs.
CONFIG ?= Debug
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
		echo HERE: $$suffix; \
	    for framework in $(FRAMEWORK_NAMES); do \
		$(XCODEBUILD) \
			-workspace '$(WORKSPACE)' \
			-scheme $$framework \
			-configuration '$(CONFIG)' \
			-destination "$$dest" \
			-derivedDataPath "$(DERIVED_DATA)$$suffix" \
			build; \
	    done;  \
	done; \

package: build
	for framework in $(FRAMEWORK_NAMES); do \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework.xcframework; \
		rm -rf $(CURDIR)/addons/$$framework/bin/$$framework*.framework; \
		$(XCODEBUILD) -create-xcframework \
			-framework $(DERIVED_DATA)/Build/Products/$(CONFIG)-iphoneos/PackageFrameworks/$$framework.framework \
			-output $(CURDIR)/addons/$$framework/bin/$${framework}.xcframework; \
		rsync -a $(DERIVED_DATA)x86_64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}.framework; \
		rsync -a $(DERIVED_DATA)arm64/Build/Products/$(CONFIG)/PackageFrameworks/$${framework}.framework/ $(CURDIR)/addons/$$framework/bin/$${framework}_x64.framework; \
	done

XCFRAMEWORK_GODOTAPPLEPLUGINS ?= $(CURDIR)/addons/GodotApplePlugins/bin/GodotApplePlugins.xcframework

gendocs:
	(cd test-apple-godot-api; ~/cvs/master-godot/editor/bin/godot.macos.editor.dev.arm64 --headless --path . --doctool .. --gdextension-docs)

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

#
# This I am using to test on the "Exported" project I placed
#
XCFRAMEWORK_EXPORT_PATH=test-apple-godot-api/TestAppleGodotApi/dylibs/addons/GodotApplePlugins/bin/GodotApplePlugins.xcframework
make oo:
	rm -rf $(XCFRAMEWORK_EXPORT_PATH)
	$(XCODEBUILD) -create-xcframework \
		-framework ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug-iphoneos/PackageFrameworks/GodotApplePlugins.framework/ \
		-framework ~/DerivedData/GodotApplePlugins-*/Build/Products/Debug/PackageFrameworks/GodotApplePlugins.framework/ \
		-output '$(XCFRAMEWORK_EXPORT_PATH)'	
