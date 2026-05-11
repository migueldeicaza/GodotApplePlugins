//
//  ARMeshAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

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

    var anchor: ARKit.ARMeshAnchor?

    convenience init(anchor: ARKit.ARMeshAnchor) {
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

    @Export var geometryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        let vertexSource = geometry.vertices
        let count = vertexSource.count
        let stride = vertexSource.stride
        let baseAddress = vertexSource.buffer.contents().advanced(by: vertexSource.offset)
        var result: [Vector3] = []
        result.reserveCapacity(count)
        for i in 0..<count {
            let ptr = baseAddress.advanced(by: i * stride).assumingMemoryBound(to: Float.self)
            result.append(Vector3(x: ptr[0], y: ptr[1], z: ptr[2]))
        }
        return PackedVector3Array(result)
    }

    @Export var geometryNormals: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        let normalSource = geometry.normals
        let count = normalSource.count
        let stride = normalSource.stride
        let baseAddress = normalSource.buffer.contents().advanced(by: normalSource.offset)
        var result: [Vector3] = []
        result.reserveCapacity(count)
        for i in 0..<count {
            let ptr = baseAddress.advanced(by: i * stride).assumingMemoryBound(to: Float.self)
            result.append(Vector3(x: ptr[0], y: ptr[1], z: ptr[2]))
        }
        return PackedVector3Array(result)
    }

    @Export var triangleIndices: PackedInt32Array {
        guard let geometry = anchor?.geometry else { return PackedInt32Array() }
        let faces = geometry.faces
        let totalIndexCount = faces.count * faces.indexCountPerPrimitive
        let baseAddress = faces.buffer.contents()
        switch faces.bytesPerIndex {
        case 2:
            let indices = UnsafeBufferPointer<UInt16>(
                start: baseAddress.assumingMemoryBound(to: UInt16.self),
                count: totalIndexCount
            )
            return PackedInt32Array(indices.map(Int32.init))
        case 4:
            let indices = UnsafeBufferPointer<UInt32>(
                start: baseAddress.assumingMemoryBound(to: UInt32.self),
                count: totalIndexCount
            )
            return PackedInt32Array(indices.map(Int32.init))
        default:
            return PackedInt32Array()
        }
    }

    @Export var classificationPerFace: PackedInt32Array {
        guard let geometry = anchor?.geometry, let classification = geometry.classification else {
            return PackedInt32Array()
        }
        let count = classification.count
        let baseAddress = classification.buffer.contents().advanced(by: classification.offset)
        let values = UnsafeBufferPointer(
            start: baseAddress.assumingMemoryBound(to: UInt8.self),
            count: count
        )
        return PackedInt32Array(values.map { Int32(nativeClassificationToEnum($0).rawValue) })
    }

    @Callable
    func get_vertex(index: Int) -> Vector3 {
        let vertices = geometryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_normal(index: Int) -> Vector3 {
        let normals = geometryNormals
        guard index >= 0, index < Int(normals.size()) else { return Vector3() }
        return normals[index]
    }

    @Callable
    func get_face_classification(faceIndex: Int) -> Int {
        let classifications = classificationPerFace
        guard faceIndex >= 0, faceIndex < Int(classifications.size()) else { return 0 }
        return Int(classifications[faceIndex])
    }

    private func nativeClassificationToEnum(_ value: UInt8) -> MeshClassification {
        switch ARKit.ARMeshClassification(rawValue: Int(value)) {
        case .wall: return .WALL
        case .floor: return .FLOOR
        case .ceiling: return .CEILING
        case .table: return .TABLE
        case .seat: return .SEAT
        case .window: return .WINDOW
        case .door: return .DOOR
        default: return .NONE
        }
    }
}

#elseif canImport(ARKit) && os(visionOS)
import ARKit

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

    var anchor: MeshAnchor?

    convenience init(anchor: MeshAnchor) {
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

    @Export var geometryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        return packedVector3Array(geometry.vertices)
    }

    @Export var geometryNormals: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        return packedVector3Array(geometry.normals)
    }

    @Export var triangleIndices: PackedInt32Array {
        guard let geometry = anchor?.geometry else { return PackedInt32Array() }
        return packedInt32Array(triangleIndices: geometry.faces)
    }

    @Export var classificationPerFace: PackedInt32Array {
        guard let geometry = anchor?.geometry, let classification = geometry.classifications else {
            return PackedInt32Array()
        }
        let count = classification.count
        let baseAddress = classification.buffer.contents().advanced(by: classification.offset)
        let stride = classification.stride
        var result: [Int32] = []
        result.reserveCapacity(count)
        for i in 0..<count {
            let value = baseAddress.advanced(by: i * stride).assumingMemoryBound(to: UInt8.self).pointee
            result.append(Int32(nativeClassificationToEnum(value).rawValue))
        }
        return PackedInt32Array(result)
    }

    @Callable
    func get_vertex(index: Int) -> Vector3 {
        let vertices = geometryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_normal(index: Int) -> Vector3 {
        let normals = geometryNormals
        guard index >= 0, index < Int(normals.size()) else { return Vector3() }
        return normals[index]
    }

    @Callable
    func get_face_classification(faceIndex: Int) -> Int {
        let classifications = classificationPerFace
        guard faceIndex >= 0, faceIndex < Int(classifications.size()) else { return 0 }
        return Int(classifications[faceIndex])
    }

    private func nativeClassificationToEnum(_ value: UInt8) -> MeshClassification {
        switch MeshAnchor.MeshClassification(rawValue: Int(value)) {
        case .wall: return .WALL
        case .floor: return .FLOOR
        case .ceiling: return .CEILING
        case .table: return .TABLE
        case .seat: return .SEAT
        case .window: return .WINDOW
        case .door: return .DOOR
        default: return .NONE
        }
    }
}

#elseif canImport(ARKit) && os(macOS)

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

#endif
