//
//  ARWorldMap.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARWorldMap: RefCounted, @unchecked Sendable {
    var worldMap: ARKit.ARWorldMap?

    convenience init(worldMap: ARKit.ARWorldMap) {
        self.init()
        self.worldMap = worldMap
    }

    @Export var center: Vector3 {
        guard let worldMap else { return Vector3() }
        return godotVector3(worldMap.center)
    }

    @Export var extent: Vector3 {
        guard let worldMap else { return Vector3() }
        return godotVector3(worldMap.extent)
    }

    @Export var anchors: VariantArray {
        guard let worldMap else { return VariantArray() }
        return wrapAnchors(worldMap.anchors)
    }

    @Export var rawFeaturePoints: ARPointCloud? {
        guard let points = worldMap?.rawFeaturePoints else { return nil }
        return ARPointCloud(pointCloud: points)
    }

    @Callable
    func serialize() -> PackedByteArray {
        guard let worldMap else { return PackedByteArray() }
        do {
            let data = try NSKeyedArchiver.archivedData(withRootObject: worldMap, requiringSecureCoding: true)
            return PackedByteArray(Array(data))
        } catch {
            return PackedByteArray()
        }
    }

    @Callable
    static func deserialize(data: PackedByteArray) -> ARWorldMap? {
        let bytes = Array(data)
        guard !bytes.isEmpty else { return nil }
        let nsData = Data(bytes)
        do {
            guard let nativeMap = try NSKeyedUnarchiver.unarchivedObject(ofClass: ARKit.ARWorldMap.self, from: nsData) else {
                return nil
            }
            return ARWorldMap(worldMap: nativeMap)
        } catch {
            return nil
        }
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARWorldMap is an iOS-only concept. visionOS persists spatial anchors
// automatically via WorldTrackingProvider and does not use ARWorldMap for
// session save/restore. This type exists for API surface consistency but
// serialize/deserialize are no-ops on these platforms.

@Godot
class ARWorldMap: RefCounted, @unchecked Sendable {
    @Export var center: Vector3 = Vector3()
    @Export var extent: Vector3 = Vector3()
    @Export var anchors: VariantArray = VariantArray()
    @Export var rawFeaturePoints: ARPointCloud?

    @Callable func serialize() -> PackedByteArray { PackedByteArray() }
    @Callable static func deserialize(data: PackedByteArray) -> ARWorldMap? { nil }
}

#endif
