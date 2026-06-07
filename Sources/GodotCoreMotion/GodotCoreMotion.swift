import SwiftGodotRuntime

private func makeGodotApplePluginsCoreMotionTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            CMAccelerometerData.self,
            CMGyroData.self,
            CMMagnetometerData.self,
            CMDeviceMotion.self,
            CMMotionManager.self,
            CMPedometerData.self,
            CMPedometer.self,
            CMAltitudeData.self,
            CMAbsoluteAltitudeData.self,
            CMAltimeter.self,
            CMMotionActivity.self,
            CMMotionActivityManager.self,
            CMHeadphoneMotionManager.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare CoreMotion registrations: \(error)")
    }
}

private let godotApplePluginsCoreMotionTypes = makeGodotApplePluginsCoreMotionTypes()

public let godotApplePluginsCoreMotionMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsCoreMotionTypes
)

public func godotApplePluginsCoreMotionInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsCoreMotionTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(CMMotionManager.AttitudeReferenceFrame.self)
        registerEnum(CMDeviceMotion.MagneticFieldCalibrationAccuracy.self)
        registerEnum(CMMotionActivity.Confidence.self)
    } else if level == .editor {
#if os(macOS)
        loadEmbeddedCoreMotionDocs()
#endif
    }
}

public func godotApplePluginsCoreMotionDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsCoreMotionTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_core_motion_start")
public func godotApplePluginsCoreMotionStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsCoreMotionInitialize,
        deInitHook: godotApplePluginsCoreMotionDeinitialize,
        minimumInitializationLevel: godotApplePluginsCoreMotionMinimumInitializationLevel
    )
    return 1
}
