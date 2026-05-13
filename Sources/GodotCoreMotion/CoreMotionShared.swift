//
//  CoreMotionShared.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(CoreMotion)
import CoreMotion

func godotVector3(_ value: CMAcceleration) -> Vector3 {
    Vector3(x: Float(value.x), y: Float(value.y), z: Float(value.z))
}

func godotVector3(_ value: CMRotationRate) -> Vector3 {
    Vector3(x: Float(value.x), y: Float(value.y), z: Float(value.z))
}

func godotVector3(_ value: CMMagneticField) -> Vector3 {
    Vector3(x: Float(value.x), y: Float(value.y), z: Float(value.z))
}

func mapError(_ error: (any Error)?) -> Variant? {
    if let error {
        return Variant(error.localizedDescription)
    }
    return nil
}
#else
func mapError(_ error: (any Error)?) -> Variant? {
    if let error {
        return Variant(error.localizedDescription)
    }
    return nil
}
#endif
