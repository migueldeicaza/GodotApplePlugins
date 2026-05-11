//
//  ARWorldTrackingConfiguration.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARWorldTrackingConfiguration: RefCounted, @unchecked Sendable {
    enum WorldAlignment: Int, CaseIterable {
        case GRAVITY = 0
        case GRAVITY_AND_HEADING = 1
        case CAMERA = 2
    }

    enum EnvironmentTexturing: Int, CaseIterable {
        case NONE = 0
        case MANUAL = 1
        case AUTOMATIC = 2
    }

    enum PlaneDetection: Int, CaseIterable {
        case NONE = 0
        case HORIZONTAL = 1
        case VERTICAL = 2
        case SLANTED = 4
    }

    enum SceneReconstruction: Int, CaseIterable {
        case NONE = 0
        case MESH = 1
        case MESH_WITH_CLASSIFICATION = 3
    }

    @Export(.enum) var worldAlignment: WorldAlignment = .GRAVITY
    @Export(.enum) var environmentTexturing: EnvironmentTexturing = .NONE
    @Export(.enum) var sceneReconstruction: SceneReconstruction = .NONE
    @Export var planeDetectionMask: Int = PlaneDetection.NONE.rawValue
    @Export var isLightEstimationEnabled: Bool = true
    @Export var isAutoFocusEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var automaticImageScaleEstimationEnabled: Bool = false
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isCollaborationEnabled: Bool = false
    @Export var userFaceTrackingEnabled: Bool = false
    @Export var appClipCodeTrackingEnabled: Bool = false
    @Export var handTrackingEnabled: Bool = false
    @Export var detectionImageGroupName: String = ""
    @Export var frameSemanticsMask: Int = FrameSemantics.NONE.rawValue

    enum FrameSemantics: Int, CaseIterable {
        case NONE = 0
        case PERSON_SEGMENTATION = 1
        case PERSON_SEGMENTATION_WITH_DEPTH = 2
        case BODY_DETECTION = 4
    }

    private var detectionImages: Set<ARKit.ARReferenceImage> = []

    @Callable
    static func is_supported() -> Bool {
        ARKit.ARWorldTrackingConfiguration.isSupported
    }

    @Callable
    func set_detection_image_group(groupName: String) -> Bool {
        guard let images = ARKit.ARReferenceImage.referenceImages(inGroupNamed: groupName, bundle: nil) else {
            return false
        }
        detectionImageGroupName = groupName
        detectionImages = images
        return true
    }

    @Callable
    static func supports_user_face_tracking() -> Bool {
        if #available(iOS 13.0, *) {
            return ARKit.ARWorldTrackingConfiguration.supportsUserFaceTracking
        }
        return false
    }

    @Callable
    static func supports_app_clip_code_tracking() -> Bool {
        if #available(iOS 14.3, *) {
            return ARKit.ARWorldTrackingConfiguration.supportsAppClipCodeTracking
        }
        return false
    }

    @Callable
    static func supports_scene_reconstruction(scene_reconstruction: SceneReconstruction) -> Bool {
        if #available(iOS 13.4, *) {
            return ARKit.ARWorldTrackingConfiguration.supportsSceneReconstruction(
                scene_reconstruction.toNative()
            )
        }
        return false
    }

    func makeNative() -> ARKit.ARWorldTrackingConfiguration {
        let configuration = ARKit.ARWorldTrackingConfiguration()
        configuration.worldAlignment = switch worldAlignment {
        case .GRAVITY: .gravity
        case .GRAVITY_AND_HEADING: .gravityAndHeading
        case .CAMERA: .camera
        }
        configuration.environmentTexturing = switch environmentTexturing {
        case .NONE: .none
        case .MANUAL: .manual
        case .AUTOMATIC: .automatic
        }
        configuration.sceneReconstruction = sceneReconstruction.toNative()
        configuration.planeDetection = makePlaneDetection()
        configuration.isLightEstimationEnabled = isLightEstimationEnabled
        configuration.isAutoFocusEnabled = isAutoFocusEnabled
        configuration.providesAudioData = providesAudioData
        if #available(iOS 13.0, *) {
            configuration.wantsHDREnvironmentTextures = wantsHDREnvironmentTextures
            configuration.automaticImageScaleEstimationEnabled = automaticImageScaleEstimationEnabled
            configuration.isCollaborationEnabled = isCollaborationEnabled
            configuration.userFaceTrackingEnabled = userFaceTrackingEnabled
        }
        if #available(iOS 12.0, *) {
            configuration.maximumNumberOfTrackedImages = maximumNumberOfTrackedImages
        }
        if #available(iOS 14.3, *) {
            configuration.appClipCodeTrackingEnabled = appClipCodeTrackingEnabled
        }
        if #available(iOS 13.0, *) {
            var semantics: ARKit.ARConfiguration.FrameSemantics = []
            if frameSemanticsMask & FrameSemantics.PERSON_SEGMENTATION.rawValue != 0 {
                semantics.insert(.personSegmentation)
            }
            if frameSemanticsMask & FrameSemantics.PERSON_SEGMENTATION_WITH_DEPTH.rawValue != 0 {
                semantics.insert(.personSegmentationWithDepth)
            }
            if frameSemanticsMask & FrameSemantics.BODY_DETECTION.rawValue != 0 {
                semantics.insert(.bodyDetection)
            }
            if !semantics.isEmpty {
                configuration.frameSemantics = semantics
            }
        }
        if !detectionImages.isEmpty {
            configuration.detectionImages = detectionImages
        } else if !detectionImageGroupName.isEmpty {
            if let images = ARKit.ARReferenceImage.referenceImages(inGroupNamed: detectionImageGroupName, bundle: nil) {
                configuration.detectionImages = images
            }
        }
        return configuration
    }

    private func makePlaneDetection() -> ARKit.ARWorldTrackingConfiguration.PlaneDetection {
        var result: ARKit.ARWorldTrackingConfiguration.PlaneDetection = []
        if planeDetectionMask & PlaneDetection.HORIZONTAL.rawValue != 0 {
            result.insert(.horizontal)
        }
        if planeDetectionMask & PlaneDetection.VERTICAL.rawValue != 0 {
            result.insert(.vertical)
        }
        return result
    }
}

