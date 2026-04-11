//
//  ARPointCloud.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARPointCloud: RefCounted, @unchecked Sendable {
    var pointCloud: ARKit.ARPointCloud?

    convenience init(pointCloud: ARKit.ARPointCloud) {
        self.init()
        self.pointCloud = pointCloud
    }

    @Export var count: Int {
        pointCloud.map { Int($0.count) } ?? 0
    }

    @Export var points: PackedVector3Array {
        guard let pointCloud else { return PackedVector3Array() }
        let buffer = UnsafeBufferPointer(start: pointCloud.points, count: Int(pointCloud.count))
        return packedVector3Array(buffer)
    }

    @Export var identifiers: PackedInt64Array {
        guard let pointCloud else { return PackedInt64Array() }
        let buffer = UnsafeBufferPointer(start: pointCloud.identifiers, count: Int(pointCloud.count))
        return PackedInt64Array(buffer.map(Int64.init))
    }

    @Callable
    func get_point(index: Int) -> Vector3 {
        guard let pointCloud, index >= 0, index < Int(pointCloud.count) else { return Vector3() }
        return godotVector3(pointCloud.points[index])
    }

    @Callable
    func get_identifier(index: Int) -> Int {
        guard let pointCloud, index >= 0, index < Int(pointCloud.count) else { return 0 }
        return Int(bitPattern: UInt(pointCloud.identifiers[index]))
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

@Godot
class ARPointCloud: RefCounted, @unchecked Sendable {
    @Export var count: Int = 0
    @Export var points: PackedVector3Array = PackedVector3Array()
    @Export var identifiers: PackedInt64Array = PackedInt64Array()

    @Callable
    func get_point(index: Int) -> Vector3 {
        guard index >= 0, index < Int(points.size()) else { return Vector3() }
        return points[index]
    }

    @Callable
    func get_identifier(index: Int) -> Int {
        guard index >= 0, index < Int(identifiers.size()) else { return 0 }
        return Int(identifiers[index])
    }
}

#endif
