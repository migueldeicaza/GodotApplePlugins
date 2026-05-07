import SwiftGodotRuntime

private func makeGodotApplePluginsAuthenticationServicesTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            ASAuthorizationAppleIDCredential.self,
            ASPasswordCredential.self,
            ASAuthorizationController.self,
            ASWebAuthenticationSession.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare Authentication Services registrations: \(error)")
    }
}

private let godotApplePluginsAuthenticationServicesTypes = makeGodotApplePluginsAuthenticationServicesTypes()

public let godotApplePluginsAuthenticationServicesMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsAuthenticationServicesTypes
)

public func godotApplePluginsAuthenticationServicesInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsAuthenticationServicesTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(ASAuthorizationAppleIDCredential.UserDetectionStatus.self)
        registerEnum(ASAuthorizationAppleIDCredential.UserAgeRange.self)
    } else if level == .editor {
        EditorInterop.loadLibraryDocs()
    }
}

public func godotApplePluginsAuthenticationServicesDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsAuthenticationServicesTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_authentication_services_start")
public func godotApplePluginsAuthenticationServicesStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsAuthenticationServicesInitialize,
        deInitHook: godotApplePluginsAuthenticationServicesDeinitialize,
        minimumInitializationLevel: godotApplePluginsAuthenticationServicesMinimumInitializationLevel
    )
    return 1
}
