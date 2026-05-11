//
//  ARKitShared.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation
import simd

func variantArray(_ values: [RefCounted]) -> VariantArray {
    let result = VariantArray()
    for value in values {
        result.append(Variant(value))
    }
    return result
}

func godotVector3(_ value: SIMD3<Float>) -> Vector3 {
    Vector3(x: value.x, y: value.y, z: value.z)
}

func simdVector3(_ value: Vector3) -> SIMD3<Float> {
    SIMD3<Float>(value.x, value.y, value.z)
}

func godotTransform3D(_ matrix: simd_float4x4) -> Transform3D {
    var basis = Basis()
    basis.x = godotVector3(SIMD3<Float>(matrix.columns.0.x, matrix.columns.0.y, matrix.columns.0.z))
    basis.y = godotVector3(SIMD3<Float>(matrix.columns.1.x, matrix.columns.1.y, matrix.columns.1.z))
    basis.z = godotVector3(SIMD3<Float>(matrix.columns.2.x, matrix.columns.2.y, matrix.columns.2.z))

    var transform = Transform3D()
    transform.basis = basis
    transform.origin = godotVector3(SIMD3<Float>(matrix.columns.3.x, matrix.columns.3.y, matrix.columns.3.z))
    return transform
}

func simdTransform(_ transform: Transform3D) -> simd_float4x4 {
    let basis = transform.basis
    return simd_float4x4(columns: (
        SIMD4<Float>(basis.x.x, basis.x.y, basis.x.z, 0),
        SIMD4<Float>(basis.y.x, basis.y.y, basis.y.z, 0),
        SIMD4<Float>(basis.z.x, basis.z.y, basis.z.z, 0),
        SIMD4<Float>(transform.origin.x, transform.origin.y, transform.origin.z, 1)
    ))
}

func packedFloat32Array(_ matrix: simd_float3x3) -> PackedFloat32Array {
    PackedFloat32Array([
        matrix.columns.0.x, matrix.columns.0.y, matrix.columns.0.z,
        matrix.columns.1.x, matrix.columns.1.y, matrix.columns.1.z,
        matrix.columns.2.x, matrix.columns.2.y, matrix.columns.2.z,
    ])
}

func packedFloat32Array(_ matrix: simd_float4x4) -> PackedFloat32Array {
    PackedFloat32Array([
        matrix.columns.0.x, matrix.columns.0.y, matrix.columns.0.z, matrix.columns.0.w,
        matrix.columns.1.x, matrix.columns.1.y, matrix.columns.1.z, matrix.columns.1.w,
        matrix.columns.2.x, matrix.columns.2.y, matrix.columns.2.z, matrix.columns.2.w,
        matrix.columns.3.x, matrix.columns.3.y, matrix.columns.3.z, matrix.columns.3.w,
    ])
}

func godotEulerAngles(_ matrix: simd_float4x4) -> Vector3 {
    let pitch = atan2(matrix.columns.2.y, matrix.columns.2.z)
    let yaw = atan2(-matrix.columns.2.x, hypot(matrix.columns.2.y, matrix.columns.2.z))
    let roll = atan2(matrix.columns.1.x, matrix.columns.0.x)
    return Vector3(x: pitch, y: yaw, z: roll)
}

#if canImport(ARKit) && os(iOS)
import ARKit
import CoreVideo
import UIKit

func godotVector2(_ size: CGSize) -> Vector2 {
    Vector2(x: Float(size.width), y: Float(size.height))
}

func godotVector2(_ point: CGPoint) -> Vector2 {
    Vector2(x: Float(point.x), y: Float(point.y))
}

func godotVector2(width: Int, height: Int) -> Vector2 {
    Vector2(x: Float(width), y: Float(height))
}

func cgSize(_ value: Vector2) -> CGSize {
    CGSize(width: CGFloat(value.x), height: CGFloat(value.y))
}

func cgPoint(_ value: Vector2) -> CGPoint {
    CGPoint(x: CGFloat(value.x), y: CGFloat(value.y))
}

func uiInterfaceOrientation(_ rawValue: Int) -> UIInterfaceOrientation {
    UIInterfaceOrientation(rawValue: rawValue) ?? .portrait
}

func packedFloat32Array(_ transform: CGAffineTransform) -> PackedFloat32Array {
    PackedFloat32Array([
        Float(transform.a),
        Float(transform.b),
        Float(transform.c),
        Float(transform.d),
        Float(transform.tx),
        Float(transform.ty),
    ])
}

func packedVector3Array(_ values: [SIMD3<Float>]) -> PackedVector3Array {
    PackedVector3Array(values.map(godotVector3))
}

func packedVector3Array(_ values: UnsafeBufferPointer<SIMD3<Float>>) -> PackedVector3Array {
    PackedVector3Array(Array(values).map(godotVector3))
}

