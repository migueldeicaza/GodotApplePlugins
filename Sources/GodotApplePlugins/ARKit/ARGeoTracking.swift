//
//  ARGeoTracking.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit
import CoreLocation

@Godot
class ARGeoAnchor: RefCounted, @unchecked Sendable {
    enum AltitudeSource: Int, CaseIterable {
        case UNKNOWN = 0
        case COARSE = 1
        case PRECISE = 2
        case USER_DEFINED = 3
    }

    var anchor: ARKit.ARGeoAnchor?

    @available(iOS 14.0, *)
    convenience init(anchor: ARKit.ARGeoAnchor) {
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

    @Export var coordinate: Vector2 {
        guard let anchor else { return Vector2() }
        return Vector2(x: Float(anchor.coordinate.latitude), y: Float(anchor.coordinate.longitude))
    }

    @Export var altitude: Double {
        guard let anchor else { return 0 }
        return anchor.altitude
    }

    @Export(.enum) var altitudeSource: AltitudeSource {
        if #available(iOS 14.0, *) {
            guard let anchor else { return .UNKNOWN }
            switch anchor.altitudeSource {
            case .unknown:
                return .UNKNOWN
            case .coarse:
                return .COARSE
            case .precise:
                return .PRECISE
            case .userDefined:
                return .USER_DEFINED
            @unknown default:
                return .UNKNOWN
            }
        }
        return .UNKNOWN
    }

    @Callable
    static func create(latitude: Double, longitude: Double, altitude: Double) -> ARGeoAnchor {
        if #available(iOS 14.0, *) {
            let coordinate = CLLocationCoordinate2D(latitude: latitude, longitude: longitude)
            let native = ARKit.ARGeoAnchor(coordinate: coordinate, altitude: altitude)
            return ARGeoAnchor(anchor: native)
        }
        let anchor = ARGeoAnchor()
        return anchor
    }

    @Callable
    static func create_at_coordinate(latitude: Double, longitude: Double) -> ARGeoAnchor {
        if #available(iOS 14.0, *) {
            let coordinate = CLLocationCoordinate2D(latitude: latitude, longitude: longitude)
            let native = ARKit.ARGeoAnchor(coordinate: coordinate)
            return ARGeoAnchor(anchor: native)
        }
        return ARGeoAnchor()
    }
}

@Godot
class ARGeoTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export(.enum) var environmentTexturing: ARWorldTrackingConfiguration.EnvironmentTexturing = .NONE
    @Export var planeDetectionMask: Int = 0
    @Export var isLightEstimationEnabled: Bool = true
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var detectionImageGroupName: String = ""

    private var detectionImages: Set<ARKit.ARReferenceImage> = []

    @Callable
    static func is_supported() -> Bool {
        if #available(iOS 14.0, *) {
            return ARKit.ARGeoTrackingConfiguration.isSupported
        }
        return false
    }

    @Callable
    static func check_availability(callback: Callable) {
        if #available(iOS 14.0, *) {
            ARKit.ARGeoTrackingConfiguration.checkAvailability { available, error in
                DispatchQueue.main.async {
                    _ = callback.call(Variant(available), mapError(error))
                }
            }
        } else {
            _ = callback.call(Variant(false), Variant("Geo tracking requires iOS 14.0 or later."))
        }
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

    @available(iOS 14.0, *)
    func makeNative() -> ARKit.ARGeoTrackingConfiguration {
        let configuration = ARKit.ARGeoTrackingConfiguration()
        configuration.environmentTexturing = switch environmentTexturing {
        case .NONE: .none
        case .MANUAL: .manual
        case .AUTOMATIC: .automatic
        }
        configuration.isLightEstimationEnabled = isLightEstimationEnabled
        configuration.wantsHDREnvironmentTextures = wantsHDREnvironmentTextures

        var planeDetection: ARKit.ARPlaneDetection = []
        if planeDetectionMask & ARWorldTrackingConfiguration.PlaneDetection.HORIZONTAL.rawValue != 0 {
            planeDetection.insert(.horizontal)
        }
        if planeDetectionMask & ARWorldTrackingConfiguration.PlaneDetection.VERTICAL.rawValue != 0 {
            planeDetection.insert(.vertical)
        }
        configuration.planeDetection = planeDetection

        if !detectionImages.isEmpty {
            configuration.detectionImages = detectionImages
        } else if !detectionImageGroupName.isEmpty {
            if let images = ARKit.ARReferenceImage.referenceImages(inGroupNamed: detectionImageGroupName, bundle: nil) {
                configuration.detectionImages = images
            }
        }
        return configuration
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

@Godot
class ARGeoAnchor: RefCounted, @unchecked Sendable {
    enum AltitudeSource: Int, CaseIterable {
        case UNKNOWN = 0
        case COARSE = 1
        case PRECISE = 2
        case USER_DEFINED = 3
    }

    @Export var identifier: String = ""
    @Export var transform: Transform3D = Transform3D()
    @Export var coordinate: Vector2 = Vector2()
    @Export var altitude: Double = 0
    @Export(.enum) var altitudeSource: AltitudeSource = .UNKNOWN

    @Callable
    static func create(latitude: Double, longitude: Double, altitude: Double) -> ARGeoAnchor {
        let anchor = ARGeoAnchor()
        anchor.coordinate = Vector2(x: Float(latitude), y: Float(longitude))
        anchor.altitude = altitude
        return anchor
    }

    @Callable
    static func create_at_coordinate(latitude: Double, longitude: Double) -> ARGeoAnchor {
        let anchor = ARGeoAnchor()
        anchor.coordinate = Vector2(x: Float(latitude), y: Float(longitude))
        return anchor
    }
}

@Godot
class ARGeoTrackingConfiguration: RefCounted, @unchecked Sendable {
    @Export(.enum) var environmentTexturing: ARWorldTrackingConfiguration.EnvironmentTexturing = .NONE
    @Export var planeDetectionMask: Int = 0
    @Export var isLightEstimationEnabled: Bool = true
    @Export var wantsHDREnvironmentTextures: Bool = false
    @Export var detectionImageGroupName: String = ""

    @Callable static func is_supported() -> Bool { false }
    @Callable static func check_availability(callback: Callable) {
        _ = callback.call(Variant(false), Variant("Geo tracking is not available on this platform."))
    }
    @Callable func set_detection_image_group(groupName: String) -> Bool { false }
}

#endif
