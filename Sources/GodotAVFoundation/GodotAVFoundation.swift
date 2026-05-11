import SwiftGodotRuntime

private func makeGodotApplePluginsAVFoundationTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            AVAudioSession.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare AVFoundation registrations: \(error)")
    }
}

private let godotApplePluginsAVFoundationTypes = makeGodotApplePluginsAVFoundationTypes()

public let godotApplePluginsAVFoundationMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsAVFoundationTypes
)

public func godotApplePluginsAVFoundationInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsAVFoundationTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(AVAudioSession.CategoryOptions.self)
        registerEnum(AVAudioSession.RouteSharingPolicy.self)
        registerEnum(AVAudioSession.SessionCategory.self)
        registerEnum(AVAudioSession.SessionMode.self)
    } else if level == .editor {
        EditorInterop.loadLibraryDocs()
    }
}

public func godotApplePluginsAVFoundationDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsAVFoundationTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_avfoundation_start")
public func godotApplePluginsAVFoundationStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsAVFoundationInitialize,
        deInitHook: godotApplePluginsAVFoundationDeinitialize,
        minimumInitializationLevel: godotApplePluginsAVFoundationMinimumInitializationLevel
    )
    return 1
}
