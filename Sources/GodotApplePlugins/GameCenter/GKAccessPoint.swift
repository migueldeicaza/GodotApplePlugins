//
//  GKAccessPoint.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/30/25.
//

@preconcurrency import SwiftGodotRuntime
import GameKit

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
    
}
