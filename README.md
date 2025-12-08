Plugins for some Apple-specific tasks, works on MacOS and iOS.

You can get a ready-to-use binary from the "releases" tab, just drag the contents  into your addons directory.   You can start testing right away 
on a Mac project, and for iOS, export your iOS project and run.

This add-on currently includes support for:

* GameCenter [GameCenter Integration Guide](Sources/GodotApplePlugins/GameCenter/GameCenterGuide.md)
* StoreKit2
* AuthenticationServices ("sign-in with Apple")

The release contains both binaries for MacOS as dynamic libraries and
an iOS xcframework compiled with the "Mergeable Library" feature.
This means that for Debug builds, your Godot game contains a dynamic
library (about 10 megs at the time of this writing) that does not need
to be copied on every build speeding your development, but you can
switch to "Release Mode" and set "Create Merged Binary" to "Manual"
and you will further reduce the size of your executable (about 1.7
megs at the time of this writing.)

# API Design

The API surfaced by this add-ons is to be as close to possible to the Apple APIs (classes, methods names, enumerations) and to avoid attempting to provide an abstraction over them - as these tend to have impedance mismatches.  

In place of Apple delegate's pattern, I use Godot's callbacks - and I surfaced properties and methods use snake-case instead of Apple's camelCase, but beyond that, the mapping should be almost identical.

Both GameCenter and AuthenticationServices APIs use class names that are 1:1 mappings to Apple's APIs as they use 2-letter namespaces (GK, AS) and they are not likely to conflicth with your code.   For the StoreKit API, I chose to change the names as these APIs use terms that are too general (Store, Product) and could clash with your own code.

# After you download

Notice that MacOS will not let you load these dynamic libraries until you remove the 
quarantine attribute from them.

So you need to run this on the binaries after you unpack them:

```
xattr -dr com.apple.quarantine addons/GodotApplePlugins/bin/*framework
```

# Notes on the APIs 

## AuthenticationServices

Make sure that your iOS or Mac app have the `com.apple.developer.applesignin` entitlment.   When I am debugging this myself on macOS, I resign the official
Godot download with this entitlement.

When you do, you will need your own app identifier, and you need this identifier to have the 

For very simple uses, you can use:

```gdscript
var auth_controller = ASAuthorizationController.new()

func _ready():
    auth_controller.authorization_completed.connect(_on_authorization_completed)
    auth_controller.authorization_failed.connect(_on_authorization_failed)

func _on_sign_in_button_pressed():
    # Request full name and email
    auth_controller.perform_apple_id_request(["full_name", "email"])

func _on_authorization_completed(credential):
    if credential is ASAuthorizationAppleIDCredential:
        print("User ID: ", credential.user)
        print("Email: ", credential.email)
        print("Full Name: ", credential.fullName)
    elif credential is ASPasswordCredential:
        print("User: ", credential.user)
        print("Password: ", credential.password)
```

For more advance users, you will find that the API replicates Apple's API, and it surfaces the various features that you expect from it.

# Sizes

This compares the size of a Godot iOS export when you add this addon.

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
