//
//  GameCenter.swift
//  SwiftGodotAppleTemplate
//
//  Created by Miguel de Icaza on 11/15/25.
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
class GameCenterManager: RefCounted, @unchecked Sendable {
    @Signal var authentication_error: SignalWithArguments<String>
    @Signal var authentication_result: SignalWithArguments<Bool>

    var isAuthenticated: Bool = false
    
    @Export var localPlayer: AppleLocalPlayer

    required init(_ context: InitContext) {
        localPlayer = AppleLocalPlayer()
        super.init(context)
    }

    @Callable
    func authenticate() {
        let localPlayer = GKLocalPlayer.local
        localPlayer.authenticateHandler = { viewController, error in
            GD.print("AppleLocalPlayer: authentication callback")
            MainActor.assumeIsolated {
                if let vc = viewController {
                    GD.print("Presenting VC")
                    presentOnTop(vc)
                    return
                }

                if let error = error {
                    GD.print("God an error: \(error)")
                    self.authentication_error.emit(String(describing: error))
                }
                GD.print("Raising events")
                self.isAuthenticated = GKLocalPlayer.local.isAuthenticated
                self.authentication_result.emit(self.isAuthenticated)
            }
        }
    }

    @Callable
    func load_leaderboards(_ ids: [String], callback: Callable) {
        GKLeaderboard.loadLeaderboards(IDs: ids.count == 0 ? nil : ids) { result, error in
            let wrapped = VariantArray()
            if let result {
                for l in result {
                    if let wrap = AppleLeaderboard(board: l) {
                        wrapped.append(Variant(wrap))
                    }
                }
            }
            _ = callback.call(Variant(wrapped))
        }
    }

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable()
    func report_achivement(achivements: VariantArray, callback: Callable) {
        var array: [GKAchievement] = []
        for va in achivements {
            guard let va else { continue }
            if let a = va.asObject(AppleAchievement.self) {
                array.append(a.achievement)
            }
        }
        GKAchievement.report(array) { error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else {
                _ = callback.call(nil)
            }
        }
    }

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable
    func reset_achivements(callback: Callable) {
        GKAchievement.resetAchievements { error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else {
                _ = callback.call(nil)
            }
        }
    }

    @Callable
    func load_achievement_descriptions(callback: Callable) {
        GKAchievementDescription.loadAchievementDescriptions { achievementDescriptions, error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else if let achievementDescriptions {
                let res = VariantArray()
                for ad in achievementDescriptions {
                    if let ad = AppleAchievementDescription(ad) {
                        res.append(Variant(ad))
                    }
                }
                _ = callback.call(Variant(res))
            } else {
                _ = callback.call(Variant(VariantArray()))
            }
        }
    }
}

#initSwiftExtension(cdecl: "godot_game_center_init", types: [
    GameCenterManager.self,
    AppleLocalPlayer.self,
    ApplePlayer.self,
    AppleLeaderboard.self,
    AppleAchievement.self,
    AppleAchievementDescription.self
])
