//
//  ARAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARAnchor: RefCounted, @unchecked Sendable {
    var anchor: ARKit.ARAnchor?
    var displayName: String = ""

    convenience init(anchor: ARKit.ARAnchor) {
        self.init()
        self.anchor = anchor
        if #available(iOS 12.0, *) {
            self.displayName = anchor.name ?? ""
        }
    }

    @Export var identifier: String {
        anchor?.identifier.uuidString ?? ""
    }

    @Export var name: String {
        displayName
    }

    @Export var sessionIdentifier: String {
        if #available(iOS 13.0, *) {
            return anchor?.sessionIdentifier?.uuidString ?? ""
        }
        return ""
    }

    @Export var transform: Transform3D {
        guard let anchor else { return Transform3D() }
        return godotTransform3D(anchor.transform)
    }

    @Callable
    static func create(transform: Transform3D) -> ARAnchor {
        let wrapped = ARAnchor()
        wrapped.anchor = ARKit.ARAnchor(transform: simdTransform(transform))
        return wrapped
    }

    @Callable
    static func create_named(name: String, transform: Transform3D) -> ARAnchor {
        let wrapped = ARAnchor()
        wrapped.displayName = name
        wrapped.anchor = ARKit.ARAnchor(name: name, transform: simdTransform(transform))
        return wrapped
    }

    @Callable
    func copy_anchor() -> ARAnchor? {
        guard let copied = anchor?.copy() as? ARKit.ARAnchor else { return nil }
        return ARAnchor(anchor: copied)
    }
}

#elseif canImport(ARKit) && os(visionOS)
import ARKit

@Godot
class ARAnchor: RefCounted, @unchecked Sendable {
    var anchor: WorldAnchor?
    var displayName: String = ""
    private var storedIdentifier: String = ""
    private var storedTransform: Transform3D = Transform3D()

    convenience init(anchor: WorldAnchor) {
        self.init()
        self.anchor = anchor
        storedIdentifier = anchor.id.uuidString
        storedTransform = godotTransform3D(anchor.originFromAnchorTransform)
    }

    @Export var identifier: String {
        if let anchor {
            return anchor.id.uuidString
        }
        return storedIdentifier
    }

    @Export var name: String {
        displayName
    }

    @Export var sessionIdentifier: String {
        ""
    }

    @Export var transform: Transform3D {
        return storedTransform
    }

    @Callable
    static func create(transform: Transform3D) -> ARAnchor {
        let wrapped = ARAnchor()
        wrapped.anchor = WorldAnchor(originFromAnchorTransform: simdTransform(transform))
        wrapped.storedTransform = transform
        return wrapped
    }

    @Callable
    static func create_named(name: String, transform: Transform3D) -> ARAnchor {
        let wrapped = create(transform: transform)
        wrapped.displayName = name
        return wrapped
    }

    @Callable
    func copy_anchor() -> ARAnchor? {
        let wrapped = ARAnchor.create_named(name: displayName, transform: transform)
        return wrapped
    }
}

#elseif canImport(ARKit) && os(macOS)

@Godot
class ARAnchor: RefCounted, @unchecked Sendable {
    var displayName: String = ""
    var storedIdentifier: String = UUID().uuidString
    var storedTransform: Transform3D = Transform3D()

    @Export var identifier: String {
        storedIdentifier
    }

    @Export var name: String {
        displayName
    }

    @Export var sessionIdentifier: String {
        ""
    }

    @Export var transform: Transform3D {
        storedTransform
    }

    @Callable
    static func create(transform: Transform3D) -> ARAnchor {
        let wrapped = ARAnchor()
        wrapped.storedTransform = transform
        return wrapped
    }

    @Callable
    static func create_named(name: String, transform: Transform3D) -> ARAnchor {
        let wrapped = create(transform: transform)
        wrapped.displayName = name
        return wrapped
    }

    @Callable
    func copy_anchor() -> ARAnchor? {
        ARAnchor.create_named(name: displayName, transform: transform)
    }
}

#endif
