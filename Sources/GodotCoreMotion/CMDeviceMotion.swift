//
//  CMDeviceMotion.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(CoreMotion) && (os(iOS) || os(macOS))
import CoreMotion
#endif

@Godot
class CMDeviceMotion: RefCounted, @unchecked Sendable {
    enum MagneticFieldCalibrationAccuracy: Int, CaseIterable {
        case UNCALIBRATED = -1
        case LOW = 0
        case MEDIUM = 1
        case HIGH = 2
    }

    @Export var timestamp: Double = 0
    @Export var attitudeQuaternion: Quaternion = Quaternion()
    @Export var attitudeRollPitchYaw: Vector3 = Vector3()
    @Export var rotationRate: Vector3 = Vector3()
    @Export var gravity: Vector3 = Vector3()
    @Export var userAcceleration: Vector3 = Vector3()
    @Export var magneticField: Vector3 = Vector3()
    @Export(.enum) var magneticFieldAccuracy: MagneticFieldCalibrationAccuracy = .UNCALIBRATED
    @Export var heading: Double = -1

    #if canImport(CoreMotion) && (os(iOS) || os(macOS))
    convenience init(motion: CoreMotion.CMDeviceMotion) {
        self.init()
        timestamp = motion.timestamp
        let q = motion.attitude.quaternion
        attitudeQuaternion = Quaternion(x: Float(q.x), y: Float(q.y), z: Float(q.z), w: Float(q.w))
        attitudeRollPitchYaw = Vector3(
            x: Float(motion.attitude.roll),
            y: Float(motion.attitude.pitch),
            z: Float(motion.attitude.yaw)
        )
        rotationRate = godotVector3(motion.rotationRate)
        gravity = godotVector3(motion.gravity)
        userAcceleration = godotVector3(motion.userAcceleration)
        magneticField = godotVector3(motion.magneticField.field)
        switch motion.magneticField.accuracy {
        case .uncalibrated: magneticFieldAccuracy = .UNCALIBRATED
        case .low: magneticFieldAccuracy = .LOW
        case .medium: magneticFieldAccuracy = .MEDIUM
        case .high: magneticFieldAccuracy = .HIGH
        @unknown default: magneticFieldAccuracy = .UNCALIBRATED
        }
        #if os(iOS)
        if #available(iOS 11.0, *) {
            heading = motion.heading
        }
        #endif
    }
    #endif
}
