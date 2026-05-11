import GameKit
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
    import UIKit
#else
    import AppKit
#endif

@Godot
class GKTurnBasedMatchmakerViewController: RefCounted, @unchecked Sendable {
    enum MatchmakingMode: Int, CaseIterable {
        case DEFAULT
        case NEARBY_ONLY
        case AUTOMATCH_ONLY
        case INVITE_ONLY

        func toGameKit() -> GameKit.GKMatchmakingMode {
            switch self {
            case .DEFAULT: return .default
            case .NEARBY_ONLY: return .nearbyOnly
            case .AUTOMATCH_ONLY: return .automatchOnly
            case .INVITE_ONLY: return .inviteOnly
            }
        }

        static func from(_ mode: GameKit.GKMatchmakingMode) -> MatchmakingMode {
            switch mode {
            case .default: return .DEFAULT
            case .nearbyOnly: return .NEARBY_ONLY
            case .automatchOnly: return .AUTOMATCH_ONLY
            case .inviteOnly: return .INVITE_ONLY
            @unknown default: return .DEFAULT
            }
        }
    }

    class Proxy: NSObject, GameKit.GKTurnBasedMatchmakerViewControllerDelegate {
        weak var base: GKTurnBasedMatchmakerViewController?

        init(_ base: GKTurnBasedMatchmakerViewController) {
            self.base = base
        }

        func turnBasedMatchmakerViewControllerWasCancelled(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController
        ) {
            guard let base else { return }
            MainActor.assumeIsolated {
                base.dismiss(viewController)
                base.cancelled.emit("")
            }
        }

        func turnBasedMatchmakerViewController(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController,
            didFailWithError error: any Error
        ) {
            guard let base else { return }
            MainActor.assumeIsolated {
                base.dismiss(viewController)
                base.failed_with_error.emit(String(describing: error))
            }
        }

        func turnBasedMatchmakerViewController(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController,
            didFind match: GameKit.GKTurnBasedMatch
        ) {
            base?.did_find_match.emit(GKTurnBasedMatch(match: match))
        }

        func turnBasedMatchmakerViewController(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController,
            playerQuitFor match: GameKit.GKTurnBasedMatch
        ) {
            base?.player_quit_for_match.emit(GKTurnBasedMatch(match: match))
        }
    }

    @Signal("detail") var cancelled: SignalWithArguments<String>
    @Signal("message") var failed_with_error: SignalWithArguments<String>
    @Signal("match") var did_find_match: SignalWithArguments<GKTurnBasedMatch>
    @Signal("match") var player_quit_for_match: SignalWithArguments<GKTurnBasedMatch>

    var vc: GameKit.GKTurnBasedMatchmakerViewController?
    var proxy: Proxy?

    #if os(macOS)
        var dialogController: GKDialogController?
    #endif

    @Export var showExistingMatches: Bool {
        get {
            MainActor.assumeIsolated {
                vc?.showExistingMatches ?? true
            }
        }
        set {
            MainActor.assumeIsolated {
                vc?.showExistingMatches = newValue
            }
        }
    }

    @Export(.enum) var matchmakingMode: MatchmakingMode {
        get {
            MainActor.assumeIsolated {
                if #available(iOS 15.0, macOS 12.0, tvOS 15.0, visionOS 1.0, *),
                    let vc
                {
                    return MatchmakingMode.from(vc.matchmakingMode)
                }
                return .DEFAULT
            }
        }
        set {
            MainActor.assumeIsolated {
                if #available(iOS 15.0, macOS 12.0, tvOS 15.0, visionOS 1.0, *),
                    let vc
                {
                    vc.matchmakingMode = newValue.toGameKit()
                }
            }
        }
    }

    @MainActor
    private func dismiss(_ controller: GameKit.GKTurnBasedMatchmakerViewController) {
        #if os(iOS)
            controller.dismiss(animated: true)
        #else
            dialogController?.dismiss(controller)
        #endif
    }

    @Callable
    static func create_controller(request: GKMatchRequest) -> GKTurnBasedMatchmakerViewController {
        MainActor.assumeIsolated {
            let vc = GameKit.GKTurnBasedMatchmakerViewController(matchRequest: request.request)
            let wrapper = GKTurnBasedMatchmakerViewController()
            let proxy = Proxy(wrapper)

            wrapper.vc = vc
            wrapper.proxy = proxy
            vc.turnBasedMatchmakerDelegate = proxy

            return wrapper
        }
    }

    class RequestMatchDelegate: NSObject, GameKit.GKTurnBasedMatchmakerViewControllerDelegate,
        @unchecked Sendable
    {
        #if os(macOS)
            var dialogController: GKDialogController?
        #endif

        private let callback: Callable
        let done: () -> Void

        init(_ callback: Callable, done: @escaping () -> Void = {}) {
            self.callback = callback
            self.done = done
        }

        func turnBasedMatchmakerViewController(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController,
            didFind match: GameKit.GKTurnBasedMatch
        ) {
            _ = callback.call(Variant(GKTurnBasedMatch(match: match)), nil)
        }

        func turnBasedMatchmakerViewControllerWasCancelled(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController
        ) {
            MainActor.assumeIsolated {
                #if os(iOS)
                    viewController.dismiss(animated: true)
                #else
                    dialogController?.dismiss(viewController)
                #endif
                let error = NSError(
                    domain: GKErrorDomain,
                    code: GKError.Code.CANCELLED.rawValue,
                    userInfo: [NSLocalizedDescriptionKey: "Cancelled"]
                )
                _ = callback.call(nil, GKError.from(error))
                done()
            }
        }

        func turnBasedMatchmakerViewController(
            _ viewController: GameKit.GKTurnBasedMatchmakerViewController,
            didFailWithError error: any Error
        ) {
            MainActor.assumeIsolated {
                #if os(iOS)
                    viewController.dismiss(animated: true)
                #else
                    dialogController?.dismiss(viewController)
                #endif
                _ = callback.call(nil, GKError.from(error))
                done()
            }
        }
    }

    @Callable
    static func request_match(request: GKMatchRequest, callback: Callable) {
        MainActor.assumeIsolated {
            let vc = GameKit.GKTurnBasedMatchmakerViewController(matchRequest: request.request)
            var hold: RequestMatchDelegate?
            hold = RequestMatchDelegate(callback, done: { hold = nil })
            vc.turnBasedMatchmakerDelegate = hold

            present(controller: vc) {
                #if os(macOS)
                    hold?.dialogController = $0 as? GKDialogController
                #endif
            }
        }
    }

    @Callable
    func present() {
        guard let vc else { return }

        MainActor.assumeIsolated {
            Self.present(controller: vc) { tracker in
                #if os(macOS)
                    self.dialogController = tracker as? GKDialogController
                #endif
            }
        }
    }

    static func present(
        controller: GameKit.GKTurnBasedMatchmakerViewController,
        track: @MainActor (AnyObject) -> Void
    ) {
        MainActor.assumeIsolated {
            #if os(iOS)
                presentOnTop(controller)
            #else
                let dialogController = GKDialogController.shared()
                dialogController.parentWindow = NSApplication.shared.mainWindow
                dialogController.present(controller)
                track(dialogController)
            #endif
        }
    }
}
