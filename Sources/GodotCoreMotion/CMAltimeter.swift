//
//  CMAltimeter.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

@Godot
class CMAltitudeData: RefCounted, @unchecked Sendable {
    @Export var timestamp: Double = 0
    @Export var relativeAltitude: Double = 0
    @Export var pressure: Double = 0

    #if canImport(CoreMotion) && os(iOS)
    convenience init(data: CoreMotion.CMAltitudeData) {
        self.init()
        timestamp = data.timestamp
        relativeAltitude = data.relativeAltitude.doubleValue
        pressure = data.pressure.doubleValue
    }
    #endif
}

@Godot
class CMAbsoluteAltitudeData: RefCounted, @unchecked Sendable {
    @Export var timestamp: Double = 0
    @Export var altitude: Double = 0
    @Export var accuracy: Double = 0
    @Export var precision: Double = 0
}

#if canImport(CoreMotion) && os(iOS)
import CoreMotion

@Godot
class CMAltimeter: RefCounted, @unchecked Sendable {
    @Signal("altitude") var relative_altitude_updated: SignalWithArguments<CMAltitudeData>
    @Signal("altitude") var absolute_altitude_updated: SignalWithArguments<CMAbsoluteAltitudeData>
    @Signal("message") var altimeter_failed: SignalWithArguments<String>

    private let altimeter = CoreMotion.CMAltimeter()
    private let updateQueue: OperationQueue = {
        let q = OperationQueue()
        q.qualityOfService = .userInitiated
        q.maxConcurrentOperationCount = 1
        return q
    }()

    @Callable static func is_relative_altitude_available() -> Bool {
        CoreMotion.CMAltimeter.isRelativeAltitudeAvailable()
    }
    @Callable static func is_absolute_altitude_available() -> Bool {
        if #available(iOS 15.0, *) {
            return CoreMotion.CMAltimeter.isAbsoluteAltitudeAvailable()
        }
        return false
    }
    @Callable static func authorization_status() -> Int {
        CoreMotion.CMAltimeter.authorizationStatus().rawValue
    }

    @Callable
    func start_relative_altitude_updates() {
        altimeter.startRelativeAltitudeUpdates(to: updateQueue) { [weak self] data, error in
            guard let self else { return }
            let wrapped = data.map { CMAltitudeData(data: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.altimeter_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.relative_altitude_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_relative_altitude_updates() {
        altimeter.stopRelativeAltitudeUpdates()
    }

    @Callable
    func start_absolute_altitude_updates() {
        guard #available(iOS 15.0, *) else {
            altimeter_failed.emit("Absolute altitude updates require iOS 15.0 or later.")
            return
        }
        altimeter.startAbsoluteAltitudeUpdates(to: updateQueue) { [weak self] data, error in
            guard let self else { return }
            let wrapped: CMAbsoluteAltitudeData? = data.map { sample in
                let w = CMAbsoluteAltitudeData()
                w.timestamp = sample.timestamp
                w.altitude = sample.altitude
                w.accuracy = sample.accuracy
                w.precision = sample.precision
                return w
            }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.altimeter_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.absolute_altitude_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_absolute_altitude_updates() {
        if #available(iOS 15.0, *) {
            altimeter.stopAbsoluteAltitudeUpdates()
        }
    }
}

#else

@Godot
class CMAltimeter: RefCounted, @unchecked Sendable {
    @Signal("altitude") var relative_altitude_updated: SignalWithArguments<CMAltitudeData>
    @Signal("altitude") var absolute_altitude_updated: SignalWithArguments<CMAbsoluteAltitudeData>
    @Signal("message") var altimeter_failed: SignalWithArguments<String>

    @Callable static func is_relative_altitude_available() -> Bool { false }
    @Callable static func is_absolute_altitude_available() -> Bool { false }
    @Callable static func authorization_status() -> Int { 0 }

    @Callable
    func start_relative_altitude_updates() {
        altimeter_failed.emit("CMAltimeter is not available on this platform.")
    }

    @Callable
    func stop_relative_altitude_updates() {}

    @Callable
    func start_absolute_altitude_updates() {
        altimeter_failed.emit("CMAltimeter is not available on this platform.")
    }

    @Callable
    func stop_absolute_altitude_updates() {}
}

#endif
