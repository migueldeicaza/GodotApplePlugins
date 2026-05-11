import SwiftGodotRuntime

private func makeGodotApplePluginsARKitTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            ARSession.self,
            ARWorldTrackingConfiguration.self,
            ARFrame.self,
            ARCamera.self,
            ARLightEstimate.self,
            ARPointCloud.self,
            ARAnchor.self,
            ARPlaneAnchor.self,
            ARRaycastQuery.self,
            ARRaycastResult.self,
            ARTrackedRaycast.self,
            ARImageAnchor.self,
            ARMeshAnchor.self,
            ARFaceAnchor.self,
            ARWorldMap.self,
            ARBodyTrackingConfiguration.self,
            ARBodyAnchor.self,
            ARBodySkeleton.self,
            ARHandAnchor.self,
            ARHandSkeleton.self,
            ARCoachingOverlay.self,
            AREnvironmentProbeAnchor.self,
            ARGeoTrackingConfiguration.self,
            ARGeoAnchor.self,
            ARCollaborationData.self,
            ARFaceTrackingConfiguration.self,
            ARImageTrackingConfiguration.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare ARKit registrations: \(error)")
    }
}

private let godotApplePluginsARKitTypes = makeGodotApplePluginsARKitTypes()

public let godotApplePluginsARKitMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsARKitTypes
)

public func godotApplePluginsARKitInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsARKitTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(ARSession.RunOption.self)
        registerEnum(ARWorldTrackingConfiguration.WorldAlignment.self)
        registerEnum(ARWorldTrackingConfiguration.EnvironmentTexturing.self)
        registerEnum(ARFrame.WorldMappingStatus.self)
        registerEnum(ARCamera.TrackingState.self)
        registerEnum(ARCamera.TrackingStateReason.self)
        registerEnum(ARPlaneAnchor.Alignment.self)
        registerEnum(ARPlaneAnchor.ClassificationStatus.self)
        registerEnum(ARPlaneAnchor.Classification.self)
        registerEnum(ARRaycastQuery.Target.self)
        registerEnum(ARRaycastQuery.TargetAlignment.self)
        registerEnum(ARRaycastResult.Target.self)
        registerEnum(ARRaycastResult.TargetAlignment.self)
        registerEnum(ARMeshAnchor.MeshClassification.self)
        registerEnum(ARFaceAnchor.BlendShapeLocation.self)
        registerEnum(ARHandAnchor.Chirality.self)
        registerEnum(ARHandSkeleton.JointName.self)
        registerEnum(ARCoachingOverlay.Goal.self)
        registerEnum(ARGeoAnchor.AltitudeSource.self)
        registerEnum(ARCollaborationData.Priority.self)
    } else if level == .editor {
        EditorInterop.loadLibraryDocs()
    }
}

public func godotApplePluginsARKitDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsARKitTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_arkit_start")
public func godotApplePluginsARKitStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsARKitInitialize,
        deInitHook: godotApplePluginsARKitDeinitialize,
        minimumInitializationLevel: godotApplePluginsARKitMinimumInitializationLevel
    )
    return 1
}