private extension ARWorldTrackingConfiguration.SceneReconstruction {
    func toNative() -> ARKit.ARConfiguration.SceneReconstruction {
        switch self {
        case .NONE:
            return []
        case .MESH:
            return .mesh
        case .MESH_WITH_CLASSIFICATION:
            return .meshWithClassification
        }
    }
}

#elseif canImport(ARKit) && os(visionOS)
import ARKit

@Godot
class ARWorldTrackingConfiguration: RefCounted, @unchecked Sendable {
    enum WorldAlignment: Int, CaseIterable {
        case GRAVITY = 0
        case GRAVITY_AND_HEADING = 1
        case CAMERA = 2
    }

    enum EnvironmentTexturing: Int, CaseIterable {
        case NONE = 0
        case MANUAL = 1
        case AUTOMATIC = 2
    }

    enum PlaneDetection: Int, CaseIterable {
        case NONE = 0
        case HORIZONTAL = 1
        case VERTICAL = 2
        case SLANTED = 4
    }

    enum SceneReconstruction: Int, CaseIterable {
        case NONE = 0
        case MESH = 1
        case MESH_WITH_CLASSIFICATION = 3
    }

    @Export(.enum) var worldAlignment: WorldAlignment = .GRAVITY
    @Export(.enum) var environmentTexturing: EnvironmentTexturing = .NONE
    @Export(.enum) var sceneReconstruction: SceneReconstruction = .NONE
    @Export var planeDetectionMask: Int = PlaneDetection.NONE.rawValue
    @Export var isLightEstimationEnabled: Bool = true
    @Export var isAutoFocusEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var automaticImageScaleEstimationEnabled: Bool = false
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isCollaborationEnabled: Bool = false
    @Export var userFaceTrackingEnabled: Bool = false
    @Export var appClipCodeTrackingEnabled: Bool = false
    @Export var handTrackingEnabled: Bool = false
    @Export var detectionImageGroupName: String = ""
    @Export var frameSemanticsMask: Int = FrameSemantics.NONE.rawValue

