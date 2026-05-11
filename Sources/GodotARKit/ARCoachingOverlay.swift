//
//  ARCoachingOverlay.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit
import UIKit

@Godot
class ARCoachingOverlay: RefCounted, @unchecked Sendable {
    enum Goal: Int, CaseIterable {
        case TRACKING = 0
        case HORIZONTAL_PLANE = 1
        case VERTICAL_PLANE = 2
        case ANY_PLANE = 3
        case GEO_TRACKING = 4
    }

    @Export(.enum) var goal: Goal = .TRACKING {
        didSet {
            overlayView?.goal = goal.toNative()
        }
    }

    @Export var activatesAutomatically: Bool = true {
        didSet {
            overlayView?.activatesAutomatically = activatesAutomatically
        }
    }

    @Signal var coaching_did_deactivate: SimpleSignal

    private var overlayView: ARCoachingOverlayView?
    private var delegateProxy: Proxy?

    class Proxy: NSObject, ARCoachingOverlayViewDelegate {
        weak var base: ARCoachingOverlay?

        init(_ base: ARCoachingOverlay) {
            self.base = base
        }

        func coachingOverlayViewDidDeactivate(_ coachingOverlayView: ARCoachingOverlayView) {
            DispatchQueue.main.async { [weak base] in
                base?.coaching_did_deactivate.emit()
            }
        }
    }

    @Callable
    func attach_to_session(session: ARSession) {
        DispatchQueue.main.async { [weak self] in
            guard let self else { return }
            let overlay = ARCoachingOverlayView()
            overlay.session = session.session
            overlay.goal = self.goal.toNative()
            overlay.activatesAutomatically = self.activatesAutomatically

            let proxy = Proxy(self)
            overlay.delegate = proxy
            self.delegateProxy = proxy
            self.overlayView = overlay

            overlay.translatesAutoresizingMaskIntoConstraints = false
            if let window = UIApplication.shared.connectedScenes
                .compactMap({ $0 as? UIWindowScene })
                .flatMap({ $0.windows })
                .first(where: { $0.isKeyWindow }),
               let rootView = window.rootViewController?.view
            {
                rootView.addSubview(overlay)
                NSLayoutConstraint.activate([
                    overlay.topAnchor.constraint(equalTo: rootView.topAnchor),
                    overlay.leadingAnchor.constraint(equalTo: rootView.leadingAnchor),
                    overlay.trailingAnchor.constraint(equalTo: rootView.trailingAnchor),
                    overlay.bottomAnchor.constraint(equalTo: rootView.bottomAnchor),
                ])
            }
        }
    }

    @Callable
    func set_active(active: Bool, animated: Bool) {
        DispatchQueue.main.async { [weak self] in
            self?.overlayView?.setActive(active, animated: animated)
        }
    }
}

private extension ARCoachingOverlay.Goal {
    func toNative() -> ARCoachingOverlayView.Goal {
        switch self {
        case .TRACKING: return .tracking
        case .HORIZONTAL_PLANE: return .horizontalPlane
        case .VERTICAL_PLANE: return .verticalPlane
        case .ANY_PLANE: return .anyPlane
        case .GEO_TRACKING:
            if #available(iOS 14.0, *) {
                return .geoTracking
            }
            return .tracking
        }
    }
}

#elseif canImport(ARKit) && (os(visionOS) || os(macOS))

// ARCoachingOverlayView is an iOS-only UIKit view. visionOS has its own system-
// provided onboarding UX for spatial experiences, and macOS remote sessions have
// no equivalent. This stub exists for API surface consistency only.

@Godot
class ARCoachingOverlay: RefCounted, @unchecked Sendable {
    enum Goal: Int, CaseIterable {
        case TRACKING = 0
        case HORIZONTAL_PLANE = 1
        case VERTICAL_PLANE = 2
        case ANY_PLANE = 3
        case GEO_TRACKING = 4
    }

    @Export(.enum) var goal: Goal = .TRACKING
    @Export var activatesAutomatically: Bool = true

    @Signal var coaching_did_deactivate: SimpleSignal

    @Callable func set_active(active: Bool, animated: Bool) {}
    @Callable func attach_to_session(session: ARSession) {}
}

#endif
