//
//  GKMatchMakerViewController.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/17/25.
//
@preconcurrency import SwiftGodotRuntime
import SwiftUI
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif
import GameKit

@Godot
class GKMatchmakerViewController: RefCounted, @unchecked Sendable {
    class Proxy: NSObject, GameKit.GKMatchmakerViewControllerDelegate, GKLocalPlayerListener {
        func matchmakerViewControllerWasCancelled(_ viewController: GameKit.GKMatchmakerViewController) {
            guard let base else { return }
            MainActor.assumeIsolated {
#if os(macOS)
                base.dialogController?.dismiss(viewController)
#else
                viewController.dismiss(animated: true)
                base.cancelled.emit("")
#endif
            }
        }

        func matchmakerViewController(_ viewController: GameKit.GKMatchmakerViewController, didFailWithError error: any Error) {
            GD.print("GKMVC: didFailWithError")
            base?.failed_with_error.emit(String(describing: error))
        }

        func matchmakerViewController(_ viewController: GameKit.GKMatchmakerViewController, didFind match: GameKit.GKMatch) {
            base?.did_find_match.emit(GKMatch(match: match))
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            didFindHostedPlayers players: [GameKit.GKPlayer]
        ) {
            let result = VariantArray()
            for player in players {
                result.append(Variant(GKPlayer(player: player)))
            }
            base?.did_find_hosted_players.emit(result)
        }

        weak var base: GKMatchmakerViewController?
        init(_ base: GKMatchmakerViewController) {
            self.base = base
        }
    }

    @Signal var cancelled: SignalWithArguments<String>

    /// Matchmaking has failed with an error
    @Signal var failed_with_error: SignalWithArguments<String>

    /// A peer-to-peer match has been found, the game should start
    @Signal var did_find_match: SignalWithArguments<GKMatch>

    /// Players have been found for a server-hosted game, the game should start, receives an array of GKPlayers
    @Signal var did_find_hosted_players: SignalWithArguments<VariantArray>

    /// The view controller if we create it
    var vc: GameKit.GKMatchmakerViewController?
    /// Delegate class if the user is rolling his own
    var proxy: Proxy?

#if os(macOS)
    /// When the user triggers the presentation, on macOS, we keep track of it
    var dialogController: GKDialogController? = nil
#endif

    /// Returns a view controller for the specified request, configure the various callbacks, and then
    /// call `present` on it.
    @Callable static func create_controller(request: GKMatchRequest) -> GKMatchmakerViewController? {
        MainActor.assumeIsolated {
            if let vc = GameKit.GKMatchmakerViewController(matchRequest: request.request) {
                let v = GKMatchmakerViewController()
                let proxy = Proxy(v)

                v.vc = vc
                v.proxy = proxy

                vc.matchmakerDelegate = proxy
                return v
            }
            return nil
        }
    }

    // This is used for the custom request that is vastly simpler than rolling your own
    class RequestMatchDelegate: NSObject, GameKit.GKMatchmakerViewControllerDelegate, @unchecked Sendable {
#if os(macOS)
        var dialogController: GKDialogController?
#endif
        private let callback: Callable
        let done: () -> ()
        init(_ callback: Callable, done: @escaping () -> () = { }) {
            self.callback = callback
            self.done = done
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            didFind match: GameKit.GKMatch
        ) {
            _ = self.callback.call(Variant(GKMatch(match: match)), nil)
        }

        func matchmakerViewControllerWasCancelled(
            _ source: GameKit.GKMatchmakerViewController
        ) {
            MainActor.assumeIsolated {
#if os(iOS)
                source.dismiss(animated: true)
#else
                dialogController?.dismiss(source)

#endif
                _ = self.callback.call(nil, Variant("cancelled"))
            }
        }

        func matchmakerViewController(
            _ source: GameKit.GKMatchmakerViewController,
            didFailWithError: (any Error)
        ) {
            _ = self.callback.call(nil, Variant(didFailWithError.localizedDescription))
        }
    }

    /// Convenience method that is a version that sets up `create_controller` and calls the callback
    /// with two arguments, the first is the match on success, and the second is an error on failure, which can be
    /// one of the following strings: "cancelled",
    @Callable static func request_match(request: GKMatchRequest, callback: Callable) {
        MainActor.assumeIsolated {
            if let vc = GameKit.GKMatchmakerViewController(matchRequest: request.request) {
                var hold: RequestMatchDelegate?

                hold = RequestMatchDelegate(callback, done: {
                    hold = nil
                })
                vc.matchmakerDelegate = hold
                GKMatchmakerViewController.present(controller: vc) {
#if os(macOS)
                    hold?.dialogController = $0 as? GKDialogController
#endif
                }
            }
        }
    }

    @Callable func present() {
        guard let vc else {
            return
        }
        GKMatchmakerViewController.present(controller: vc) { v in
#if os(macOS)
            dialogController = v as? GKDialogController
#endif
        }
    }

    static func present(controller: GameKit.GKMatchmakerViewController, track: @MainActor (AnyObject) -> ()) {
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
