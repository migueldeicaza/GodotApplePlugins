//
//  ARCollaborationData.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARCollaborationData: RefCounted, @unchecked Sendable {
    enum Priority: Int, CaseIterable {
        case CRITICAL = 0
        case OPTIONAL = 1
    }

    var collaborationData: ARKit.ARSession.CollaborationData?

    convenience init(collaborationData: ARKit.ARSession.CollaborationData) {
        self.init()
        self.collaborationData = collaborationData
    }

    @Export(.enum) var priority: Priority {
        guard let collaborationData else { return .OPTIONAL }
        switch collaborationData.priority {
        case .critical:
            return .CRITICAL
        case .optional:
            return .OPTIONAL
        @unknown default:
            return .OPTIONAL
        }
    }

    @Callable
    func serialize() -> PackedByteArray {
        guard let collaborationData else { return PackedByteArray() }
        do {
            let data = try NSKeyedArchiver.archivedData(withRootObject: collaborationData, requiringSecureCoding: true)
            return PackedByteArray(Array(data))
        } catch {
            return PackedByteArray()
        }
    }

    @Callable
    static func deserialize(data: PackedByteArray) -> ARCollaborationData? {
        let bytes = Array(data)
        guard !bytes.isEmpty else { return nil }
        let nsData = Data(bytes)
        do {
            guard let native = try NSKeyedUnarchiver.unarchivedObject(
                ofClass: ARKit.ARSession.CollaborationData.self,
                from: nsData
            ) else { return nil }
            return ARCollaborationData(collaborationData: native)
        } catch {
            return nil
        }
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARSession.CollaborationData is an iOS-only API for multi-peer AR via
// MultipeerConnectivity. visionOS does not expose a collaboration data
// stream through ARKit. This type exists for API surface consistency but
// serialize/deserialize are no-ops on these platforms.

@Godot
class ARCollaborationData: RefCounted, @unchecked Sendable {
    enum Priority: Int, CaseIterable {
        case CRITICAL = 0
        case OPTIONAL = 1
    }

    @Export(.enum) var priority: Priority = .OPTIONAL

    @Callable func serialize() -> PackedByteArray { PackedByteArray() }
    @Callable static func deserialize(data: PackedByteArray) -> ARCollaborationData? { nil }
}

#endif
