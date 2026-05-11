import SwiftGodotRuntime

private func makeGodotApplePluginsFoundationTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            Foundation.self,
            AppleURL.self,
            AppleFilePicker.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare Foundation registrations: \(error)")
    }
}

private let godotApplePluginsFoundationTypes = makeGodotApplePluginsFoundationTypes()

public let godotApplePluginsFoundationMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsFoundationTypes
)

public func godotApplePluginsFoundationInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsFoundationTypes[level]?.forEach(register)
    if level == .editor {
        EditorInterop.loadLibraryDocs()
    }
}

public func godotApplePluginsFoundationDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsFoundationTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_foundation_start")
public func godotApplePluginsFoundationStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsFoundationInitialize,
        deInitHook: godotApplePluginsFoundationDeinitialize,
        minimumInitializationLevel: godotApplePluginsFoundationMinimumInitializationLevel
    )
    return 1
}
