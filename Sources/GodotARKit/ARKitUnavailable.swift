//
//  ARKitUnavailable.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if !(canImport(ARKit) && (os(iOS) || os(visionOS) || os(macOS)))

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

    @Callable static func is_supported() -> Bool { false }
    @Callable func set_detection_image_group(groupName: String) -> Bool { false }
    @Callable static func supports_user_face_tracking() -> Bool { false }
    @Callable static func supports_app_clip_code_tracking() -> Bool { false }
    @Callable static func supports_scene_reconstruction(scene_reconstruction: SceneReconstruction) -> Bool { false }
}

@Godot
class ARLightEstimate: RefCounted, @unchecked Sendable {
    @Export var ambientIntensity: Double = 0
    @Export var ambientColorTemperature: Double = 0
    @Export var primaryLightDirection: Vector3 = Vector3()
    @Export var primaryLightIntensity: Double = 0
    @Export var sphericalHarmonicsCoefficients: PackedFloat32Array = PackedFloat32Array()

    @Callable
    func is_directional() -> Bool { false }
}

@Godot
class ARPointCloud: RefCounted, @unchecked Sendable {
    @Export var count: Int = 0
    @Export var points: PackedVector3Array = PackedVector3Array()
    @Export var identifiers: PackedInt64Array = PackedInt64Array()

    @Callable
    func get_point(index: Int) -> Vector3 { Vector3() }

    @Callable
    func get_identifier(index: Int) -> Int { 0 }
}

@Godot
class ARCamera: RefCounted, @unchecked Sendable {
    enum TrackingState: Int, CaseIterable {
        case NOT_AVAILABLE = 0
        case LIMITED = 1
        case NORMAL = 2
    }

    enum TrackingStateReason: Int, CaseIterable {
        case NONE = 0
        case INITIALIZING = 1
        case EXCESSIVE_MOTION = 2
        case INSUFFICIENT_FEATURES = 3
        case RELOCALIZING = 4
    }

    @Export var transform: Transform3D = Transform3D()
    @Export var eulerAngles: Vector3 = Vector3()
    @Export var intrinsics: PackedFloat32Array = PackedFloat32Array()
    @Export var imageResolution: Vector2 = Vector2()
    @Export var projectionMatrix: PackedFloat32Array = PackedFloat32Array()
    @Export var exposureDuration: Double = 0
    @Export var exposureOffset: Double = 0
    @Export(.enum) var trackingState: TrackingState = .NOT_AVAILABLE
    @Export(.enum) var trackingStateReason: TrackingStateReason = .NONE

    @Callable
    func projection_matrix_for_orientation(orientation: Int, viewportSize: Vector2, zNear: Double, zFar: Double) -> PackedFloat32Array { PackedFloat32Array() }

    @Callable
    func project_point(point: Vector3, orientation: Int, viewportSize: Vector2) -> Vector2 { Vector2() }

    @Callable
    func unproject_point(point: Vector2, ontoPlaneWithTransform: Transform3D, orientation: Int, viewportSize: Vector2) -> Vector3 { Vector3() }

    @Callable
    func view_matrix_for_orientation(orientation: Int) -> Transform3D { Transform3D() }
}

@Godot
class ARAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var name: String = ""
    @Export var sessionIdentifier: String = ""
    @Export var transform: Transform3D = Transform3D()

    @Callable
    static func create(transform: Transform3D) -> ARAnchor {
        let anchor = ARAnchor()
        anchor.transform = transform
        return anchor
    }

    @Callable
    static func create_named(name: String, transform: Transform3D) -> ARAnchor {
        let anchor = ARAnchor()
        anchor.name = name
        anchor.transform = transform
        return anchor
    }

    @Callable
    func copy_anchor() -> ARAnchor? { nil }
}

@Godot
class ARPlaneAnchor: RefCounted, @unchecked Sendable {
    enum Alignment: Int, CaseIterable {
        case HORIZONTAL = 0
        case VERTICAL = 1
        case SLANTED = 2
    }

