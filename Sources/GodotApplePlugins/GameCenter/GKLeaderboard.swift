//
//  AppleLeaderboard.swift
//  GodotApplePlugins
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

enum AppleLeaderboardType: Int, CaseIterable {
    case classic
    case recurring
    case unknown
}

@Godot
class GKLeaderboard: RefCounted, @unchecked Sendable {
    var board: GameKit.GKLeaderboard = GameKit.GKLeaderboard()

    enum TimeScope: Int, CaseIterable {
        case today
        case week
        case allTime

        func toGameKit() -> GameKit.GKLeaderboard.TimeScope {
            switch self {
            case .today: return .today
            case .week: return .week
            case .allTime: return .allTime
            }
        }
    }

    enum PlayerScope: Int, CaseIterable {
        case global
        case friendsOnly

        func toGameKit() -> GameKit.GKLeaderboard.PlayerScope {
            switch self {
            case .global: return .global
            case .friendsOnly: return .friendsOnly
            }
        }
    }

    convenience init(board: GameKit.GKLeaderboard) {
        self.init()
        self.board = board
    }

    @Export var title: String { board.title ?? "" }
    @Export(.enum) var type: AppleLeaderboardType {
        switch board.type {
        case .classic: return .classic
        case .recurring: return .recurring
        default: return .unknown
        }
    }

    @Export var groupIdentifier: String { board.groupIdentifier ?? "" }
    @Export var activityIdentifier: String {
        if #available(iOS 26.0, macOS 26.0, *) {
            return board.activityIdentifier
        } else {
            return ""
        }
    }
    @Export var activityProperties: VariantDictionary {
        if #available(iOS 26.0, macOS 26.0, *) {
            let result = VariantDictionary()
            for (key, value) in board.activityProperties {
                result[Variant(key)] = Variant(value)
            }
            return result
        } else {
            return VariantDictionary()
        }
    }
    // Not sure how to surface dates to Godot
    //@Export var startDate:
    //@Export var endDate:
    @Export var duration: Double { board.duration }

    @Callable
    /// Callback is invoked with nil on success, or a string on error
    func submit_score(score: Int, context: Int, player: GKPlayer, callback: Callable) {
        board.submitScore(score, context: context, player: player.player) { error in
            let result: Variant?
            if let error {
                result = Variant(error.localizedDescription)
            } else {
                result = nil
            }
            _ = callback.call(result)
        }
    }

    /// Loads the image for the leaderboard, the call back is invoked with two arguments
    /// a n Image as the first argument, an any error as the second.
    /// either one can be nil.
    @Callable()
    func load_image(callback: Callable) {
        board.loadImage { image, error in
            if let image, let godotImage = image.asGodotImage() {
                _ = callback.call(godotImage, nil)
            } else if let error {
                _ = callback.call(nil, Variant(error.localizedDescription))
            } else {
                _ = callback.call(nil, Variant("Could not load leaderboard image"))
            }
        }
    }

    /// Fetches the leaderboards and calls the provided callable with `Array[GKLeaderboard]` and an Variant that is null on success, or a string on error
    @Callable
    static func load_leaderboards(_ ids: PackedStringArray, callback: Callable) {
        var sids: [String]?
        if ids.count == 0 {
            sids = nil
        } else {
            var result: [String] = []
            for x in 0..<ids.count {
                result.append(ids[x])
            }
            sids = result
        }
        GameKit.GKLeaderboard.loadLeaderboards(IDs: sids) { result, error in
            let wrapped = TypedArray<GKLeaderboard?>()

            if let result {
                for l in result {
                    let wrap = GKLeaderboard(board: l)
                    wrapped.append(wrap)
                }
            }
            _ = callback.call(Variant(wrapped), error != nil ? Variant(String(describing: error)) : nil)
        }
    }

    func processEntries(
        callback: Callable,
        local: GameKit.GKLeaderboard.Entry?,
        requested: [GameKit.GKLeaderboard.Entry]?,
        range: Int? = nil,
        error: (any Error)?
    ) {

        let le: GKLeaderboardEntry?
        if let local {
            le = GKLeaderboardEntry(store: local)
        } else {
            le = nil
        }
        let re: Variant?
        if let requested {
            let arr = TypedArray<GKLeaderboardEntry?>()
            for x in requested {
                arr.append(GKLeaderboardEntry(store: x))
            }
            re = Variant(arr)
        } else {
            re = nil
        }
        if let range {
            _ = callback.call(Variant(le), re, Variant(range), mapError(error))
        } else {
            _ = callback.call(Variant(le), re, mapError(error))
        }
    }

    ///
    /// Returns the scores for the local player and other players for the specified time period.
    /// Calls the callback with:
    /// - Score for the local player (or nil if he does not have one)
    /// - Scores for the specified players
    /// - Error if not nil
    @Callable
    func load_entries(players: VariantArray, timeScope: GKLeaderboard.TimeScope, callback: Callable) {
        var gkPlayers: [GameKit.GKPlayer] = []
        for p in players {
            guard let p, let po = p.asObject(GKPlayer.self) else { continue }
            gkPlayers.append(po.player)
        }

        board.loadEntries(for: gkPlayers, timeScope: timeScope.toGameKit()) { local, requested, error in
            self.processEntries(callback: callback, local: local, requested: requested, error: error)
        }
    }
}

@Godot
class GKLeaderboardEntry: RefCounted, @unchecked Sendable {
    var store: GameKit.GKLeaderboard.Entry? = nil

    convenience init(store: GameKit.GKLeaderboard.Entry) {
        self.init()
        self.store = store
    }

    @Export var context: Int { store?.context ?? 0 }
    @Export var rank: Int { store?.context ?? 0 }
    @Export var score: Int { store?.score ?? 0 }
    @Export var formattedScore: String { store?.formattedScore ?? "" }
    @Export var player: GKPlayer? {
        if let p = store?.player {
            return GKPlayer(player: p)
        } else {
            return nil
        }
    }
}
