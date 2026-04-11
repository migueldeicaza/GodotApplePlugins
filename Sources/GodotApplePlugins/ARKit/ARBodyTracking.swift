//
//  ARBodyTracking.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARBodySkeleton: RefCounted, @unchecked Sendable {
    var skeleton: ARKit.ARSkeleton3D?

    convenience init(skeleton: ARKit.ARSkeleton3D) {
        self.init()
        self.skeleton = skeleton
    }

    @Export var jointCount: Int {
        skeleton?.jointCount ?? 0
    }

    @Callable
    func get_joint_name(index: Int) -> String {
        guard let skeleton, index >= 0, index < skeleton.jointCount else { return "" }
        return ARKit.ARSkeletonDefinition.defaultBody3D.jointNames[index]
    }

    @Callable
    func get_joint_transform(index: Int) -> Transform3D {
        guard let skeleton, index >= 0, index < skeleton.jointCount else { return Transform3D() }
        return godotTransform3D(skeleton.jointModelTransforms[index])
    }

    @Callable
    func get_joint_is_tracked(index: Int) -> Bool {
        guard let skeleton, index >= 0, index < skeleton.jointCount else { return false }
        return skeleton.isJointTracked(index)
    }
}

@Godot
class ARBodyAnchor: RefCounted, @unchecked Sendable {
    var anchor: ARKit.ARBodyAnchor?

    convenience init(anchor: ARKit.ARBodyAnchor) {
        self.init()
        self.anchor = anchor
    }

    @Export var identifier: String {
        anchor?.identifier.uuidString ?? ""
    }

    @Export var transform: Transform3D {
        guard let anchor else { return Transform3D() }
        return godotTransform3D(anchor.transform)
    }

    @Export var isTracked: Bool {
        anchor?.isTracked ?? false
    }

    @Export var estimatedScaleFactor: Double {
        Double(anchor?.estimatedScaleFactor ?? 1.0)
    }

    @Export var skeleton: ARBodySkeleton? {
        guard let nativeSkeleton = anchor?.skeleton else { return nil }
        return ARBodySkeleton(skeleton: nativeSkeleton)
    }
}

@Godot
class ARBodyTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var automaticSkeletonScaleEstimationEnabled: Bool = false

    @Callable
    static func is_supported() -> Bool {
        ARKit.ARBodyTrackingConfiguration.isSupported
    }

    func makeNative() -> ARKit.ARBodyTrackingConfiguration {
        let config = ARKit.ARBodyTrackingConfiguration()
        config.isAutoFocusEnabled = isAutoFocusEnabled
        config.isLightEstimationEnabled = isLightEstimationEnabled
        config.automaticSkeletonScaleEstimationEnabled = automaticSkeletonScaleEstimationEnabled
        return config
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARBodyTrackingConfiguration and ARBodyAnchor are iOS-only APIs. visionOS does
// not expose full-body skeletal tracking through ARKit. These stubs exist for API
// surface consistency only.

@Godot
class ARBodySkeleton: RefCounted, @unchecked Sendable {
    @Export var jointCount: Int = 0

    @Callable func get_joint_name(index: Int) -> String { "" }
    @Callable func get_joint_transform(index: Int) -> Transform3D { Transform3D() }
    @Callable func get_joint_is_tracked(index: Int) -> Bool { false }
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
class ARBodyTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var automaticSkeletonScaleEstimationEnabled: Bool = false

    @Callable static func is_supported() -> Bool { false }
}

#endif