    enum FrameSemantics: Int, CaseIterable {
        case NONE = 0
        case PERSON_SEGMENTATION = 1
        case PERSON_SEGMENTATION_WITH_DEPTH = 2
        case BODY_DETECTION = 4
    }

    @Callable
    static func is_supported() -> Bool {
        WorldTrackingProvider.isSupported
    }

    @Callable
    func set_detection_image_group(groupName: String) -> Bool {
        guard ReferenceImage.loadReferenceImages(inGroupNamed: groupName) != nil else {
            return false
        }
        detectionImageGroupName = groupName
        return true
    }

    @Callable
    static func supports_user_face_tracking() -> Bool {
        false
    }

    @Callable
    static func supports_app_clip_code_tracking() -> Bool {
        false
    }

    @Callable
    static func supports_scene_reconstruction(scene_reconstruction: SceneReconstruction) -> Bool {
        guard scene_reconstruction != .NONE else { return true }
        return SceneReconstructionProvider.isSupported
    }

    fileprivate func makePlaneAlignments() -> [PlaneAnchor.Alignment] {
        var result: [PlaneAnchor.Alignment] = []
        if planeDetectionMask & PlaneDetection.HORIZONTAL.rawValue != 0 {
            result.append(.horizontal)
        }
        if planeDetectionMask & PlaneDetection.VERTICAL.rawValue != 0 {
            result.append(.vertical)
        }
        if #available(visionOS 2.0, *), planeDetectionMask & PlaneDetection.SLANTED.rawValue != 0 {
            result.append(.slanted)
        }
        return result
    }
}

#elseif canImport(ARKit) && os(macOS)
import ARKit

@Godot
class ARWorldTrackingConfiguration: RefCounted, @unchecked Sendable {
    enum WorldAlignment: Int, CaseIterable {
        case GRAVITY = 0
        case GRAVITY_AND_HEADING = 1
        case CAMERA = 2
    }

    enum EnvironmentTexturing: Int, CaseIterable {
        case NONE = 0
        case MANUAL = 1
        case AUTOMATIC = 2
    }

    enum PlaneDetection: Int, CaseIterable {
        case NONE = 0
        case HORIZONTAL = 1
        case VERTICAL = 2
        case SLANTED = 4
    }

    enum SceneReconstruction: Int, CaseIterable {
        case NONE = 0
        case MESH = 1
        case MESH_WITH_CLASSIFICATION = 3
    }

    @Export(.enum) var worldAlignment: WorldAlignment = .GRAVITY
    @Export(.enum) var environmentTexturing: EnvironmentTexturing = .NONE
    @Export(.enum) var sceneReconstruction: SceneReconstruction = .NONE
    @Export var planeDetectionMask: Int = PlaneDetection.NONE.rawValue
    @Export var isLightEstimationEnabled: Bool = true
    @Export var isAutoFocusEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var automaticImageScaleEstimationEnabled: Bool = false
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isCollaborationEnabled: Bool = false
    @Export var userFaceTrackingEnabled: Bool = false
    @Export var appClipCodeTrackingEnabled: Bool = false
    @Export var handTrackingEnabled: Bool = false
    @Export var detectionImageGroupName: String = ""
    @Export var frameSemanticsMask: Int = FrameSemantics.NONE.rawValue

    enum FrameSemantics: Int, CaseIterable {
        case NONE = 0
        case PERSON_SEGMENTATION = 1
        case PERSON_SEGMENTATION_WITH_DEPTH = 2
        case BODY_DETECTION = 4
    }

    @Callable
    static func is_supported() -> Bool {
        if #available(macOS 26.0, *) {
            return WorldTrackingProvider.isSupported
        }
        return false
    }

    @Callable
    func set_detection_image_group(groupName: String) -> Bool {
        detectionImageGroupName = groupName
        return false
    }

    @Callable
    static func supports_user_face_tracking() -> Bool {
        false
    }

    @Callable
    static func supports_app_clip_code_tracking() -> Bool {
        false
    }

    @Callable
    static func supports_scene_reconstruction(scene_reconstruction: SceneReconstruction) -> Bool {
        false
    }
}
#endif
