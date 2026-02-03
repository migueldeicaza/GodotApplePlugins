import GameKit
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

@Godot
class GKLeaderboard: RefCounted, @unchecked Sendable {
    var board: GameKit.GKLeaderboard = GameKit.GKLeaderboard()

    enum AppleLeaderboardType: Int, CaseIterable {
        case CLASSIC
        case RECURRING
        case UNKNOWN
    }

    enum TimeScope: Int, CaseIterable {
        case TODAY
        case WEEK
        case ALL_TIME

        func toGameKit() -> GameKit.GKLeaderboard.TimeScope {
            switch self {
            case .TODAY: return .today
            case .WEEK: return .week
            case .ALL_TIME: return .allTime
            }
        }
    }

    enum PlayerScope: Int, CaseIterable {
        case GLOBAL
        case FRIENDS_ONLY

        func toGameKit() -> GameKit.GKLeaderboard.PlayerScope {
            switch self {
            case .GLOBAL: return .global
            case .FRIENDS_ONLY: return .friendsOnly
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
        case .classic: return .CLASSIC
        case .recurring: return .RECURRING
        default: return .UNKNOWN
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
    @Export var startDate: Double {
        board.startDate?.timeIntervalSince1970 ?? 0
    }
    @Export var nextStartDate: Double {
        board.startDate?.timeIntervalSince1970 ?? 0
    }
    @Export var duration: Double { board.duration }

    @Callable
    /// Callback is invoked with nil on success, or a string on error
    func submit_score(score: Int, context: Int, player: GKPlayer, callback: Callable) {
        board.submitScore(score, context: context, player: player.player) { error in
            _ = callback.call(GKError.from(error))
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
                _ = callback.call(nil, GKError.from(error))
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
            _ = callback.call(Variant(wrapped), GKError.from(error))
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
            _ = callback.call(Variant(le), re, Variant(range), GKError.from(error))
        } else {
            _ = callback.call(Variant(le), re, GKError.from(error))
        }
    }

    ///
    /// Returns the scores for the local player and other players for the specified time period.
    /// Calls the callback with:
    /// - Score for the local player (or nil if he does not have one)
    /// - Scores for the specified players
    /// - Error if not nil
    @Callable
    func load_entries(players: VariantArray, timeScope: GKLeaderboard.TimeScope, callback: Callable)
    {
        var gkPlayers: [GameKit.GKPlayer] = []
        for p in players {
            guard let p, let po = p.asObject(GKPlayer.self) else { continue }
            gkPlayers.append(po.player)
        }

        board.loadEntries(for: gkPlayers, timeScope: timeScope.toGameKit()) {
            local, requested, error in
            self.processEntries(
                callback: callback, local: local, requested: requested, error: error)
        }
    }

    @Callable
    func load_local_player_entries(
        playerScope: GKLeaderboard.PlayerScope, timeScope: GKLeaderboard.TimeScope, rangeStart: Int,
        rangeLength: Int, callback: Callable
    ) {
        let range = NSRange(location: rangeStart, length: rangeLength)
        board.loadEntries(
            for: playerScope.toGameKit(), timeScope: timeScope.toGameKit(), range: range
        ) { localPlayerEntry, entries, totalPlayerCount, error in
            self.processEntries(
                callback: callback, local: localPlayerEntry, requested: entries,
                range: totalPlayerCount, error: error)
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
    @Export var rank: Int { store?.rank ?? 0 }
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
