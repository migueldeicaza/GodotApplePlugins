//
//  ARLightEstimate.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARLightEstimate: RefCounted, @unchecked Sendable {
    var lightEstimate: ARKit.ARLightEstimate?

    convenience init(lightEstimate: ARKit.ARLightEstimate) {
        self.init()
        self.lightEstimate = lightEstimate
    }

    @Export var ambientIntensity: Double {
        Double(lightEstimate?.ambientIntensity ?? 0)
    }

    @Export var ambientColorTemperature: Double {
        Double(lightEstimate?.ambientColorTemperature ?? 0)
    }

    @Export var primaryLightDirection: Vector3 {
        guard let estimate = lightEstimate as? ARDirectionalLightEstimate else { return Vector3() }
        return godotVector3(estimate.primaryLightDirection)
    }

    @Export var primaryLightIntensity: Double {
        guard let estimate = lightEstimate as? ARDirectionalLightEstimate else { return 0 }
        return Double(estimate.primaryLightIntensity)
    }

    @Export var sphericalHarmonicsCoefficients: PackedFloat32Array {
        guard let estimate = lightEstimate as? ARDirectionalLightEstimate else {
            return PackedFloat32Array()
        }
        let count = estimate.sphericalHarmonicsCoefficients.count / MemoryLayout<Float>.stride
        return estimate.sphericalHarmonicsCoefficients.withUnsafeBytes {
            guard let baseAddress = $0.baseAddress?.assumingMemoryBound(to: Float.self) else {
                return PackedFloat32Array()
            }
            let buffer = UnsafeBufferPointer(start: baseAddress, count: count)
            return PackedFloat32Array(Array(buffer))
        }
    }

    @Callable
    func is_directional() -> Bool {
        lightEstimate is ARDirectionalLightEstimate
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

@Godot
class ARLightEstimate: RefCounted, @unchecked Sendable {
    @Export var ambientIntensity: Double = 0
    @Export var ambientColorTemperature: Double = 0
    @Export var primaryLightDirection: Vector3 = Vector3()
    @Export var primaryLightIntensity: Double = 0
    @Export var sphericalHarmonicsCoefficients: PackedFloat32Array = PackedFloat32Array()

    @Callable
    func is_directional() -> Bool {
        false
    }
}

#endif
