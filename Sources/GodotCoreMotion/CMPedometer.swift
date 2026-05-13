//
//  CMPedometer.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

@Godot
class CMPedometerData: RefCounted, @unchecked Sendable {
    @Export var startDate: Double = 0
    @Export var endDate: Double = 0
    @Export var numberOfSteps: Int = 0
    @Export var distance: Double = 0
    @Export var floorsAscended: Int = 0
    @Export var floorsDescended: Int = 0
    @Export var currentPace: Double = 0
    @Export var currentCadence: Double = 0
    @Export var averageActivePace: Double = 0

    #if canImport(CoreMotion) && os(iOS)
    convenience init(data: CoreMotion.CMPedometerData) {
        self.init()
        startDate = data.startDate.timeIntervalSince1970
        endDate = data.endDate.timeIntervalSince1970
        numberOfSteps = data.numberOfSteps.intValue
        distance = data.distance?.doubleValue ?? -1
        floorsAscended = data.floorsAscended?.intValue ?? -1
        floorsDescended = data.floorsDescended?.intValue ?? -1
        currentPace = data.currentPace?.doubleValue ?? -1
        currentCadence = data.currentCadence?.doubleValue ?? -1
        averageActivePace = data.averageActivePace?.doubleValue ?? -1
    }
    #endif
}

#if canImport(CoreMotion) && os(iOS)
import CoreMotion

@Godot
class CMPedometer: RefCounted, @unchecked Sendable {
    @Signal("data") var pedometer_updated: SignalWithArguments<CMPedometerData>
    @Signal("message") var pedometer_failed: SignalWithArguments<String>

    private let pedometer = CoreMotion.CMPedometer()

    @Callable static func is_step_counting_available() -> Bool {
        CoreMotion.CMPedometer.isStepCountingAvailable()
    }
    @Callable static func is_distance_available() -> Bool {
        CoreMotion.CMPedometer.isDistanceAvailable()
    }
    @Callable static func is_floor_counting_available() -> Bool {
        CoreMotion.CMPedometer.isFloorCountingAvailable()
    }
    @Callable static func is_pace_available() -> Bool {
        CoreMotion.CMPedometer.isPaceAvailable()
    }
    @Callable static func is_cadence_available() -> Bool {
        CoreMotion.CMPedometer.isCadenceAvailable()
    }
    @Callable static func authorization_status() -> Int {
        CoreMotion.CMPedometer.authorizationStatus().rawValue
    }

    @Callable
    func query_pedometer_data(fromUnixTime: Double, toUnixTime: Double, callback: Callable) {
        let from = Date(timeIntervalSince1970: fromUnixTime)
        let to = Date(timeIntervalSince1970: toUnixTime)
        pedometer.queryPedometerData(from: from, to: to) { data, error in
            let wrapped = data.map { CMPedometerData(data: $0) }
            let mappedError = mapError(error)
            DispatchQueue.main.async {
                if let wrapped {
                    _ = callback.call(Variant(wrapped), mappedError)
                } else {
                    _ = callback.call(nil, mappedError)
                }
            }
        }
    }

    @Callable
    func start_updates(fromUnixTime: Double) {
        let from = Date(timeIntervalSince1970: fromUnixTime)
        pedometer.startUpdates(from: from) { [weak self] data, error in
            guard let self else { return }
            let wrapped = data.map { CMPedometerData(data: $0) }
            let errorMessage = error?.localizedDescription
            DispatchQueue.main.async {
                if let errorMessage {
                    self.pedometer_failed.emit(errorMessage)
                    return
                }
                guard let wrapped else { return }
                self.pedometer_updated.emit(wrapped)
            }
        }
    }

    @Callable
    func stop_updates() {
        pedometer.stopUpdates()
    }
}

#else

@Godot
class CMPedometer: RefCounted, @unchecked Sendable {
    @Signal("data") var pedometer_updated: SignalWithArguments<CMPedometerData>
    @Signal("message") var pedometer_failed: SignalWithArguments<String>

    @Callable static func is_step_counting_available() -> Bool { false }
    @Callable static func is_distance_available() -> Bool { false }
    @Callable static func is_floor_counting_available() -> Bool { false }
    @Callable static func is_pace_available() -> Bool { false }
    @Callable static func is_cadence_available() -> Bool { false }
    @Callable static func authorization_status() -> Int { 0 }

    @Callable
    func query_pedometer_data(fromUnixTime: Double, toUnixTime: Double, callback: Callable) {
        _ = callback.call(nil, Variant("CMPedometer is not available on this platform."))
    }

    @Callable
    func start_updates(fromUnixTime: Double) {
        pedometer_failed.emit("CMPedometer is not available on this platform.")
    }

    @Callable
    func stop_updates() {}
}

#endif
