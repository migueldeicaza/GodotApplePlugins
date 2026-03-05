//
//  GKAccessPoint.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/30/25.
//

@preconcurrency import SwiftGodotRuntime
import GameKit
#if canImport(UIKit)
import UIKit
#elseif canImport(AppKit)
import AppKit
#endif

@Godot
public class GKAccessPoint: RefCounted, @unchecked Sendable {
    // Wrapper for GKAccessPoint.Location
    public enum Location: Int, CaseIterable {
        case TOP_LEADING = 0
        case TOP_TRAILING = 1
        case BOTTOM_LEADING = 2
        case BOTTOM_TRAILING = 3
    }
    
    private var shared: GameKit.GKAccessPoint {
        return GameKit.GKAccessPoint.shared
    }

    // MARK: - Properties

    @Export
    var active: Bool {
        get { shared.isActive }
        set { shared.isActive = newValue }
    }

    @Export
    var visible: Bool {
        get { shared.isVisible }
    }
    
    @Export
    var isPresentingGameCenter: Bool {
        get { shared.isPresentingGameCenter }
    }
    
    @Export
    public var showHighlights: Bool {
        get { shared.showHighlights }
        set { shared.showHighlights = newValue }
    }
    
    @Export
    var location: Location {
        get {
            switch shared.location {
            case .topLeading: return .TOP_LEADING
            case .topTrailing: return .TOP_TRAILING
            case .bottomLeading: return .BOTTOM_LEADING
            case .bottomTrailing: return .BOTTOM_TRAILING
            @unknown default: return .TOP_LEADING
            }
        }
        set {
            switch newValue {
            case .TOP_LEADING: shared.location = .topLeading
            case .TOP_TRAILING: shared.location = .topTrailing
            case .BOTTOM_LEADING: shared.location = .bottomLeading
            case .BOTTOM_TRAILING: shared.location = .bottomTrailing
            }
        }
    }
    
    @Export
    var frameInScreenCoordinates: Rect2 {
        get {
            let rect = shared.frameInScreenCoordinates
            return Rect2(
                x: Float(rect.origin.x),
                y: Float(rect.origin.y),
                width: Float(rect.width),
                height: Float(rect.height)
            )
        }
    }

    @Export
    var frameInUnitCoordinates: Rect2 {
        get {
            #if os(visionOS)
            return Rect2(x: 0, y: 0, width: 0, height: 0)
            #else
            let rect = shared.frameInScreenCoordinates

            #if os(macOS)
            guard let screenFrame = NSScreen.main?.frame else {
                return Rect2(x: 0, y: 0, width: 0, height: 0)
            }
            let screenWidth = screenFrame.width
            let screenHeight = screenFrame.height
            #else
            let screenBounds = UIScreen.main.bounds
            let screenWidth = screenBounds.width
            let screenHeight = screenBounds.height
            #endif

            guard screenWidth > 0, screenHeight > 0 else {
                return Rect2(x: 0, y: 0, width: 0, height: 0)
            }

            return Rect2(
                x: Float(rect.minX / screenWidth),
                y: Float(rect.minY / screenHeight),
                width: Float(rect.width / screenWidth),
                height: Float(rect.height / screenHeight)
            )
            #endif
        }
    }
    
    #if os(tvOS)
    @Export
    var focused: Bool {
        get { shared.isFocused }
        set { shared.isFocused = newValue }
    }
    #else
    @Export
    var focused: Bool {
        get { false }
        set { /* No-op on non-tvOS */ }
    }
    #endif

    // MARK: - Trigger Methods
    
    @Callable
    func trigger(done: Callable) {
        shared.trigger(handler: {
            _ = done.call()
        })
    }
    
    @Callable
    func trigger_with_state(state: GKGameCenterViewController.State, done: Callable) {
        shared.trigger(state: state.toGameKit(), handler: {
             _ = done.call()
            })
    }
    
    @Callable
    func trigger_with_achievement(achievementID: String, done: Callable) {
        if #available(macOS 15.0, iOS 18.0, tvOS 14.0, visionOS 1.0, *) {
             shared.trigger(achievementID: achievementID, handler: {
                 _ = done.call()
             })
        }
    }
    
    @Callable
    func trigger_with_leaderboard(leaderboardID: String, playerScope: Int, timeScope: Int, done: Callable) {
        if #available(macOS 15.0, iOS 18.0, tvOS 14.0, visionOS 1.0, *) {
            let pScope = GameKit.GKLeaderboard.PlayerScope(rawValue: playerScope) ?? .global
            let tScope = GameKit.GKLeaderboard.TimeScope(rawValue: timeScope) ?? .allTime
            shared.trigger(leaderboardID: leaderboardID, playerScope: pScope, timeScope: tScope, handler: {
                _ = done.call()
            })
        }
    }
    
    @Callable
    func trigger_with_leaderboard_set(leaderboardSetID: String, done: Callable) {
        if #available(macOS 15.0, iOS 18.0, tvOS 14.0, visionOS 1.0, *) {
            shared.trigger(leaderboardSetID: leaderboardSetID, handler: {
                _ = done.call()
            })
        }
    }
    
    @Callable
    func trigger_with_player(player: GKPlayer, done: Callable) {
        if #available(macOS 15.0, iOS 18.0, tvOS 14.0, visionOS 1.0, *) {
            shared.trigger(player: player.player, handler: {
                _ = done.call()
            })
        }
    }

    @Callable
    func trigger_for_play_together(done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *) {
            shared.triggerForPlayTogether(handler: {
                _ = done.call()
            })
        }
        #endif
    }

    @Callable
    func trigger_for_challenges(done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *) {
            shared.triggerForChallenges(handler: {
                _ = done.call()
            })
        }
        #endif
    }

    @Callable
    func trigger_with_challenge_definition_id(challengeDefinitionID: String, done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *) {
            shared.trigger(challengeDefinitionID: challengeDefinitionID, handler: {
                _ = done.call()
            })
        }
        #endif
    }

    @Callable
    func trigger_with_game_activity(gameActivity: GKGameActivity, done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = gameActivity.rawActivity as? GameKit.GKGameActivity
        {
            shared.trigger(gameActivity: activity, handler: {
                _ = done.call()
            })
        }
        #endif
    }

    @Callable
    func trigger_with_game_activity_definition_id(gameActivityDefinitionID: String, done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *) {
            shared.trigger(gameActivityDefinitionID: gameActivityDefinitionID, handler: {
                _ = done.call()
            })
        }
        #endif
    }

    @Callable
    func trigger_for_friending(done: Callable) {
        #if os(iOS) || os(macOS)
        if #available(iOS 26.0, macOS 26.0, *) {
            shared.triggerForFriending(handler: {
                _ = done.call()
            })
        }
        #endif
    }
    
}
