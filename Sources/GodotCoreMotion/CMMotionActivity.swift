//
//  CMMotionActivity.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

@Godot
class CMMotionActivity: RefCounted, @unchecked Sendable {
    enum Confidence: Int, CaseIterable {
        case LOW = 0
        case MEDIUM = 1
        case HIGH = 2
    }

    @Export var startDate: Double = 0
    @Export(.enum) var confidence: Confidence = .LOW
    @Export var stationary: Bool = false
    @Export var walking: Bool = false
    @Export var running: Bool = false
    @Export var automotive: Bool = false
    @Export var cycling: Bool = false
    @Export var unknown: Bool = false

    #if canImport(CoreMotion) && os(iOS)
    convenience init(activity: CoreMotion.CMMotionActivity) {
        self.init()
        startDate = activity.startDate.timeIntervalSince1970
        switch activity.confidence {
        case .low: confidence = .LOW
        case .medium: confidence = .MEDIUM
        case .high: confidence = .HIGH
        @unknown default: confidence = .LOW
        }
        stationary = activity.stationary
        walking = activity.walking
        running = activity.running
        automotive = activity.automotive
        cycling = activity.cycling
        unknown = activity.unknown
    }
    #endif
}

#if canImport(CoreMotion) && os(iOS)
import CoreMotion

@Godot
class CMMotionActivityManager: RefCounted, @unchecked Sendable {
    @Signal("activity") var activity_updated: SignalWithArguments<CMMotionActivity>
    @Signal("message") var activity_failed: SignalWithArguments<String>

    private let manager = CoreMotion.CMMotionActivityManager()
    private let updateQueue: OperationQueue = {
        let q = OperationQueue()
        q.qualityOfService = .userInitiated
        q.maxConcurrentOperationCount = 1
        return q
    }()

    @Callable static func is_activity_available() -> Bool {
        CoreMotion.CMMotionActivityManager.isActivityAvailable()
    }

    @Callable static func authorization_status() -> Int {
        CoreMotion.CMMotionActivityManager.authorizationStatus().rawValue
    }

    @Callable
    func query_activity(fromUnixTime: Double, toUnixTime: Double, callback: Callable) {
        let from = Date(timeIntervalSince1970: fromUnixTime)
        let to = Date(timeIntervalSince1970: toUnixTime)
        manager.queryActivityStarting(from: from, to: to, to: updateQueue) { activities, error in
            DispatchQueue.main.async {
                let array = VariantArray()
                if let activities {
                    for activity in activities {
                        array.append(Variant(CMMotionActivity(activity: activity)))
                    }
                }
                _ = callback.call(Variant(array), mapError(error))
            }
        }
    }

    @Callable
    func start_activity_updates() {
        manager.startActivityUpdates(to: updateQueue) { [weak self] activity in
            guard let self else { return }
            DispatchQueue.main.async {
                guard let activity else { return }
                self.activity_updated.emit(CMMotionActivity(activity: activity))
            }
        }
    }

    @Callable
    func stop_activity_updates() {
        manager.stopActivityUpdates()
    }
}

#else

@Godot
class CMMotionActivityManager: RefCounted, @unchecked Sendable {
    @Signal("activity") var activity_updated: SignalWithArguments<CMMotionActivity>
    @Signal("message") var activity_failed: SignalWithArguments<String>

    @Callable static func is_activity_available() -> Bool { false }
    @Callable static func authorization_status() -> Int { 0 }

    @Callable
    func query_activity(fromUnixTime: Double, toUnixTime: Double, callback: Callable) {
        _ = callback.call(Variant(VariantArray()), Variant("CMMotionActivityManager is not available on this platform."))
    }

    @Callable
    func start_activity_updates() {
        activity_failed.emit("CMMotionActivityManager is not available on this platform.")
    }

    @Callable
    func stop_activity_updates() {}
}

#endif
