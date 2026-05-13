//
//  CMMotionManager.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(CoreMotion) && os(iOS)
import CoreMotion

@Godot
class CMMotionManager: RefCounted, @unchecked Sendable {
    enum AttitudeReferenceFrame: Int, CaseIterable {
        case XARBITRARY_Z_VERTICAL = 1
        case XARBITRARY_CORRECTED_Z_VERTICAL = 2
        case XMAGNETIC_NORTH_Z_VERTICAL = 4
        case XTRUE_NORTH_Z_VERTICAL = 8
    }

    @Signal("accelerometer") var accelerometer_updated: SignalWithArguments<CMAccelerometerData>
    @Signal("gyro") var gyro_updated: SignalWithArguments<CMGyroData>
    @Signal("magnetometer") var magnetometer_updated: SignalWithArguments<CMMagnetometerData>
    @Signal("deviceMotion") var device_motion_updated: SignalWithArguments<CMDeviceMotion>
    @Signal("message") var update_failed: SignalWithArguments<String>

    private let manager = CoreMotion.CMMotionManager()
    private let updateQueue: OperationQueue = {
        let q = OperationQueue()
        q.qualityOfService = .userInitiated
        q.maxConcurrentOperationCount = 1
        return q
    }()

    @Export var isAccelerometerAvailable: Bool { manager.isAccelerometerAvailable }
    @Export var isAccelerometerActive: Bool { manager.isAccelerometerActive }
    @Export var isGyroAvailable: Bool { manager.isGyroAvailable }
    @Export var isGyroActive: Bool { manager.isGyroActive }
    @Export var isMagnetometerAvailable: Bool { manager.isMagnetometerAvailable }
    @Export var isMagnetometerActive: Bool { manager.isMagnetometerActive }
    @Export var isDeviceMotionAvailable: Bool { manager.isDeviceMotionAvailable }
    @Export var isDeviceMotionActive: Bool { manager.isDeviceMotionActive }

    @Export var accelerometerUpdateInterval: Double {
        get { manager.accelerometerUpdateInterval }
        set { manager.accelerometerUpdateInterval = newValue }
    }
    @Export var gyroUpdateInterval: Double {
        get { manager.gyroUpdateInterval }
        set { manager.gyroUpdateInterval = newValue }
    }
    @Export var magnetometerUpdateInterval: Double {
        get { manager.magnetometerUpdateInterval }
        set { manager.magnetometerUpdateInterval = newValue }
    }
    @Export var deviceMotionUpdateInterval: Double {
        get { manager.deviceMotionUpdateInterval }
        set { manager.deviceMotionUpdateInterval = newValue }
    }

    @Export var accelerometerData: CMAccelerometerData? {
        guard let data = manager.accelerometerData else { return nil }
        return CMAccelerometerData(data: data)
    }
    @Export var gyroData: CMGyroData? {
        guard let data = manager.gyroData else { return nil }
        return CMGyroData(data: data)
    }
    @Export var magnetometerData: CMMagnetometerData? {
        guard let data = manager.magnetometerData else { return nil }
        return CMMagnetometerData(data: data)
    }
    @Export var deviceMotion: CMDeviceMotion? {
        guard let motion = manager.deviceMotion else { return nil }
        return CMDeviceMotion(motion: motion)
    }

