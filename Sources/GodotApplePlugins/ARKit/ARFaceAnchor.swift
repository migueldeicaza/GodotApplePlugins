//
//  ARFaceAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

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

    var anchor: ARKit.ARFaceAnchor?

    convenience init(anchor: ARKit.ARFaceAnchor) {
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

    @Export var geometryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        let vertices = UnsafeBufferPointer(
            start: geometry.vertices,
            count: geometry.vertexCount
        )
        return packedVector3Array(vertices)
    }

    @Export var geometryTextureCoordinates: PackedVector2Array {
        guard let geometry = anchor?.geometry else { return PackedVector2Array() }
        let coordinates = UnsafeBufferPointer(
            start: geometry.textureCoordinates,
            count: geometry.textureCoordinateCount
        )
        return packedVector2Array(coordinates)
    }

    @Export var triangleIndices: PackedInt32Array {
        guard let geometry = anchor?.geometry else { return PackedInt32Array() }
        let indices = UnsafeBufferPointer(
            start: geometry.triangleIndices,
            count: geometry.triangleCount * 3
        )
        return PackedInt32Array(indices.map(Int32.init))
    }

    @Export var leftEyeTransform: Transform3D {
        guard let anchor else { return Transform3D() }
        return godotTransform3D(anchor.leftEyeTransform)
    }

    @Export var rightEyeTransform: Transform3D {
        guard let anchor else { return Transform3D() }
        return godotTransform3D(anchor.rightEyeTransform)
    }

    @Export var lookAtPoint: Vector3 {
        guard let anchor else { return Vector3() }
        return godotVector3(anchor.lookAtPoint)
    }

    @Callable
    func get_blend_shape_value(location: BlendShapeLocation) -> Double {
        guard let blendShapes = anchor?.blendShapes,
              let nativeLocation = location.toNative(),
              let value = blendShapes[nativeLocation] as? NSNumber
        else { return 0 }
        return value.doubleValue
    }
}

private extension ARFaceAnchor.BlendShapeLocation {
    func toNative() -> ARKit.ARFaceAnchor.BlendShapeLocation? {
        switch self {
        case .EYE_BLINK_LEFT: return .eyeBlinkLeft
        case .EYE_LOOK_DOWN_LEFT: return .eyeLookDownLeft
        case .EYE_LOOK_IN_LEFT: return .eyeLookInLeft
        case .EYE_LOOK_OUT_LEFT: return .eyeLookOutLeft
        case .EYE_LOOK_UP_LEFT: return .eyeLookUpLeft
        case .EYE_SQUINT_LEFT: return .eyeSquintLeft
        case .EYE_WIDE_LEFT: return .eyeWideLeft
        case .EYE_BLINK_RIGHT: return .eyeBlinkRight
        case .EYE_LOOK_DOWN_RIGHT: return .eyeLookDownRight
        case .EYE_LOOK_IN_RIGHT: return .eyeLookInRight
        case .EYE_LOOK_OUT_RIGHT: return .eyeLookOutRight
        case .EYE_LOOK_UP_RIGHT: return .eyeLookUpRight
        case .EYE_SQUINT_RIGHT: return .eyeSquintRight
        case .EYE_WIDE_RIGHT: return .eyeWideRight
        case .JAW_FORWARD: return .jawForward
        case .JAW_LEFT: return .jawLeft
        case .JAW_RIGHT: return .jawRight
        case .JAW_OPEN: return .jawOpen
        case .MOUTH_CLOSE: return .mouthClose
        case .MOUTH_FUNNEL: return .mouthFunnel
        case .MOUTH_PUCKER: return .mouthPucker
        case .MOUTH_LEFT: return .mouthLeft
        case .MOUTH_RIGHT: return .mouthRight
        case .MOUTH_SMILE_LEFT: return .mouthSmileLeft
        case .MOUTH_SMILE_RIGHT: return .mouthSmileRight
        case .MOUTH_FROWN_LEFT: return .mouthFrownLeft
        case .MOUTH_FROWN_RIGHT: return .mouthFrownRight
        case .MOUTH_DIMPLE_LEFT: return .mouthDimpleLeft
        case .MOUTH_DIMPLE_RIGHT: return .mouthDimpleRight
        case .MOUTH_STRETCH_LEFT: return .mouthStretchLeft
        case .MOUTH_STRETCH_RIGHT: return .mouthStretchRight
        case .MOUTH_ROLL_LOWER: return .mouthRollLower
        case .MOUTH_ROLL_UPPER: return .mouthRollUpper
        case .MOUTH_SHRUG_LOWER: return .mouthShrugLower
        case .MOUTH_SHRUG_UPPER: return .mouthShrugUpper
        case .MOUTH_PRESS_LEFT: return .mouthPressLeft
        case .MOUTH_PRESS_RIGHT: return .mouthPressRight
        case .MOUTH_LOWER_DOWN_LEFT: return .mouthLowerDownLeft
        case .MOUTH_LOWER_DOWN_RIGHT: return .mouthLowerDownRight
        case .MOUTH_UPPER_UP_LEFT: return .mouthUpperUpLeft
        case .MOUTH_UPPER_UP_RIGHT: return .mouthUpperUpRight
        case .BROW_DOWN_LEFT: return .browDownLeft
        case .BROW_DOWN_RIGHT: return .browDownRight
        case .BROW_INNER_UP: return .browInnerUp
        case .BROW_OUTER_UP_LEFT: return .browOuterUpLeft
        case .BROW_OUTER_UP_RIGHT: return .browOuterUpRight
        case .CHEEK_PUFF: return .cheekPuff
        case .CHEEK_SQUINT_LEFT: return .cheekSquintLeft
        case .CHEEK_SQUINT_RIGHT: return .cheekSquintRight
        case .NOSE_SNEER_LEFT: return .noseSneerLeft
        case .NOSE_SNEER_RIGHT: return .noseSneerRight
        case .TONGUE_OUT: return .tongueOut
        }
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

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

#endif
