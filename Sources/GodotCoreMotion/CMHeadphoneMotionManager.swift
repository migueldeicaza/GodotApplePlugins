//
//  CMHeadphoneMotionManager.swift
//  GodotApplePlugins
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(CoreMotion) && (os(iOS) || os(macOS))
import CoreMotion

@Godot
class CMHeadphoneMotionManager: RefCounted, @unchecked Sendable {
    @Signal("deviceMotion") var device_motion_updated: SignalWithArguments<CMDeviceMotion>
    @Signal("message") var update_failed: SignalWithArguments<String>
    @Signal var connected: SimpleSignal
    @Signal var disconnected: SimpleSignal

    private var delegateProxy: Proxy?

    private lazy var manager: CoreMotion.CMHeadphoneMotionManager = {
        let m = CoreMotion.CMHeadphoneMotionManager()
        let proxy = Proxy(self)
        delegateProxy = proxy
        m.delegate = proxy
        return m
    }()

    private let updateQueue: OperationQueue = {
        let q = OperationQueue()
        q.qualityOfService = .userInitiated
        q.maxConcurrentOperationCount = 1
        return q
    }()

    class Proxy: NSObject, CMHeadphoneMotionManagerDelegate {
        weak var base: CMHeadphoneMotionManager?
        init(_ base: CMHeadphoneMotionManager) {
            self.base = base
        }

        func headphoneMotionManagerDidConnect(_ manager: CoreMotion.CMHeadphoneMotionManager) {
            DispatchQueue.main.async { [weak base] in
                base?.connected.emit()
            }
        }

        func headphoneMotionManagerDidDisconnect(_ manager: CoreMotion.CMHeadphoneMotionManager) {
            DispatchQueue.main.async { [weak base] in
                base?.disconnected.emit()
            }
        }
    }

    @Export var isDeviceMotionAvailable: Bool { manager.isDeviceMotionAvailable }
    @Export var isDeviceMotionActive: Bool { manager.isDeviceMotionActive }

    @Export var deviceMotion: CMDeviceMotion? {
        guard let motion = manager.deviceMotion else { return nil }
        return CMDeviceMotion(motion: motion)
    }

    @Callable static func authorization_status() -> Int {
        if #available(iOS 17.4, macOS 14.4, *) {
            return CoreMotion.CMHeadphoneMotionManager.authorizationStatus().rawValue
        }
        return 0
    }

    @Callable
    func start_device_motion_updates() {
        manager.startDeviceMotionUpdates(to: updateQueue) { [weak self] motion, error in
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
}

#else

@Godot
class CMHeadphoneMotionManager: RefCounted, @unchecked Sendable {
    @Signal("deviceMotion") var device_motion_updated: SignalWithArguments<CMDeviceMotion>
    @Signal("message") var update_failed: SignalWithArguments<String>
    @Signal var connected: SimpleSignal
    @Signal var disconnected: SimpleSignal

    @Export var isDeviceMotionAvailable: Bool = false
    @Export var isDeviceMotionActive: Bool = false
    @Export var deviceMotion: CMDeviceMotion?

    @Callable static func authorization_status() -> Int { 0 }

    @Callable
    func start_device_motion_updates() {
        update_failed.emit("CMHeadphoneMotionManager is not available on this platform.")
    }

    @Callable
    func stop_device_motion_updates() {}
}

#endif
