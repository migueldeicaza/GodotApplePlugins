//
//  CMAccelerometerData.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(CoreMotion) && os(iOS)
import CoreMotion
#endif

@Godot
class CMAccelerometerData: RefCounted, @unchecked Sendable {
    @Export var timestamp: Double = 0
    @Export var acceleration: Vector3 = Vector3()

    #if canImport(CoreMotion) && os(iOS)
    convenience init(data: CoreMotion.CMAccelerometerData) {
        self.init()
        timestamp = data.timestamp
        acceleration = godotVector3(data.acceleration)
    }
    #endif
}

@Godot
class CMGyroData: RefCounted, @unchecked Sendable {
    @Export var timestamp: Double = 0
    @Export var rotationRate: Vector3 = Vector3()

    #if canImport(CoreMotion) && os(iOS)
    convenience init(data: CoreMotion.CMGyroData) {
        self.init()
        timestamp = data.timestamp
        rotationRate = godotVector3(data.rotationRate)
    }
    #endif
}

@Godot
class CMMagnetometerData: RefCounted, @unchecked Sendable {
    @Export var timestamp: Double = 0
    @Export var magneticField: Vector3 = Vector3()

    #if canImport(CoreMotion) && os(iOS)
    convenience init(data: CoreMotion.CMMagnetometerData) {
        self.init()
        timestamp = data.timestamp
        magneticField = godotVector3(data.magneticField)
    }
    #endif
}