    enum ClassificationStatus: Int, CaseIterable {
        case NOT_AVAILABLE = 0
        case UNDETERMINED = 1
        case UNKNOWN = 2
        case KNOWN = 3
    }

    enum Classification: Int, CaseIterable {
        case NONE = 0
        case WALL = 1
        case FLOOR = 2
        case CEILING = 3
        case TABLE = 4
        case SEAT = 5
        case WINDOW = 6
        case DOOR = 7
        case STAIRS = 8
        case BED = 9
        case CABINET = 10
        case HOME_APPLIANCE = 11
        case TV = 12
        case PLANT = 13
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var center: Vector3 = Vector3()
    @Export var extent: Vector3 = Vector3()
    @Export var planeExtentWidth: Double = 0
    @Export var planeExtentHeight: Double = 0
    @Export var planeExtentRotationOnYAxis: Double = 0
    @Export(.enum) var alignment: Alignment = .HORIZONTAL
    @Export(.enum) var classificationStatus: ClassificationStatus = .NOT_AVAILABLE
    @Export(.enum) var classification: Classification = .NONE
    @Export var boundaryVertices: PackedVector3Array = PackedVector3Array()
    @Export var geometryVertices: PackedVector3Array = PackedVector3Array()
    @Export var textureCoordinates: PackedVector2Array = PackedVector2Array()
    @Export var triangleIndices: PackedInt32Array = PackedInt32Array()

    @Callable static func is_classification_supported() -> Bool { false }
    @Callable func get_boundary_vertex(index: Int) -> Vector3 { Vector3() }
    @Callable func get_geometry_vertex(index: Int) -> Vector3 { Vector3() }
    @Callable func get_texture_coordinate(index: Int) -> Vector2 { Vector2() }
    @Callable func get_triangle_index(index: Int) -> Int { 0 }
}

@Godot
class ARFrame: RefCounted, @unchecked Sendable {
    enum WorldMappingStatus: Int, CaseIterable {
        case NOT_AVAILABLE = 0
        case LIMITED = 1
        case EXTENDING = 2
        case MAPPED = 3
    }

    @Export var timestamp: Double = 0
    @Export var camera: ARCamera = ARCamera()
    @Export var lightEstimate: ARLightEstimate?
    @Export var anchors: VariantArray = VariantArray()
    @Export var rawFeaturePoints: ARPointCloud?
    @Export(.enum) var worldMappingStatus: WorldMappingStatus = .NOT_AVAILABLE
    @Export var capturedImageSize: Vector2 = Vector2()

    @Callable
    func display_transform(orientation: Int, viewportSize: Vector2) -> PackedFloat32Array { PackedFloat32Array() }

    @Callable
    func get_anchor(index: Int) -> RefCounted? { nil }

    @Callable
    func get_captured_image_plane_count() -> Int { 0 }

    @Callable
    func get_captured_image_plane_size(plane: Int) -> Vector2 { Vector2() }

    @Callable
    func get_captured_image_plane_stride(plane: Int) -> Int { 0 }

    @Callable
    func get_captured_image_plane_data(plane: Int) -> PackedByteArray { PackedByteArray() }

    @Export var segmentationBufferSize: Vector2 = Vector2()
    @Callable func get_segmentation_buffer() -> PackedByteArray { PackedByteArray() }

    @Export var estimatedDepthDataSize: Vector2 = Vector2()
    @Callable func get_estimated_depth_data() -> PackedByteArray { PackedByteArray() }
}

@Godot
class ARSession: RefCounted, @unchecked Sendable {
    enum RunOption: Int, CaseIterable {
        case RESET_TRACKING = 1
        case REMOVE_EXISTING_ANCHORS = 2
        case STOP_TRACKED_RAYCASTS = 4
        case RESET_SCENE_RECONSTRUCTION = 8
    }

