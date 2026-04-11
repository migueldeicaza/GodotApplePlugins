![banner](./doctools/GodotApplePlugins.webp)

Godot Plugins for deep Apple platform integration across macOS, iOS, and visionOS.

<p align="center">
<strong>
	<a href="https://migueldeicaza.github.io/GodotApplePlugins/index.html">API Documentation</a> | Download: <a href="https://godotengine.org/asset-library/asset/4552">Godot Asset Library</a> | Download: <a href="https://github.com/migueldeicaza/GodotApplePlugins/releases">GitHub Releases</a> | Discord <a href="https://discord.gg/bHAsTYaCZM">#godot-apple-plugins</a>
</strong>
</p>

You can get a ready-to-use binary from the "releases" tab, just drag the contents  into 
your addons directory.   You can start testing right away  on a Mac project, and for iOS, 
export your iOS project and run.

This add-on currently includes comprehensive support for:

* GameCenter [GameCenter Integration Guide](Sources/GodotApplePlugins/GameCenter/GameCenterGuide.md)
* StoreKit2 (https://migueldeicaza.github.io/GodotApplePlugins/class_storekitmanager.html)
* Sign-in with Apple (AuthenticationServices)
* AppleFilePicker: allow your application to invoke the file system picker.
* ARKit: world tracking sessions, frames/camera data, anchors, raycasts, world maps, body and hand tracking, geo tracking, coaching overlays, environment probes, and collaboration data. See the API docs for platform-specific availability.

## ARKit

The ARKit binding is documented in `doc_classes/` and in the published API reference. The surface includes:

* Core session types: `ARSession`, `ARWorldTrackingConfiguration`, `ARFrame`, `ARCamera`, `ARLightEstimate`, `ARPointCloud`
* Anchors: `ARAnchor`, `ARPlaneAnchor`, `ARImageAnchor`, `ARMeshAnchor`, `ARFaceAnchor`, `ARBodyAnchor`, `ARHandAnchor`, `AREnvironmentProbeAnchor`, `ARGeoAnchor`
* Session helpers: `ARRaycastQuery`, `ARRaycastResult`, `ARTrackedRaycast`, `ARWorldMap`, `ARCoachingOverlay`, `ARCollaborationData`

Platform notes:

* iOS provides the broadest ARKit coverage.
* visionOS uses the provider-based `ARKitSession` backend.
* macOS uses the remote-device ARKit session workflow exposed by Apple.

The release contains both binaries for MacOS as dynamic libraries and
an iOS xcframework compiled with the "Mergeable Library" feature.
This means that for Debug builds, your Godot game contains a dynamic
library (about 10 megs at the time of this writing) that does not need
to be copied on every build speeding your development, but you can
switch to "Release Mode" and set "Create Merged Binary" to "Manual"
and you will further reduce the size of your executable (about 1.7
megs at the time of this writing).

# Requirements

iOS 17.0, macOS 14.0, visionOS 1.0

# Stub Generator

This repository also includes a stub generator for producing a C GDExtension library that mirrors the documented `GodotApplePlugins` API surface for non-Apple targets like Windows and Linux.

You can point the generator at any checkout directory instead of changing into that directory first:

```sh
swift run GodotApplePluginsStubGenerator /path/to/GodotApplePlugins
swift run GodotApplePluginsStubGenerator /path/to/GodotApplePlugins --output Generated/GodotApplePluginsStub
swift run GodotApplePluginsStubGenerator --directory /path/to/GodotApplePlugins --docs doc_classes --output Generated/GodotApplePluginsStub
```

By default it reads `doc_classes/` under the directory you point it at and writes the generated stub package to `Generated/GodotApplePluginsStub/`.

You can also generate a sliced stub surface by naming specific XML files or class names. The generator automatically includes documented parent classes required by inheritance:

```sh
swift run GodotApplePluginsStubGenerator /path/to/GodotApplePlugins --file GKPlayer.xml --file GKLocalPlayer.xml
swift run GodotApplePluginsStubGenerator /path/to/GodotApplePlugins --files GKPlayer.xml,GKLocalPlayer.xml
```

There is also a Makefile entry point that generates the C stubs and compiles the generated C into a shared library for the current host:

```sh
make generate-stubs
make generate-stubs STUB_BASE_DIR=/path/to/GodotApplePlugins
make generate-stubs STUB_BASE_DIR=/path/to/GodotApplePlugins STUB_FILES="GKPlayer.xml GKLocalPlayer.xml"
```

Useful overrides:

- `STUB_BASE_DIR`: directory containing `doc_classes/`
- `STUB_OUTPUT_DIR`: where the generated C package and compiled library should go
- `STUB_FILES`: optional space-separated list of XML files or class names to include
- `STUB_HEADERS_DIR`: directory containing `gdextension_interface.h`

# API Design

The API surfaced by this add-ons is to be as close to possible to the Apple APIs (classes, methods names, enumerations) and to avoid attempting to provide an abstraction over them - as these tend to have impedance mismatches.  

In place of Apple delegate's pattern, I use Godot's callbacks - and I surfaced properties and methods use snake-case instead of Apple's camelCase, but beyond that, the mapping should be almost identical.

Both GameCenter and AuthenticationServices APIs use class names that are 1:1 mappings to Apple's APIs as they use 2-letter namespaces (GK, AS) and they are not likely to conflicth with your code.   For the StoreKit API, I chose to change the names as these APIs use terms that are too general (Store, Product) and could clash with your own code.

# Notes on the APIs 

## AuthenticationServices

Make sure that your iOS or Mac app have the `com.apple.developer.applesignin` entitlment.   
When I am debugging this myself on macOS, I resign the official
Godot download with this entitlement (you must download a provisioning profile that
contains the entitlement, or the APIs will fail).

For very simple uses, you can use:

```gdscript
var auth_controller = ASAuthorizationController.new()

func _ready():
    auth_controller.authorization_completed.connect(_on_authorization_completed)
    auth_controller.authorization_failed.connect(_on_authorization_failed)

func _on_sign_in_button_pressed():
    # Request full name and email
    auth_controller.signin_with_scopes(["full_name", "email"])

func _on_authorization_completed(credential):
    if credential is ASAuthorizationAppleIDCredential:
        print("User ID: ", credential.user)
        print("Email: ", credential.email)
        print("Full Name: ", credential.fullName)
    elif credential is ASPasswordCredential:
        print("User: ", credential.user)
        print("Password: ", credential.password)
```

For more advance users, you will find that the API replicates Apple's API, and 
it surfaces the various features that you expect from it.

### Configure

For iOS, set at Project -> Export -> iOS -> `entitlements/additional`:

```xml
<key>com.apple.developer.applesignin</key>
<array>
    <string>Default</string>
</array>
```

For macOS, set the same entitlements as above (eg. when running codesign):

```sh
codesign --force --options=runtime --verbose --timestamp \
  --entitlements entitlements.plist --sign "<SIGN_ENTITY>" \
  "MyApp.app/Contents/MacOS/MyApp"
```

where `entitlements.plist` contains again:

```xml
<key>com.apple.developer.applesignin</key>
<array>
    <string>Default</string>
</array>
```

# Size

This addon adds 2.5 megabytes to your executable for release builds, but it is
larger during development to speed up your development.

Plain Godot Export, empty:

```
Debug:   104 MB
Release:  93 MB
```

Godot Export, adding GodotApplePlugins with mergeable libraries:

```
Debug:   107 MB
Release:  95 MB
```

If you manually disable mergeable libraries and build your own addon:

```
Debug:   114 MB
Release: 105 MB
```

# Credits

The "AuthenticationServices" code was derived from [Dragos Daian's/
Nirmal Ac's](https://github.com/appsinacup/godot-apple-login) binding and 
Xogot's own use.   Dragos also provided extensive technical guidance on 
putting together this addon for distribution.   Thank you!
