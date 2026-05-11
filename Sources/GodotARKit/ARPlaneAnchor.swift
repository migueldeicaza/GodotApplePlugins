//
//  ARPlaneAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

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

    var anchor: ARKit.ARPlaneAnchor?

    convenience init(anchor: ARKit.ARPlaneAnchor) {
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

    @Export var center: Vector3 {
        guard let anchor else { return Vector3() }
        return godotVector3(anchor.center)
    }

    @Export var extent: Vector3 {
        guard let anchor else { return Vector3() }
        if #available(iOS 16.0, *) {
            return Vector3(
                x: anchor.planeExtent.width,
                y: 0,
                z: anchor.planeExtent.height
            )
        }
        return godotVector3(anchor.extent)
    }

    @Export var planeExtentWidth: Double {
        if #available(iOS 16.0, *) {
            return Double(anchor?.planeExtent.width ?? 0)
        }
        return 0
    }

    @Export var planeExtentHeight: Double {
        if #available(iOS 16.0, *) {
            return Double(anchor?.planeExtent.height ?? 0)
        }
        return 0
    }

    @Export var planeExtentRotationOnYAxis: Double {
        if #available(iOS 16.0, *) {
            return Double(anchor?.planeExtent.rotationOnYAxis ?? 0)
        }
        return 0
    }

    @Export(.enum) var alignment: Alignment {
        guard let anchor else { return .HORIZONTAL }
        switch anchor.alignment {
        case .horizontal:
            return .HORIZONTAL
        case .vertical:
            return .VERTICAL
        @unknown default:
            return .HORIZONTAL
        }
    }

    @Export(.enum) var classificationStatus: ClassificationStatus {
        guard #available(iOS 12.0, *), let anchor else { return .NOT_AVAILABLE }
        guard Self.is_classification_supported() else { return .NOT_AVAILABLE }
        switch anchor.classification {
        case ARKit.ARPlaneAnchor.Classification.none:
            return .UNKNOWN
        case .wall, .floor, .ceiling, .table, .seat, .window, .door:
            return .KNOWN
        @unknown default:
            return .UNKNOWN
        }
    }

    @Export(.enum) var classification: Classification {
        guard #available(iOS 12.0, *), let anchor else { return .NONE }
        switch anchor.classification {
        case ARKit.ARPlaneAnchor.Classification.none:
            return .NONE
        case .wall:
            return .WALL
        case .floor:
            return .FLOOR
        case .ceiling:
            return .CEILING
        case .table:
            return .TABLE
        case .seat:
            return .SEAT
        case .window:
            return .WINDOW
        case .door:
            return .DOOR
        @unknown default:
            return .NONE
        }
    }

    @Export var boundaryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        return packedVector3Array(geometry.boundaryVertices)
    }

    @Export var geometryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        return packedVector3Array(geometry.vertices)
    }

    @Export var textureCoordinates: PackedVector2Array {
        guard let geometry = anchor?.geometry else { return PackedVector2Array() }
        return packedVector2Array(geometry.textureCoordinates)
    }

    @Export var triangleIndices: PackedInt32Array {
        guard let geometry = anchor?.geometry else { return PackedInt32Array() }
        return PackedInt32Array(geometry.triangleIndices.map(Int32.init))
    }

    @Callable
    static func is_classification_supported() -> Bool {
        if #available(iOS 12.0, *) {
            return ARKit.ARPlaneAnchor.isClassificationSupported
        }
        return false
    }

    @Callable
    func get_boundary_vertex(index: Int) -> Vector3 {
        let vertices = boundaryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_geometry_vertex(index: Int) -> Vector3 {
        let vertices = geometryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_texture_coordinate(index: Int) -> Vector2 {
        let coordinates = textureCoordinates
        guard index >= 0, index < Int(coordinates.size()) else { return Vector2() }
        return coordinates[index]
    }

    @Callable
    func get_triangle_index(index: Int) -> Int {
        let indices = triangleIndices
        guard index >= 0, index < Int(indices.size()) else { return 0 }
        return Int(indices[index])
    }
}

#elseif canImport(ARKit) && os(visionOS)
import ARKit

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

    var anchor: PlaneAnchor?

    convenience init(anchor: PlaneAnchor) {
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

    @Export var center: Vector3 {
        guard let extentTransform = anchor?.geometry.extent.anchorFromExtentTransform else { return Vector3() }
        return Vector3(
            x: extentTransform.columns.3.x,
            y: extentTransform.columns.3.y,
            z: extentTransform.columns.3.z
        )
    }

    @Export var extent: Vector3 {
        guard let extent = anchor?.geometry.extent else { return Vector3() }
        return Vector3(x: extent.width, y: 0, z: extent.height)
    }

    @Export var planeExtentWidth: Double {
        Double(anchor?.geometry.extent.width ?? 0)
    }

    @Export var planeExtentHeight: Double {
        Double(anchor?.geometry.extent.height ?? 0)
    }

    @Export var planeExtentRotationOnYAxis: Double {
        guard let extentTransform = anchor?.geometry.extent.anchorFromExtentTransform else { return 0 }
        return Double(atan2(extentTransform.columns.0.z, extentTransform.columns.0.x))
    }

    @Export(.enum) var alignment: Alignment {
        guard let anchor else { return .HORIZONTAL }
        switch anchor.alignment {
        case .horizontal:
            return .HORIZONTAL
        case .vertical:
            return .VERTICAL
        case .slanted:
            return .SLANTED
        @unknown default:
            return .HORIZONTAL
        }
    }

    @Export(.enum) var classificationStatus: ClassificationStatus {
        switch anchor?.classification ?? .notAvailable {
        case .notAvailable:
            return .NOT_AVAILABLE
        case .undetermined:
            return .UNDETERMINED
        case .unknown:
            return .UNKNOWN
        case .wall, .floor, .ceiling, .table, .seat, .window, .door:
            return .KNOWN
        @unknown default:
            return .UNKNOWN
        }
    }

    @Export(.enum) var classification: Classification {
        switch anchor?.classification ?? .notAvailable {
        case .notAvailable, .undetermined, .unknown:
            return .NONE
        case .wall:
            return .WALL
        case .floor:
            return .FLOOR
        case .ceiling:
            return .CEILING
        case .table:
            return .TABLE
        case .seat:
            return .SEAT
        case .window:
            return .WINDOW
        case .door:
            return .DOOR
        @unknown default:
            return .NONE
        }
    }

    @Export var boundaryVertices: PackedVector3Array {
        geometryVertices
    }

    @Export var geometryVertices: PackedVector3Array {
        guard let geometry = anchor?.geometry else { return PackedVector3Array() }
        return packedVector3Array(geometry.meshVertices)
    }

    @Export var textureCoordinates: PackedVector2Array {
        PackedVector2Array()
    }

    @Export var triangleIndices: PackedInt32Array {
        guard let geometry = anchor?.geometry else { return PackedInt32Array() }
        return packedInt32Array(triangleIndices: geometry.meshFaces)
    }

    @Callable
    static func is_classification_supported() -> Bool {
        true
    }

    @Callable
    func get_boundary_vertex(index: Int) -> Vector3 {
        let vertices = boundaryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_geometry_vertex(index: Int) -> Vector3 {
        let vertices = geometryVertices
        guard index >= 0, index < Int(vertices.size()) else { return Vector3() }
        return vertices[index]
    }

    @Callable
    func get_texture_coordinate(index: Int) -> Vector2 {
        let coordinates = textureCoordinates
        guard index >= 0, index < Int(coordinates.size()) else { return Vector2() }
        return coordinates[index]
    }

    @Callable
    func get_triangle_index(index: Int) -> Int {
        let indices = triangleIndices
        guard index >= 0, index < Int(indices.size()) else { return 0 }
        return Int(indices[index])
    }
}

#elseif canImport(ARKit) && os(macOS)

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

    @Callable
    static func is_classification_supported() -> Bool {
        false
    }

    @Callable
    func get_boundary_vertex(index: Int) -> Vector3 {
        Vector3()
    }

    @Callable
    func get_geometry_vertex(index: Int) -> Vector3 {
        Vector3()
    }

    @Callable
    func get_texture_coordinate(index: Int) -> Vector2 {
        Vector2()
    }

    @Callable
    func get_triangle_index(index: Int) -> Int {
        0
    }
}

#endif
