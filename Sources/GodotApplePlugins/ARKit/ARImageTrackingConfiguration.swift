//
//  ARImageTrackingConfiguration.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARImageTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var detectionImageGroupName: String = ""

    private var detectionImages: Set<ARKit.ARReferenceImage> = []

    @Callable
    static func is_supported() -> Bool {
        ARKit.ARImageTrackingConfiguration.isSupported
    }

    @Callable
    func set_detection_image_group(groupName: String) -> Bool {
        guard let images = ARKit.ARReferenceImage.referenceImages(inGroupNamed: groupName, bundle: nil) else {
            return false
        }
        detectionImageGroupName = groupName
        detectionImages = images
        return true
    }

    func makeNative() -> ARKit.ARImageTrackingConfiguration {
        let config = ARKit.ARImageTrackingConfiguration()
        config.isAutoFocusEnabled = isAutoFocusEnabled
        config.isLightEstimationEnabled = isLightEstimationEnabled
        config.maximumNumberOfTrackedImages = maximumNumberOfTrackedImages
        if !detectionImages.isEmpty {
            config.trackingImages = detectionImages
        } else if !detectionImageGroupName.isEmpty {
            if let images = ARKit.ARReferenceImage.referenceImages(inGroupNamed: detectionImageGroupName, bundle: nil) {
                config.trackingImages = images
            }
        }
        return config
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARImageTrackingConfiguration is an iOS-only standalone image tracking mode.
// On visionOS, image tracking is available through ARWorldTrackingConfiguration
// with detectionImageGroupName and ImageTrackingProvider. On macOS, image tracking
// is not available. This stub exists for API surface consistency only.

@Godot
class ARImageTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export var maximumNumberOfTrackedImages: Int = 0
    @Export var isAutoFocusEnabled: Bool = true
    @Export var isLightEstimationEnabled: Bool = true
    @Export var detectionImageGroupName: String = ""

    @Callable static func is_supported() -> Bool { false }
    @Callable func set_detection_image_group(groupName: String) -> Bool { false }
}

#endif