func packedVector2Array(_ values: [SIMD2<Float>]) -> PackedVector2Array {
    PackedVector2Array(values.map { Vector2(x: $0.x, y: $0.y) })
}

func packedVector2Array(_ values: UnsafeBufferPointer<SIMD2<Float>>) -> PackedVector2Array {
    PackedVector2Array(Array(values).map { Vector2(x: $0.x, y: $0.y) })
}

func packedByteArray(pixelBuffer: CVPixelBuffer, plane: Int) -> PackedByteArray {
    CVPixelBufferLockBaseAddress(pixelBuffer, .readOnly)
    defer { CVPixelBufferUnlockBaseAddress(pixelBuffer, .readOnly) }

    let planeCount = CVPixelBufferGetPlaneCount(pixelBuffer)
    let isPlanar = planeCount > 0

    if isPlanar {
        guard plane >= 0, plane < planeCount else { return PackedByteArray() }
        guard let baseAddress = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, plane) else {
            return PackedByteArray()
        }
        let bytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, plane)
        let height = CVPixelBufferGetHeightOfPlane(pixelBuffer, plane)
        let buffer = UnsafeBufferPointer(
            start: baseAddress.assumingMemoryBound(to: UInt8.self),
            count: bytesPerRow * height
        )
        return PackedByteArray(Array(buffer))
    }

    guard plane == 0, let baseAddress = CVPixelBufferGetBaseAddress(pixelBuffer) else {
        return PackedByteArray()
    }
    let buffer = UnsafeBufferPointer(
        start: baseAddress.assumingMemoryBound(to: UInt8.self),
        count: CVPixelBufferGetDataSize(pixelBuffer)
    )
    return PackedByteArray(Array(buffer))
}

func wrapAnchor(_ anchor: ARKit.ARAnchor) -> Variant {
    if let planeAnchor = anchor as? ARKit.ARPlaneAnchor {
        return Variant(ARPlaneAnchor(anchor: planeAnchor))
    }
    if let imageAnchor = anchor as? ARKit.ARImageAnchor {
        return Variant(ARImageAnchor(anchor: imageAnchor))
    }
    if let meshAnchor = anchor as? ARKit.ARMeshAnchor {
        return Variant(ARMeshAnchor(anchor: meshAnchor))
    }
    if let faceAnchor = anchor as? ARKit.ARFaceAnchor {
        return Variant(ARFaceAnchor(anchor: faceAnchor))
    }
    if let bodyAnchor = anchor as? ARKit.ARBodyAnchor {
        return Variant(ARBodyAnchor(anchor: bodyAnchor))
    }
    if let probeAnchor = anchor as? ARKit.AREnvironmentProbeAnchor {
        return Variant(AREnvironmentProbeAnchor(anchor: probeAnchor))
    }
    if #available(iOS 14.0, *), let geoAnchor = anchor as? ARKit.ARGeoAnchor {
        return Variant(ARGeoAnchor(anchor: geoAnchor))
    }
    return Variant(ARAnchor(anchor: anchor))
}

func wrapAnchors(_ anchors: [ARKit.ARAnchor]) -> VariantArray {
    let result = VariantArray()
    for anchor in anchors {
        result.append(wrapAnchor(anchor))
    }
    return result
}
#endif

#if canImport(ARKit) && os(visionOS)
import ARKit
import Metal

func packedVector3Array(_ source: GeometrySource) -> PackedVector3Array {
    guard source.componentsPerVector >= 3 else { return PackedVector3Array() }

    let baseAddress = source.buffer.contents().advanced(by: source.offset)
    var result: [Vector3] = []
    result.reserveCapacity(source.count)

    for index in 0..<source.count {
        let vectorAddress = baseAddress.advanced(by: index * source.stride)
        let components = vectorAddress.assumingMemoryBound(to: Float.self)
        result.append(Vector3(x: components[0], y: components[1], z: components[2]))
    }

    return PackedVector3Array(result)
}

func packedInt32Array(triangleIndices geometryElement: GeometryElement) -> PackedInt32Array {
    let totalIndexCount = geometryElement.count * 3
    guard totalIndexCount > 0 else { return PackedInt32Array() }

    let baseAddress = geometryElement.buffer.contents()
    switch geometryElement.bytesPerIndex {
    case 2:
        let indices = UnsafeBufferPointer(
            start: baseAddress.assumingMemoryBound(to: UInt16.self),
            count: totalIndexCount
        )
        return PackedInt32Array(indices.map(Int32.init))
    case 4:
        let indices = UnsafeBufferPointer(
            start: baseAddress.assumingMemoryBound(to: UInt32.self),
            count: totalIndexCount
        )
        return PackedInt32Array(indices.map(Int32.init))
    default:
        return PackedInt32Array()
    }
}
#endif
