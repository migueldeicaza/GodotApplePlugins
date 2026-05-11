//
//  ARFrame.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit
import CoreVideo

@Godot
class ARFrame: RefCounted, @unchecked Sendable {
    enum WorldMappingStatus: Int, CaseIterable {
        case NOT_AVAILABLE = 0
        case LIMITED = 1
        case EXTENDING = 2
        case MAPPED = 3
    }

    var frame: ARKit.ARFrame?

    convenience init(frame: ARKit.ARFrame) {
        self.init()
        self.frame = frame
    }

    @Export var timestamp: Double {
        frame?.timestamp ?? 0
    }

    @Export var camera: ARCamera {
        guard let camera = frame?.camera else { return ARCamera() }
        return ARCamera(camera: camera)
    }

    @Export var lightEstimate: ARLightEstimate? {
        guard let estimate = frame?.lightEstimate else { return nil }
        return ARLightEstimate(lightEstimate: estimate)
    }

    @Export var anchors: VariantArray {
        guard let frame else { return VariantArray() }
        return wrapAnchors(frame.anchors)
    }

    @Export var rawFeaturePoints: ARPointCloud? {
        guard let pointCloud = frame?.rawFeaturePoints else { return nil }
        return ARPointCloud(pointCloud: pointCloud)
    }

    @Export(.enum) var worldMappingStatus: WorldMappingStatus {
        guard let frame else { return .NOT_AVAILABLE }
        switch frame.worldMappingStatus {
        case .notAvailable:
            return .NOT_AVAILABLE
        case .limited:
            return .LIMITED
        case .extending:
            return .EXTENDING
        case .mapped:
            return .MAPPED
        @unknown default:
            return .NOT_AVAILABLE
        }
    }

    @Export var capturedImageSize: Vector2 {
        guard let image = frame?.capturedImage else { return Vector2() }
        return godotVector2(width: CVPixelBufferGetWidth(image), height: CVPixelBufferGetHeight(image))
    }

    @Callable
    func display_transform(orientation: Int, viewportSize: Vector2) -> PackedFloat32Array {
        guard let frame else { return PackedFloat32Array() }
        let transform = frame.displayTransform(
            for: uiInterfaceOrientation(orientation),
            viewportSize: cgSize(viewportSize)
        )
        return packedFloat32Array(transform)
    }

    @Callable
    func get_anchor(index: Int) -> RefCounted? {
        guard let frame, index >= 0, index < frame.anchors.count else { return nil }
        let variant = wrapAnchor(frame.anchors[index])
        return variant.asObject(RefCounted.self)
    }

    @Callable
    func get_captured_image_plane_count() -> Int {
        guard let image = frame?.capturedImage else { return 0 }
        let planeCount = CVPixelBufferGetPlaneCount(image)
        return planeCount == 0 ? 1 : planeCount
    }

    @Callable
    func get_captured_image_plane_size(plane: Int) -> Vector2 {
        guard let image = frame?.capturedImage else { return Vector2() }
        let planeCount = CVPixelBufferGetPlaneCount(image)
        if planeCount == 0 {
            guard plane == 0 else { return Vector2() }
            return godotVector2(width: CVPixelBufferGetWidth(image), height: CVPixelBufferGetHeight(image))
        }
        guard plane >= 0, plane < planeCount else { return Vector2() }
        return godotVector2(
            width: CVPixelBufferGetWidthOfPlane(image, plane),
            height: CVPixelBufferGetHeightOfPlane(image, plane)
        )
    }

    @Callable
    func get_captured_image_plane_stride(plane: Int) -> Int {
        guard let image = frame?.capturedImage else { return 0 }
        let planeCount = CVPixelBufferGetPlaneCount(image)
        if planeCount == 0 {
            return plane == 0 ? CVPixelBufferGetBytesPerRow(image) : 0
        }
        guard plane >= 0, plane < planeCount else { return 0 }
        return CVPixelBufferGetBytesPerRowOfPlane(image, plane)
    }

    @Callable
    func get_captured_image_plane_data(plane: Int) -> PackedByteArray {
        guard let image = frame?.capturedImage else { return PackedByteArray() }
        return packedByteArray(pixelBuffer: image, plane: plane)
    }

