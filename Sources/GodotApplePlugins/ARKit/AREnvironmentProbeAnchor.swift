//
//  AREnvironmentProbeAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class AREnvironmentProbeAnchor: RefCounted, @unchecked Sendable {
    var anchor: ARKit.AREnvironmentProbeAnchor?

    convenience init(anchor: ARKit.AREnvironmentProbeAnchor) {
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

    @Export var extent: Vector3 {
        guard let anchor else { return Vector3() }
        return godotVector3(anchor.extent)
    }

    @Export var hasEnvironmentTexture: Bool {
        anchor?.environmentTexture != nil
    }

    @Export var textureWidth: Int {
        guard let texture = anchor?.environmentTexture else { return 0 }
        return texture.width
    }

    @Export var textureHeight: Int {
        guard let texture = anchor?.environmentTexture else { return 0 }
        return texture.height
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

@Godot
class AREnvironmentProbeAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var extent: Vector3 = Vector3()
    @Export var hasEnvironmentTexture: Bool = false
    @Export var textureWidth: Int = 0
    @Export var textureHeight: Int = 0
}

#endif
