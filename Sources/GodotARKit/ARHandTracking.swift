//
//  ARHandTracking.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(visionOS)
import ARKit

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

    var skeleton: HandSkeleton?

    convenience init(skeleton: HandSkeleton) {
        self.init()
        self.skeleton = skeleton
    }

    @Export var jointCount: Int {
        skeleton.map { _ in HandSkeleton.JointName.allCases.count } ?? 0
    }

    @Callable
    func get_joint_transform(joint: JointName) -> Transform3D {
        guard let skeleton, let nativeJoint = joint.toNative() else { return Transform3D() }
        let jointTransform = skeleton.joint(nativeJoint)
        return godotTransform3D(jointTransform.anchorFromJointTransform)
    }

    @Callable
    func get_joint_is_tracked(joint: JointName) -> Bool {
        guard let skeleton, let nativeJoint = joint.toNative() else { return false }
        return skeleton.joint(nativeJoint).isTracked
    }

    @Callable
    func get_all_joint_transforms() -> VariantArray {
        guard let skeleton else { return VariantArray() }
        let result = VariantArray()
        for jointName in HandSkeleton.JointName.allCases {
            let joint = skeleton.joint(jointName)
            result.append(Variant(godotTransform3D(joint.anchorFromJointTransform)))
        }
        return result
    }
}

private extension ARHandSkeleton.JointName {
    func toNative() -> HandSkeleton.JointName? {
        switch self {
        case .WRIST: return .wrist
        case .THUMB_KNUCKLE: return .thumbKnuckle
        case .THUMB_INTERMEDIATE_BASE: return .thumbIntermediateBase
        case .THUMB_INTERMEDIATE_TIP: return .thumbIntermediateTip
        case .THUMB_TIP: return .thumbTip
        case .INDEX_FINGER_METACARPAL: return .indexFingerMetacarpal
        case .INDEX_FINGER_KNUCKLE: return .indexFingerKnuckle
        case .INDEX_FINGER_INTERMEDIATE_BASE: return .indexFingerIntermediateBase
        case .INDEX_FINGER_INTERMEDIATE_TIP: return .indexFingerIntermediateTip
        case .INDEX_FINGER_TIP: return .indexFingerTip
        case .MIDDLE_FINGER_METACARPAL: return .middleFingerMetacarpal
        case .MIDDLE_FINGER_KNUCKLE: return .middleFingerKnuckle
        case .MIDDLE_FINGER_INTERMEDIATE_BASE: return .middleFingerIntermediateBase
        case .MIDDLE_FINGER_INTERMEDIATE_TIP: return .middleFingerIntermediateTip
        case .MIDDLE_FINGER_TIP: return .middleFingerTip
        case .RING_FINGER_METACARPAL: return .ringFingerMetacarpal
        case .RING_FINGER_KNUCKLE: return .ringFingerKnuckle
        case .RING_FINGER_INTERMEDIATE_BASE: return .ringFingerIntermediateBase
        case .RING_FINGER_INTERMEDIATE_TIP: return .ringFingerIntermediateTip
        case .RING_FINGER_TIP: return .ringFingerTip
        case .LITTLE_FINGER_METACARPAL: return .littleFingerMetacarpal
        case .LITTLE_FINGER_KNUCKLE: return .littleFingerKnuckle
        case .LITTLE_FINGER_INTERMEDIATE_BASE: return .littleFingerIntermediateBase
        case .LITTLE_FINGER_INTERMEDIATE_TIP: return .littleFingerIntermediateTip
        case .LITTLE_FINGER_TIP: return .littleFingerTip
        case .FOREARM_WRIST: return .forearmWrist
        case .FOREARM_ARM: return .forearmArm
        }
    }
}

@Godot
class ARHandAnchor: RefCounted, @unchecked Sendable {
    enum Chirality: Int, CaseIterable {
        case LEFT = 0
        case RIGHT = 1
    }

    var anchor: HandAnchor?

    convenience init(anchor: HandAnchor) {
        self.init()
        self.anchor = anchor
    }

    @Export var identifier: String {
        anchor?.id.uuidString ?? ""
    }

    @Export var transform: Transform3D {
        guard let anchor else { return Transform3D() }
        return godotTransform3D(anchor.originFromAnchorTransform)
    }

    @Export(.enum) var chirality: Chirality {
        guard let anchor else { return .LEFT }
        switch anchor.chirality {
        case .left: return .LEFT
        case .right: return .RIGHT
        @unknown default: return .LEFT
        }
    }

    @Export var isTracked: Bool {
        anchor?.isTracked ?? false
    }

    @Export var skeleton: ARHandSkeleton? {
        guard let handSkeleton = anchor?.handSkeleton else { return nil }
        return ARHandSkeleton(skeleton: handSkeleton)
    }
}

// Hand tracking is visionOS-only. iOS does not expose HandTrackingProvider or
// HandAnchor through ARKit (hand input on iOS uses Vision framework instead).
// These stubs exist for API surface consistency only.

#elseif canImport(ARKit) && os(iOS)

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

// Hand tracking is visionOS-only. macOS remote sessions do not relay hand data.

#elseif canImport(ARKit) && os(macOS)

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

#endif
