//
//  GKGameCenterViewController.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/2/25.
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
class GKGameCenterViewController: RefCounted, @unchecked Sendable {
    class Delegate: NSObject, GameKit.GKGameCenterControllerDelegate {
        func gameCenterViewControllerDidFinish(_ gameCenterViewController: GameKit.GKGameCenterViewController) {
#if os(iOS)
            gameCenterViewController.dismiss(animated: true)
#else
            dialogController?.dismiss(gameCenterViewController)

#endif
            done()
        }
        
#if os(macOS)
        var dialogController: GKDialogController?
#endif
        var done: () -> ()

        init(done: @escaping () -> ()) {
            self.done = done
        }
    }

    enum State: Int, CaseIterable {
        case defaultScreen
        case leaderboards
        case achievements
        case localPlayerProfile
        case dashboard
        case localPlayerFriendsList

        func toGameKit() -> GameKit.GKGameCenterViewControllerState {
            switch self {
                case .defaultScreen:
                return .default
            case .leaderboards:
                return .leaderboards
            case .achievements:
                return .achievements
            case .localPlayerProfile:
                return .localPlayerProfile
            case .dashboard:
                return .dashboard
            case .localPlayerFriendsList:
                return .localPlayerFriendsList
            }
        }
    }

    /// Returns a view controller for the specified type, which you can then call present on
    @Callable static func show_type(_ type: State) {
        MainActor.assumeIsolated {
            let vc = GameKit.GKGameCenterViewController(state: type.toGameKit())
            show(vc)
        }
    }

    @Callable static func show_leaderboard(leaderboard: GKLeaderboard, scope: GKLeaderboard.PlayerScope) {
        MainActor.assumeIsolated {
            let vc = GameKit.GKGameCenterViewController(leaderboard: leaderboard.board, playerScope: scope.toGameKit())
            show(vc)
        }
    }

    @Callable static func show_leaderboard_time_period(id: String, scope: GKLeaderboard.PlayerScope, timeScope: GKLeaderboard.TimeScope) {
        MainActor.assumeIsolated {
            let vc = GameKit.GKGameCenterViewController(leaderboardID: id, playerScope: scope.toGameKit(), timeScope: timeScope.toGameKit())
            show(vc)
        }
    }

    @Callable static func show_leaderboardset(id: String) {
        if #available(iOS 18.0, macOS 15.0, *) {
            MainActor.assumeIsolated {
                let vc = GameKit.GKGameCenterViewController(leaderboardSetID: id)
                show(vc)
            }
        }
    }

    @Callable static func show_achievement(id: String) {
        MainActor.assumeIsolated {
            let vc = GameKit.GKGameCenterViewController(achievementID: id)
            show(vc)
        }
    }

    @Callable static func show_player(player: GKPlayer) {
        if #available(iOS 18.0, macOS 15.0, *) {
            MainActor.assumeIsolated {
                let vc = GameKit.GKGameCenterViewController(player: player.player)
                show(vc)
            }
        }
    }

    @MainActor
    static func show(_ controller: GameKit.GKGameCenterViewController) {
        var hold: Delegate?
        hold = Delegate {
            hold = nil
        }
        controller.gameCenterDelegate = hold
        present(controller: controller) {
#if os(macOS)
            hold?.dialogController = $0 as? GKDialogController
#endif
        }
    }

    @MainActor
    static func present(controller: GameKit.GKGameCenterViewController, track: @MainActor (AnyObject) -> ()) {
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
