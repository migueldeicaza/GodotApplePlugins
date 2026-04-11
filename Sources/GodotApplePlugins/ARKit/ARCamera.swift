//
//  ARCamera.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

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

    var camera: ARKit.ARCamera?

    convenience init(camera: ARKit.ARCamera) {
        self.init()
        self.camera = camera
    }

    @Export var transform: Transform3D {
        guard let camera else { return Transform3D() }
        return godotTransform3D(camera.transform)
    }

    @Export var eulerAngles: Vector3 {
        guard let camera else { return Vector3() }
        return godotVector3(camera.eulerAngles)
    }

    @Export var intrinsics: PackedFloat32Array {
        guard let camera else { return PackedFloat32Array() }
        return packedFloat32Array(camera.intrinsics)
    }

    @Export var imageResolution: Vector2 {
        guard let camera else { return Vector2() }
        return godotVector2(camera.imageResolution)
    }

    @Export var projectionMatrix: PackedFloat32Array {
        guard let camera else { return PackedFloat32Array() }
        return packedFloat32Array(camera.projectionMatrix)
    }

    @Export var exposureDuration: Double {
        if #available(iOS 13.0, *) {
            return camera?.exposureDuration ?? 0
        }
        return 0
    }

    @Export var exposureOffset: Double {
        if #available(iOS 13.0, *) {
            return Double(camera?.exposureOffset ?? 0)
        }
        return 0
    }

    @Export(.enum) var trackingState: TrackingState {
        Self.mapTrackingState(camera?.trackingState ?? .notAvailable)
    }

    @Export(.enum) var trackingStateReason: TrackingStateReason {
        Self.mapTrackingStateReason(camera?.trackingState ?? .notAvailable)
    }

    @Callable
    func projection_matrix_for_orientation(
        orientation: Int, viewportSize: Vector2, zNear: Double, zFar: Double
    ) -> PackedFloat32Array {
        guard let camera else { return PackedFloat32Array() }
        let matrix = camera.projectionMatrix(
            for: uiInterfaceOrientation(orientation),
            viewportSize: cgSize(viewportSize),
            zNear: CGFloat(zNear),
            zFar: CGFloat(zFar)
        )
        return packedFloat32Array(matrix)
    }

    @Callable
    func project_point(point: Vector3, orientation: Int, viewportSize: Vector2) -> Vector2 {
        guard let camera else { return Vector2() }
        let projected = camera.projectPoint(
            simdVector3(point),
            orientation: uiInterfaceOrientation(orientation),
            viewportSize: cgSize(viewportSize)
        )
        return godotVector2(projected)
    }

    @Callable
    func unproject_point(
        point: Vector2, ontoPlaneWithTransform: Transform3D, orientation: Int, viewportSize: Vector2
    ) -> Vector3 {
        guard let camera else { return Vector3() }
        let unprojected = camera.unprojectPoint(
            cgPoint(point),
            ontoPlaneWithTransform: simdTransform(ontoPlaneWithTransform),
            orientation: uiInterfaceOrientation(orientation),
            viewportSize: cgSize(viewportSize)
        )
        return godotVector3(unprojected)
    }

    @Callable
    func view_matrix_for_orientation(orientation: Int) -> Transform3D {
        guard let camera else { return Transform3D() }
        return godotTransform3D(camera.viewMatrix(for: uiInterfaceOrientation(orientation)))
    }

    private static func mapTrackingState(_ state: ARKit.ARCamera.TrackingState) -> TrackingState {
        switch state {
        case .notAvailable:
            return .NOT_AVAILABLE
        case .limited:
            return .LIMITED
        case .normal:
            return .NORMAL
        @unknown default:
            return .NOT_AVAILABLE
        }
    }

    static func mapTrackingStateReason(_ state: ARKit.ARCamera.TrackingState) -> TrackingStateReason {
        switch state {
        case .notAvailable:
            return .NONE
        case .normal:
            return .NONE
        case let .limited(reason):
            switch reason {
            case .none:
                return .NONE
            case .initializing:
                return .INITIALIZING
            case .excessiveMotion:
                return .EXCESSIVE_MOTION
            case .insufficientFeatures:
                return .INSUFFICIENT_FEATURES
            case .relocalizing:
                return .RELOCALIZING
            @unknown default:
                return .NONE
            }
        @unknown default:
            return .NONE
        }
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))
import ARKit

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

    private var cameraTransform: Transform3D = Transform3D()
    private var cameraEulerAngles: Vector3 = Vector3()
    private var cameraTrackingState: TrackingState = .NOT_AVAILABLE
    private var cameraTrackingStateReason: TrackingStateReason = .NONE
    private var cameraTimestamp: Double = 0

    @available(macOS 26.0, *)
    convenience init(deviceAnchor: DeviceAnchor) {
        self.init()
        apply(deviceAnchor: deviceAnchor)
    }

    @available(macOS 26.0, *)
    func apply(deviceAnchor: DeviceAnchor) {
        cameraTransform = godotTransform3D(deviceAnchor.originFromAnchorTransform)
        cameraEulerAngles = godotEulerAngles(deviceAnchor.originFromAnchorTransform)
        #if os(macOS)
        switch deviceAnchor.trackingState {
        case .tracked:
            cameraTrackingState = .NORMAL
            cameraTrackingStateReason = .NONE
        case .orientationTracked:
            cameraTrackingState = .LIMITED
            cameraTrackingStateReason = .INITIALIZING
        case .untracked:
            cameraTrackingState = .NOT_AVAILABLE
            cameraTrackingStateReason = .INITIALIZING
        @unknown default:
            cameraTrackingState = .NOT_AVAILABLE
            cameraTrackingStateReason = .NONE
        }
        #else
        cameraTrackingState = deviceAnchor.isTracked ? .NORMAL : .LIMITED
        cameraTrackingStateReason = deviceAnchor.isTracked ? .NONE : .INITIALIZING
        #endif

        #if os(macOS)
        cameraTimestamp = deviceAnchor.timestamp
        #else
        if #available(visionOS 2.0, *) {
            cameraTimestamp = deviceAnchor.timestamp
        }
        #endif
    }

    @Export var transform: Transform3D {
        cameraTransform
    }

    @Export var eulerAngles: Vector3 {
        cameraEulerAngles
    }

    // Not available on visionOS/macOS: the system compositor owns the camera
    // pipeline, so Apple does not expose lens intrinsics to applications.
    @Export var intrinsics: PackedFloat32Array {
        PackedFloat32Array()
    }

    // Not available on visionOS/macOS: no direct camera access.
    @Export var imageResolution: Vector2 {
        Vector2()
    }

    // Not available on visionOS/macOS: projection is handled by the system
    // compositor; Apple does not expose the per-eye projection matrices.
    @Export var projectionMatrix: PackedFloat32Array {
        PackedFloat32Array()
    }

    // Not available on visionOS/macOS: exposure control is an iOS camera feature.
    @Export var exposureDuration: Double {
        0
    }

    // Not available on visionOS/macOS: exposure control is an iOS camera feature.
    @Export var exposureOffset: Double {
        0
    }

    @Export(.enum) var trackingState: TrackingState {
        cameraTrackingState
    }

    @Export(.enum) var trackingStateReason: TrackingStateReason {
        cameraTrackingStateReason
    }

    @Export var timestamp: Double {
        cameraTimestamp
    }

    // Not available on visionOS/macOS: these projection utilities require camera
    // intrinsics which Apple does not expose outside the iOS camera pipeline.
    @Callable
    func projection_matrix_for_orientation(
        orientation: Int, viewportSize: Vector2, zNear: Double, zFar: Double
    ) -> PackedFloat32Array {
        PackedFloat32Array()
    }

    @Callable
    func project_point(point: Vector3, orientation: Int, viewportSize: Vector2) -> Vector2 {
        Vector2()
    }

    @Callable
    func unproject_point(
        point: Vector2, ontoPlaneWithTransform: Transform3D, orientation: Int, viewportSize: Vector2
    ) -> Vector3 {
        Vector3()
    }

    @Callable
    func view_matrix_for_orientation(orientation: Int) -> Transform3D {
        godotTransform3D(simdTransform(transform).inverse)
    }
}

#endif
