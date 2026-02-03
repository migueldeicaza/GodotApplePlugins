//
//  GameCenter.swift
//  SwiftGodotAppleTemplate
//
//  Created by Miguel de Icaza on 11/15/25.
//

import GameKit
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
    import UIKit
#else
    import AppKit
#endif

@Godot
class GameCenterManager: RefCounted, @unchecked Sendable {
    @Signal("message") var authentication_error: SignalWithArguments<String>
    @Signal("status") var authentication_result: SignalWithArguments<Bool>

    var isAuthenticated: Bool = false

    @Export var localPlayer: GKLocalPlayer
    @Export var accessPoint: GKAccessPoint {
        GKAccessPoint()
    }

    required init(_ context: InitContext) {
        localPlayer = GKLocalPlayer()
        super.init(context)
    }

    @Callable
    func authenticate() {
        let localPlayer = GameKit.GKLocalPlayer.local
        localPlayer.authenticateHandler = { viewController, error in
            MainActor.assumeIsolated {
                if let vc = viewController {
                    presentOnTop(vc)
                    return
                }

                if let error = error {
                    self.authentication_error.emit(String(describing: error))
                }
                self.isAuthenticated = GameKit.GKLocalPlayer.local.isAuthenticated
                self.authentication_result.emit(self.isAuthenticated)
            }
        }
    }
}

//#if standalone
//#initSwiftExtension(cdecl: "godot_game_center_init", types: [
//    GameCenterManager.self,
//    GKAccessPoint.self,
//    GKAchievement.self,
//    GKAchievementDescription.self,
//    GKLocalPlayer.self,
//    GKLeaderboard.self,
//    GKLeaderboardSet.self,
//    GKMatch.self,
//    GKMatchmakerViewController.self,
//    GKMatchRequest.self,
//    GKPlayer.self,
//])
//#endif