    @Signal("frame") var frame_updated: SignalWithArguments<ARFrame>
    @Signal("anchors") var anchors_added: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_updated: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_removed: SignalWithArguments<VariantArray>
    @Signal("camera") var camera_tracking_changed: SignalWithArguments<ARCamera>
    @Signal("message") var session_failed: SignalWithArguments<String>
    @Signal var session_interrupted: SimpleSignal
    @Signal var interruption_ended: SimpleSignal
    @Signal("hand") var hand_anchor_updated: SignalWithArguments<ARHandAnchor>
    @Signal("mesh") var mesh_anchor_added: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_updated: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_removed: SignalWithArguments<ARMeshAnchor>
    @Signal("collaborationData") var collaboration_data_received: SignalWithArguments<ARCollaborationData>

    @Export var identifier: String = ""
    @Export var currentFrame: ARFrame?
    @Export var anchors: VariantArray = VariantArray()
    @Export var configuration: ARWorldTrackingConfiguration?

    @Callable
    static func is_supported() -> Bool { false }

    @Callable
    func run(configuration: ARWorldTrackingConfiguration, options: Int = 0) {
        self.configuration = configuration
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func run_body_tracking(configuration: ARBodyTrackingConfiguration, options: Int = 0) {
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func run_geo_tracking(configuration: ARGeoTrackingConfiguration, options: Int = 0) {
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func run_face_tracking(configuration: ARFaceTrackingConfiguration, options: Int = 0) {
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func run_image_tracking(configuration: ARImageTrackingConfiguration, options: Int = 0) {
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func pause() {}

    @Callable
    func add_anchor(anchor: ARAnchor) {}

    @Callable
    func remove_anchor(anchor: ARAnchor) {}

    @Callable
    func remove_all_anchors() {}

    @Callable
    func set_world_origin(relativeTransform: Transform3D) {}

    @Callable
    func capture_high_resolution_frame(callback: Callable) {
        _ = callback.call(nil, Variant("ARKit is not available on this platform."))
    }

    @Callable
    func raycast(query: ARRaycastQuery) -> VariantArray {
        VariantArray()
    }

    @Callable
    func tracked_raycast(query: ARRaycastQuery, callback: Callable) -> ARTrackedRaycast? {
        nil
    }

    @Callable
    func get_current_world_map(callback: Callable) {
        _ = callback.call(nil, Variant("ARKit is not available on this platform."))
    }

    @Callable
    func run_with_world_map(configuration: ARWorldTrackingConfiguration, worldMap: ARWorldMap) {
        session_failed.emit("ARKit is not available on this platform.")
    }

    @Callable
    func update_with_collaboration_data(data: ARCollaborationData) {}
}

@Godot
class ARRaycastQuery: RefCounted, @unchecked Sendable {
    enum Target: Int, CaseIterable {
        case EXISTING_PLANE_GEOMETRY = 1
        case EXISTING_PLANE_INFINITE = 2
        case ESTIMATED_PLANE = 3
    }

    enum TargetAlignment: Int, CaseIterable {
        case HORIZONTAL = 0
        case VERTICAL = 1
        case ANY = 2
    }

    @Export var direction: Vector3 = Vector3()
    @Export var origin: Vector3 = Vector3()
    @Export(.enum) var target: Target = .EXISTING_PLANE_GEOMETRY
    @Export(.enum) var targetAlignment: TargetAlignment = .ANY

    @Callable
    static func create(origin: Vector3, direction: Vector3, target: Target, targetAlignment: TargetAlignment) -> ARRaycastQuery {
        let query = ARRaycastQuery()
        query.origin = origin
        query.direction = direction
        query.target = target
        query.targetAlignment = targetAlignment
        return query
    }
}

@Godot
class ARRaycastResult: RefCounted, @unchecked Sendable {
    enum Target: Int, CaseIterable {
        case EXISTING_PLANE_GEOMETRY = 1
        case EXISTING_PLANE_INFINITE = 2
        case ESTIMATED_PLANE = 3
    }

    enum TargetAlignment: Int, CaseIterable {
        case HORIZONTAL = 0
        case VERTICAL = 1
        case ANY = 2
    }

    @Export var worldTransform: Transform3D = Transform3D()
    @Export(.enum) var target: Target = .EXISTING_PLANE_GEOMETRY
    @Export(.enum) var targetAlignment: TargetAlignment = .ANY
    @Export var anchor: ARAnchor?
}

@Godot
class ARTrackedRaycast: RefCounted, @unchecked Sendable {
    @Export var isTracking: Bool = false

    @Callable
    func stop_tracking() {}
}

@Godot
class ARImageAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var referenceImageName: String = ""
    @Export var referenceImagePhysicalSize: Vector2 = Vector2()
    @Export var isTracked: Bool = false
    @Export var estimatedScaleFactor: Double = 1.0
}

@Godot
class ARMeshAnchor: RefCounted, @unchecked Sendable {
    enum MeshClassification: Int, CaseIterable {
        case NONE = 0
        case WALL = 1
        case FLOOR = 2
        case CEILING = 3
        case TABLE = 4
        case SEAT = 5
        case WINDOW = 6
        case DOOR = 7
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var geometryVertices: PackedVector3Array = PackedVector3Array()
    @Export var geometryNormals: PackedVector3Array = PackedVector3Array()
    @Export var triangleIndices: PackedInt32Array = PackedInt32Array()
    @Export var classificationPerFace: PackedInt32Array = PackedInt32Array()

    @Callable func get_vertex(index: Int) -> Vector3 { Vector3() }
    @Callable func get_normal(index: Int) -> Vector3 { Vector3() }
    @Callable func get_face_classification(faceIndex: Int) -> Int { 0 }
}

@Godot
class ARFaceAnchor: RefCounted, @unchecked Sendable {
    enum BlendShapeLocation: Int, CaseIterable {
        case EYE_BLINK_LEFT = 0
        case EYE_LOOK_DOWN_LEFT = 1
        case EYE_LOOK_IN_LEFT = 2
        case EYE_LOOK_OUT_LEFT = 3
        case EYE_LOOK_UP_LEFT = 4
        case EYE_SQUINT_LEFT = 5
        case EYE_WIDE_LEFT = 6
        case EYE_BLINK_RIGHT = 7
        case EYE_LOOK_DOWN_RIGHT = 8
        case EYE_LOOK_IN_RIGHT = 9
        case EYE_LOOK_OUT_RIGHT = 10
        case EYE_LOOK_UP_RIGHT = 11
        case EYE_SQUINT_RIGHT = 12
        case EYE_WIDE_RIGHT = 13
        case JAW_FORWARD = 14
        case JAW_LEFT = 15
        case JAW_RIGHT = 16
        case JAW_OPEN = 17
        case MOUTH_CLOSE = 18
        case MOUTH_FUNNEL = 19
        case MOUTH_PUCKER = 20
        case MOUTH_LEFT = 21
        case MOUTH_RIGHT = 22
        case MOUTH_SMILE_LEFT = 23
        case MOUTH_SMILE_RIGHT = 24
        case MOUTH_FROWN_LEFT = 25
        case MOUTH_FROWN_RIGHT = 26
        case MOUTH_DIMPLE_LEFT = 27
        case MOUTH_DIMPLE_RIGHT = 28
        case MOUTH_STRETCH_LEFT = 29
        case MOUTH_STRETCH_RIGHT = 30
        case MOUTH_ROLL_LOWER = 31
        case MOUTH_ROLL_UPPER = 32
        case MOUTH_SHRUG_LOWER = 33
        case MOUTH_SHRUG_UPPER = 34
        case MOUTH_PRESS_LEFT = 35
        case MOUTH_PRESS_RIGHT = 36
        case MOUTH_LOWER_DOWN_LEFT = 37
        case MOUTH_LOWER_DOWN_RIGHT = 38
        case MOUTH_UPPER_UP_LEFT = 39
        case MOUTH_UPPER_UP_RIGHT = 40
        case BROW_DOWN_LEFT = 41
        case BROW_DOWN_RIGHT = 42
        case BROW_INNER_UP = 43
        case BROW_OUTER_UP_LEFT = 44
        case BROW_OUTER_UP_RIGHT = 45
        case CHEEK_PUFF = 46
        case CHEEK_SQUINT_LEFT = 47
        case CHEEK_SQUINT_RIGHT = 48
        case NOSE_SNEER_LEFT = 49
        case NOSE_SNEER_RIGHT = 50
        case TONGUE_OUT = 51
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var isTracked: Bool = false
    @Export var geometryVertices: PackedVector3Array = PackedVector3Array()
    @Export var geometryTextureCoordinates: PackedVector2Array = PackedVector2Array()
    @Export var triangleIndices: PackedInt32Array = PackedInt32Array()
    @Export var leftEyeTransform: Transform3D = Transform3D()
    @Export var rightEyeTransform: Transform3D = Transform3D()
    @Export var lookAtPoint: Vector3 = Vector3()

    @Callable func get_blend_shape_value(location: BlendShapeLocation) -> Double { 0 }
}

@Godot
class ARWorldMap: RefCounted, @unchecked Sendable {
    @Export var center: Vector3 = Vector3()
    @Export var extent: Vector3 = Vector3()
    @Export var anchors: VariantArray = VariantArray()
    @Export var rawFeaturePoints: ARPointCloud?

    @Callable func serialize() -> PackedByteArray { PackedByteArray() }
    @Callable static func deserialize(data: PackedByteArray) -> ARWorldMap? { nil }
}

@Godot
class ARBodyTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var automaticSkeletonScaleEstimationEnabled: Bool = false

    @Callable static func is_supported() -> Bool { false }
}

@Godot
class ARBodyAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var isTracked: Bool = false
    @Export var estimatedScaleFactor: Double = 1.0
    @Export var skeleton: ARBodySkeleton?
}

@Godot
class ARBodySkeleton: RefCounted, @unchecked Sendable {
    @Export var jointCount: Int = 0

    @Callable func get_joint_name(index: Int) -> String { "" }
    @Callable func get_joint_transform(index: Int) -> Transform3D { Transform3D() }
    @Callable func get_joint_is_tracked(index: Int) -> Bool { false }
}

@Godot
class ARHandAnchor: RefCounted, @unchecked Sendable {
    enum Chirality: Int, CaseIterable {
        case LEFT = 0
        case RIGHT = 1
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export(.enum) var chirality: Chirality = .LEFT
    @Export var isTracked: Bool = false
    @Export var skeleton: ARHandSkeleton?
}

@Godot
class ARHandSkeleton: RefCounted, @unchecked Sendable {
    enum JointName: Int, CaseIterable {
        case WRIST = 0
        case THUMB_KNUCKLE = 1
        case THUMB_INTERMEDIATE_BASE = 2
        case THUMB_INTERMEDIATE_TIP = 3
        case THUMB_TIP = 4
        case INDEX_FINGER_METACARPAL = 5
        case INDEX_FINGER_KNUCKLE = 6
        case INDEX_FINGER_INTERMEDIATE_BASE = 7
        case INDEX_FINGER_INTERMEDIATE_TIP = 8
        case INDEX_FINGER_TIP = 9
        case MIDDLE_FINGER_METACARPAL = 10
        case MIDDLE_FINGER_KNUCKLE = 11
        case MIDDLE_FINGER_INTERMEDIATE_BASE = 12
        case MIDDLE_FINGER_INTERMEDIATE_TIP = 13
        case MIDDLE_FINGER_TIP = 14
        case RING_FINGER_METACARPAL = 15
        case RING_FINGER_KNUCKLE = 16
        case RING_FINGER_INTERMEDIATE_BASE = 17
        case RING_FINGER_INTERMEDIATE_TIP = 18
        case RING_FINGER_TIP = 19
        case LITTLE_FINGER_METACARPAL = 20
        case LITTLE_FINGER_KNUCKLE = 21
        case LITTLE_FINGER_INTERMEDIATE_BASE = 22
        case LITTLE_FINGER_INTERMEDIATE_TIP = 23
        case LITTLE_FINGER_TIP = 24
        case FOREARM_WRIST = 25
        case FOREARM_ARM = 26
    }

    @Export var jointCount: Int = 0

    @Callable func get_joint_transform(joint: JointName) -> Transform3D { Transform3D() }
    @Callable func get_joint_is_tracked(joint: JointName) -> Bool { false }
    @Callable func get_all_joint_transforms() -> VariantArray { VariantArray() }
}

@Godot
class ARCoachingOverlay: RefCounted, @unchecked Sendable {
    enum Goal: Int, CaseIterable {
        case TRACKING = 0
        case HORIZONTAL_PLANE = 1
        case VERTICAL_PLANE = 2
        case ANY_PLANE = 3
        case GEO_TRACKING = 4
    }

    @Export(.enum) var goal: Goal = .TRACKING
    @Export var activatesAutomatically: Bool = true

    @Signal var coaching_did_deactivate: SimpleSignal

    @Callable func set_active(active: Bool, animated: Bool) {}
    @Callable func attach_to_session(session: ARSession) {}
}

@Godot
class ARFaceTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedFaces: Int = 1
    @Export var isLightEstimationEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var isWorldTrackingEnabled: Bool = false

    @Callable static func is_supported() -> Bool { false }
    @Callable static func supports_world_tracking() -> Bool { false }
}

@Godot
class ARImageTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var detectionImageGroupName: String = ""

    @Callable static func is_supported() -> Bool { false }
    @Callable func set_detection_image_group(groupName: String) -> Bool { false }
}

@Godot
class AREnvironmentProbeAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var extent: Vector3 = Vector3()
    @Export var hasEnvironmentTexture: Bool = false
    @Export var textureWidth: Int = 0
    @Export var textureHeight: Int = 0
}

@Godot
class ARGeoTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var environmentTexturing: ARWorldTrackingConfiguration.EnvironmentTexturing = .NONE
    @Export var planeDetectionMask: Int = 0
    @Export var isLightEstimationEnabled: Bool = true
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var detectionImageGroupName: String = ""

    @Callable static func is_supported() -> Bool { false }
    @Callable static func check_availability(callback: Callable) {
        _ = callback.call(Variant(false), Variant("ARKit is not available on this platform."))
    }
    @Callable func set_detection_image_group(groupName: String) -> Bool { false }
}

@Godot
class ARGeoAnchor: RefCounted, @unchecked Sendable {
    enum AltitudeSource: Int, CaseIterable {
        case UNKNOWN = 0
        case COARSE = 1
        case PRECISE = 2
        case USER_DEFINED = 3
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var coordinate: Vector2 = Vector2()
    @Export var altitude: Double = 0
    @Export(.enum) var altitudeSource: AltitudeSource = .UNKNOWN

    @Callable
    static func create(latitude: Double, longitude: Double, altitude: Double) -> ARGeoAnchor {
        let anchor = ARGeoAnchor()
        anchor.coordinate = Vector2(x: Float(latitude), y: Float(longitude))
        anchor.altitude = altitude
        return anchor
    }
}

@Godot
class ARCollaborationData: RefCounted, @unchecked Sendable {
    enum Priority: Int, CaseIterable {
        case CRITICAL = 0
        case OPTIONAL = 1
    }

    @Export(.enum) var priority: Priority = .OPTIONAL
    @Export var data: PackedByteArray = PackedByteArray()

    @Callable func serialize() -> PackedByteArray { PackedByteArray() }
    @Callable static func deserialize(data: PackedByteArray) -> ARCollaborationData? { nil }
}

#endif
