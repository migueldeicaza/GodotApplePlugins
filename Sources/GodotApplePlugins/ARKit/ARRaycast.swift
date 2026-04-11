//
//  ARRaycast.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

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

    var query: ARKit.ARRaycastQuery?

    convenience init(query: ARKit.ARRaycastQuery) {
        self.init()
        self.query = query
    }

    @Export var direction: Vector3 {
        guard let query else { return Vector3() }
        return godotVector3(query.direction)
    }

    @Export var origin: Vector3 {
        guard let query else { return Vector3() }
        return godotVector3(query.origin)
    }

    @Export(.enum) var target: Target {
        guard let query else { return .EXISTING_PLANE_GEOMETRY }
        switch query.target {
        case .existingPlaneGeometry:
            return .EXISTING_PLANE_GEOMETRY
        case .existingPlaneInfinite:
            return .EXISTING_PLANE_INFINITE
        case .estimatedPlane:
            return .ESTIMATED_PLANE
        @unknown default:
            return .EXISTING_PLANE_GEOMETRY
        }
    }

    @Export(.enum) var targetAlignment: TargetAlignment {
        guard let query else { return .ANY }
        switch query.targetAlignment {
        case .horizontal:
            return .HORIZONTAL
        case .vertical:
            return .VERTICAL
        case .any:
            return .ANY
        @unknown default:
            return .ANY
        }
    }

    @Callable
    static func create(origin: Vector3, direction: Vector3, target: Target, targetAlignment: TargetAlignment) -> ARRaycastQuery {
        let nativeTarget: ARKit.ARRaycastQuery.Target = switch target {
        case .EXISTING_PLANE_GEOMETRY: .existingPlaneGeometry
        case .EXISTING_PLANE_INFINITE: .existingPlaneInfinite
        case .ESTIMATED_PLANE: .estimatedPlane
        }
        let nativeAlignment: ARKit.ARRaycastQuery.TargetAlignment = switch targetAlignment {
        case .HORIZONTAL: .horizontal
        case .VERTICAL: .vertical
        case .ANY: .any
        }
        let native = ARKit.ARRaycastQuery(
            origin: simdVector3(origin),
            direction: simdVector3(direction),
            allowing: nativeTarget,
            alignment: nativeAlignment
        )
        return ARRaycastQuery(query: native)
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

    var result: ARKit.ARRaycastResult?

    convenience init(result: ARKit.ARRaycastResult) {
        self.init()
        self.result = result
    }

    @Export var worldTransform: Transform3D {
        guard let result else { return Transform3D() }
        return godotTransform3D(result.worldTransform)
    }

    @Export(.enum) var target: Target {
        guard let result else { return .EXISTING_PLANE_GEOMETRY }
        switch result.target {
        case .existingPlaneGeometry:
            return .EXISTING_PLANE_GEOMETRY
        case .existingPlaneInfinite:
            return .EXISTING_PLANE_INFINITE
        case .estimatedPlane:
            return .ESTIMATED_PLANE
        @unknown default:
            return .EXISTING_PLANE_GEOMETRY
        }
    }

    @Export(.enum) var targetAlignment: TargetAlignment {
        guard let result else { return .ANY }
        switch result.targetAlignment {
        case .horizontal:
            return .HORIZONTAL
        case .vertical:
            return .VERTICAL
        case .any:
            return .ANY
        @unknown default:
            return .ANY
        }
    }

    @Export var anchor: ARAnchor? {
        guard let nativeAnchor = result?.anchor else { return nil }
        return ARAnchor(anchor: nativeAnchor)
    }
}

@Godot
class ARTrackedRaycast: RefCounted, @unchecked Sendable {
    var trackedRaycast: ARKit.ARTrackedRaycast?

    @Export var isTracking: Bool {
        trackedRaycast != nil
    }

    @Callable
    func stop_tracking() {
        trackedRaycast?.stopTracking()
        trackedRaycast = nil
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// Raycasting is not available on visionOS or macOS. The visionOS ARKit framework
// does not include ARRaycastQuery/ARRaycastResult; spatial hit-testing on visionOS
// is done through RealityKit (CollisionComponent, scene queries) rather than via
// the ARKit session. These types exist as data shells so the API surface is
// consistent across platforms, but ARSession.raycast will always return empty and
// ARSession.tracked_raycast will always return nil.

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

#endif
