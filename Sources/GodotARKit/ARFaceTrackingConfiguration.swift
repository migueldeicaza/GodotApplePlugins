//
//  ARFaceTrackingConfiguration.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARFaceTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedFaces: Int = 1
    @Export var isLightEstimationEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var isWorldTrackingEnabled: Bool = false

    @Callable
    static func is_supported() -> Bool {
        ARKit.ARFaceTrackingConfiguration.isSupported
    }

    @Callable
    static func supports_world_tracking() -> Bool {
        if #available(iOS 13.0, *) {
            return ARKit.ARFaceTrackingConfiguration.supportsWorldTracking
        }
        return false
    }

    func makeNative() -> ARKit.ARFaceTrackingConfiguration {
        let config = ARKit.ARFaceTrackingConfiguration()
        if #available(iOS 13.0, *) {
            config.maximumNumberOfTrackedFaces = maximumNumberOfTrackedFaces
            config.isWorldTrackingEnabled = isWorldTrackingEnabled
        }
        config.isLightEstimationEnabled = isLightEstimationEnabled
        config.providesAudioData = providesAudioData
        return config
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARFaceTrackingConfiguration is an iOS-only API that uses the front-facing
// TrueDepth camera. visionOS and macOS do not expose face tracking through ARKit.
// This stub exists for API surface consistency only.

@Godot
class ARFaceTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedFaces: Int = 1
    @Export var isLightEstimationEnabled: Bool = true
    @Export var providesAudioData: Bool = false
    @Export var isWorldTrackingEnabled: Bool = false

    @Callable static func is_supported() -> Bool { false }
    @Callable static func supports_world_tracking() -> Bool { false }
}

#endif