    @Export var segmentationBufferSize: Vector2 {
        if #available(iOS 13.0, *) {
            guard let buffer = frame?.segmentationBuffer else { return Vector2() }
            return godotVector2(width: CVPixelBufferGetWidth(buffer), height: CVPixelBufferGetHeight(buffer))
        }
        return Vector2()
    }

    @Callable
    func get_segmentation_buffer() -> PackedByteArray {
        if #available(iOS 13.0, *) {
            guard let buffer = frame?.segmentationBuffer else { return PackedByteArray() }
            return packedByteArray(pixelBuffer: buffer, plane: 0)
        }
        return PackedByteArray()
    }

    @Export var estimatedDepthDataSize: Vector2 {
        if #available(iOS 13.0, *) {
            guard let buffer = frame?.estimatedDepthData else { return Vector2() }
            return godotVector2(width: CVPixelBufferGetWidth(buffer), height: CVPixelBufferGetHeight(buffer))
        }
        return Vector2()
    }

    @Callable
    func get_estimated_depth_data() -> PackedByteArray {
        if #available(iOS 13.0, *) {
            guard let buffer = frame?.estimatedDepthData else { return PackedByteArray() }
            return packedByteArray(pixelBuffer: buffer, plane: 0)
        }
        return PackedByteArray()
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))
import ARKit

@Godot
class ARFrame: RefCounted, @unchecked Sendable {
    enum WorldMappingStatus: Int, CaseIterable {
        case NOT_AVAILABLE = 0
        case LIMITED = 1
        case EXTENDING = 2
        case MAPPED = 3
    }

    private var frameTimestamp: Double = 0
    private var frameCamera: ARCamera = ARCamera()
    private var frameLightEstimate: ARLightEstimate?
    private var frameAnchors: [RefCounted] = []
    private var frameRawFeaturePoints: ARPointCloud?
    private var frameWorldMappingStatus: WorldMappingStatus = .NOT_AVAILABLE

    convenience init(
        timestamp: Double,
        camera: ARCamera,
        anchors: [RefCounted],
        lightEstimate: ARLightEstimate? = nil,
        rawFeaturePoints: ARPointCloud? = nil,
        worldMappingStatus: WorldMappingStatus = .NOT_AVAILABLE
    ) {
        self.init()
        frameTimestamp = timestamp
        frameCamera = camera
        frameAnchors = anchors
        frameLightEstimate = lightEstimate
        frameRawFeaturePoints = rawFeaturePoints
        frameWorldMappingStatus = worldMappingStatus
    }

    @Export var timestamp: Double {
        frameTimestamp
    }

    @Export var camera: ARCamera {
        frameCamera
    }

    @Export var lightEstimate: ARLightEstimate? {
        frameLightEstimate
    }

    @Export var anchors: VariantArray {
        variantArray(frameAnchors)
    }

    @Export var rawFeaturePoints: ARPointCloud? {
        frameRawFeaturePoints
    }

    @Export(.enum) var worldMappingStatus: WorldMappingStatus {
        frameWorldMappingStatus
    }

    // Not available on visionOS/macOS: the passthrough camera feed is composited
    // by the OS and not accessible to applications. All captured-image, display-
    // transform, segmentation, and depth properties below return empty values.
    // This is an Apple platform limitation, not a missing binding.

    @Export var capturedImageSize: Vector2 {
        Vector2()
    }

    @Callable
    func display_transform(orientation: Int, viewportSize: Vector2) -> PackedFloat32Array {
        PackedFloat32Array()
    }

    @Callable
    func get_anchor(index: Int) -> RefCounted? {
        guard index >= 0, index < frameAnchors.count else { return nil }
        return frameAnchors[index]
    }

    @Callable
    func get_captured_image_plane_count() -> Int {
        0
    }

    @Callable
    func get_captured_image_plane_size(plane: Int) -> Vector2 {
        Vector2()
    }

    @Callable
    func get_captured_image_plane_stride(plane: Int) -> Int {
        0
    }

    @Callable
    func get_captured_image_plane_data(plane: Int) -> PackedByteArray {
        PackedByteArray()
    }

    @Export var segmentationBufferSize: Vector2 {
        Vector2()
    }

    @Callable
    func get_segmentation_buffer() -> PackedByteArray {
        PackedByteArray()
    }

    @Export var estimatedDepthDataSize: Vector2 {
        Vector2()
    }

    @Callable
    func get_estimated_depth_data() -> PackedByteArray {
        PackedByteArray()
    }
}

#endif
