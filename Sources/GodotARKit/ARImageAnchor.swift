//
//  ARImageAnchor.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARImageAnchor: RefCounted, @unchecked Sendable {
    var anchor: ARKit.ARImageAnchor?

    convenience init(anchor: ARKit.ARImageAnchor) {
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

    @Export var referenceImageName: String {
        anchor?.referenceImage.name ?? ""
    }

    @Export var referenceImagePhysicalSize: Vector2 {
        guard let size = anchor?.referenceImage.physicalSize else { return Vector2() }
        return Vector2(x: Float(size.width), y: Float(size.height))
    }

    @Export var isTracked: Bool {
        if #available(iOS 12.0, *) {
            return anchor?.isTracked ?? false
        }
        return anchor != nil
    }

    @Export var estimatedScaleFactor: Double {
        if #available(iOS 13.0, *) {
            return Double(anchor?.estimatedScaleFactor ?? 1.0)
        }
        return 1.0
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

@Godot
class ARImageAnchor: RefCounted, @unchecked Sendable {
    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var referenceImageName: String = ""
    @Export var referenceImagePhysicalSize: Vector2 = Vector2()
    @Export var isTracked: Bool = false
    @Export var estimatedScaleFactor: Double = 1.0
}

#endif