    @Callable
    func start_accelerometer_updates() {
        manager.startAccelerometerUpdates(to: updateQueue) { [weak self] data, error in
            guard let self else { return }
            let wrapped = data.map { CMAccelerometerData(data: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.update_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.accelerometer_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_accelerometer_updates() {
        manager.stopAccelerometerUpdates()
    }

    @Callable
    func start_gyro_updates() {
        manager.startGyroUpdates(to: updateQueue) { [weak self] data, error in
            guard let self else { return }
            let wrapped = data.map { CMGyroData(data: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.update_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.gyro_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_gyro_updates() {
        manager.stopGyroUpdates()
    }

    @Callable
    func start_magnetometer_updates() {
        manager.startMagnetometerUpdates(to: updateQueue) { [weak self] data, error in
            guard let self else { return }
            let wrapped = data.map { CMMagnetometerData(data: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.update_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.magnetometer_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_magnetometer_updates() {
        manager.stopMagnetometerUpdates()
    }

    @Callable
    func start_device_motion_updates(referenceFrame: AttitudeReferenceFrame = .XARBITRARY_Z_VERTICAL) {
        let frame = Self.toCMAttitudeReferenceFrame(referenceFrame)
        manager.startDeviceMotionUpdates(using: frame, to: updateQueue) { [weak self] motion, error in
            guard let self else { return }
            let wrapped = motion.map { CMDeviceMotion(motion: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.update_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.device_motion_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_device_motion_updates() {
        manager.stopDeviceMotionUpdates()
    }

    private static func toCMAttitudeReferenceFrame(_ frame: AttitudeReferenceFrame) -> CoreMotion.CMAttitudeReferenceFrame {
        switch frame {
        case .XARBITRARY_Z_VERTICAL: return .xArbitraryZVertical
        case .XARBITRARY_CORRECTED_Z_VERTICAL: return .xArbitraryCorrectedZVertical
        case .XMAGNETIC_NORTH_Z_VERTICAL: return .xMagneticNorthZVertical
        case .XTRUE_NORTH_Z_VERTICAL: return .xTrueNorthZVertical
        }
    }
}

#else

@Godot
class CMMotionManager: RefCounted, @unchecked Sendable {
    enum AttitudeReferenceFrame: Int, CaseIterable {
        case XARBITRARY_Z_VERTICAL = 1
        case XARBITRARY_CORRECTED_Z_VERTICAL = 2
        case XMAGNETIC_NORTH_Z_VERTICAL = 4
        case XTRUE_NORTH_Z_VERTICAL = 8
    }

    @Signal("accelerometer") var accelerometer_updated: SignalWithArguments<CMAccelerometerData>
    @Signal("gyro") var gyro_updated: SignalWithArguments<CMGyroData>
    @Signal("magnetometer") var magnetometer_updated: SignalWithArguments<CMMagnetometerData>
    @Signal("deviceMotion") var device_motion_updated: SignalWithArguments<CMDeviceMotion>
    @Signal("message") var update_failed: SignalWithArguments<String>

    @Export var isAccelerometerAvailable: Bool = false
    @Export var isAccelerometerActive: Bool = false
    @Export var isGyroAvailable: Bool = false
    @Export var isGyroActive: Bool = false
    @Export var isMagnetometerAvailable: Bool = false
    @Export var isMagnetometerActive: Bool = false
    @Export var isDeviceMotionAvailable: Bool = false
    @Export var isDeviceMotionActive: Bool = false

    @Export var accelerometerUpdateInterval: Double = 0
    @Export var gyroUpdateInterval: Double = 0
    @Export var magnetometerUpdateInterval: Double = 0
    @Export var deviceMotionUpdateInterval: Double = 0

    @Export var accelerometerData: CMAccelerometerData?
    @Export var gyroData: CMGyroData?
    @Export var magnetometerData: CMMagnetometerData?
    @Export var deviceMotion: CMDeviceMotion?

    @Callable func start_accelerometer_updates() {
        update_failed.emit("CMMotionManager is not available on this platform.")
    }
    @Callable func stop_accelerometer_updates() {}
    @Callable func start_gyro_updates() {
        update_failed.emit("CMMotionManager is not available on this platform.")
    }
    @Callable func stop_gyro_updates() {}
    @Callable func start_magnetometer_updates() {
        update_failed.emit("CMMotionManager is not available on this platform.")
    }
    @Callable func stop_magnetometer_updates() {}
    @Callable func start_device_motion_updates(referenceFrame: AttitudeReferenceFrame = .XARBITRARY_Z_VERTICAL) {
        update_failed.emit("CMMotionManager is not available on this platform.")
    }
    @Callable func stop_device_motion_updates() {}
}

#endif
