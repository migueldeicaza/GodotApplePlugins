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
            GD.print("GKMVC: cancelled")
        }
        
        func matchmakerViewController(_ viewController: GameKit.GKMatchmakerViewController, didFailWithError error: any Error) {
            GD.print("GKMVC: didFailWithError")
        }

        weak var base: GKMatchmakerViewController?
        init(_ base: GKMatchmakerViewController) {
            self.base = base
        }
    }

    var vc: GameKit.GKMatchmakerViewController?
    var proxy: Proxy?

    /// Returns a view controller for the specified request, configure the various callbacks, and then
    /// call present
    @Callable static func request(request: GKMatchRequest) -> GKMatchmakerViewController? {
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

    @Callable func present() {
        guard let vc else {
            return
        }
        presentOnTop(vc)
    }
}
