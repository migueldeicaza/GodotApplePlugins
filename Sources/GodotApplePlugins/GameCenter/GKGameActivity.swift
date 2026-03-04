//
//  GKGameActivity.swift
//  GodotApplePlugins
//

import GameKit
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

@available(iOS 26.0, macOS 26.0, *)
@Godot
class GKGameActivity: RefCounted, @unchecked Sendable {
    var activity: GameKit.GKGameActivity?

    convenience init(activity: GameKit.GKGameActivity) {
        self.init()
        self.activity = activity
    }

    private static func wrapAchievements(_ achievements: Set<GameKit.GKAchievement>?) -> VariantArray {
        let result = VariantArray()
        achievements?.forEach {
            result.append(Variant(GKAchievement(achievement: $0)))
        }
        return result
    }

    private static func wrapLeaderboardScores(_ scores: Set<GameKit.GKLeaderboardScore>?) -> VariantArray {
        let result = VariantArray()
        scores?.forEach {
            result.append(Variant(GKLeaderboardScore(score: $0)))
        }
        return result
    }

    private static func wrapPlayers(_ players: [GameKit.GKPlayer]?) -> TypedArray<GKPlayer?> {
        let result = TypedArray<GKPlayer?>()
        players?.forEach {
            result.append(GKPlayer(player: $0))
        }
        return result
    }

    private static func variantArrayToAchievements(_ array: VariantArray) -> [GameKit.GKAchievement] {
        var result: [GameKit.GKAchievement] = []
        for item in array {
            guard let item, let wrapped = item.asObject(GKAchievement.self) else { continue }
            result.append(wrapped.achievement)
        }
        return result
    }

    private static func variantArrayToLeaderboards(_ array: VariantArray) -> [GameKit.GKLeaderboard] {
        var result: [GameKit.GKLeaderboard] = []
        for item in array {
            guard let item, let wrapped = item.asObject(GKLeaderboard.self) else { continue }
            result.append(wrapped.board)
        }
        return result
    }

    @Export var achievements: VariantArray {
        Self.wrapAchievements(activity?.achievements)
    }

    @Export var activityDefinition: GKGameActivityDefinition? {
        guard let definition = activity?.activityDefinition else { return nil }
        let wrapped = GKGameActivityDefinition()
        wrapped.definition = definition
        return wrapped
    }

    @Export var creationDate: Double {
        activity?.creationDate.timeIntervalSince1970 ?? 0
    }

    @Export var duration: Double {
        activity?.duration ?? 0
    }

    @Export var endDate: Double {
        activity?.endDate?.timeIntervalSince1970 ?? 0
    }

    @Export var identifier: String {
        activity?.identifier ?? ""
    }

    @Export var lastResumeDate: Double {
        activity?.lastResumeDate?.timeIntervalSince1970 ?? 0
    }

    @Export var leaderboardScores: VariantArray {
        Self.wrapLeaderboardScores(activity?.leaderboardScores)
    }

    @Export var partyCode: String {
        activity?.partyCode ?? ""
    }

    @Export var partyURL: String {
        activity?.partyURL?.absoluteString ?? ""
    }

    @Export var properties: VariantDictionary {
        get {
            let result = VariantDictionary()
            for (key, value) in activity?.properties ?? [:] {
                result[key] = Variant(value)
            }
            return result
        }
        set {
            var converted: [String: String] = [:]
            for key in newValue.keys() {
                guard let key else { continue }
                guard let keyString = String(key) else { continue }
                guard let valueVariant = newValue[key] else { continue }
                guard let valueString = String(valueVariant) else { continue }
                converted[keyString] = valueString
            }
            activity?.properties = converted
        }
    }

    @Export var startDate: Double {
        activity?.startDate?.timeIntervalSince1970 ?? 0
    }

    @Export var state: Int {
        Int(activity?.state.rawValue ?? 0)
    }

    @Callable
    static func check_pending_game_activity_existence(callback: Callable) {
        GameKit.GKGameActivity.checkPendingGameActivityExistence { exists in
            _ = callback.call(Variant(exists), nil)
        }
    }

    @Callable
    static func create_with_definition(definition: GKGameActivityDefinition) -> GKGameActivity? {
        guard let definition = definition.definition else { return nil }
        return GKGameActivity(activity: GameKit.GKGameActivity(definition: definition))
    }

    @Callable
    static func is_valid_party_code(_ partyCode: String) -> Bool {
        GameKit.GKGameActivity.isValidPartyCode(partyCode)
    }

    @Callable
    static func valid_party_code_alphabet() -> PackedStringArray {
        let result = PackedStringArray()
        for item in GameKit.GKGameActivity.validPartyCodeAlphabet {
            result.append(item)
        }
        return result
    }

    @Callable
    static func start_with_definition(definition: GKGameActivityDefinition, callback: Callable) {
        guard let definition = definition.definition else {
            _ = callback.call(nil, Variant("Invalid activity definition object"))
            return
        }

        do {
            let activity = try GameKit.GKGameActivity.start(definition: definition)
            _ = callback.call(Variant(GKGameActivity(activity: activity)), nil)
        } catch {
            _ = callback.call(nil, GKError.from(error))
        }
    }

    @Callable
    static func start_with_definition_and_party_code(
        definition: GKGameActivityDefinition,
        partyCode: String,
        callback: Callable
    ) {
        guard let definition = definition.definition else {
            _ = callback.call(nil, Variant("Invalid activity definition object"))
            return
        }

        do {
            let activity = try GameKit.GKGameActivity.start(definition: definition, partyCode: partyCode)
            _ = callback.call(Variant(GKGameActivity(activity: activity)), nil)
        } catch {
            _ = callback.call(nil, GKError.from(error))
        }
    }

    @Callable
    func start() {
        activity?.start()
    }

    @Callable
    func pause() {
        activity?.pause()
    }

    @Callable
    func resume() {
        activity?.resume()
    }

    @Callable
    func end() {
        activity?.end()
    }

    @Callable
    func find_match(callback: Callable) {
        guard let activity else {
            _ = callback.call(nil, Variant("Invalid game activity object"))
            return
        }

        activity.findMatch { match, error in
            if let match {
                _ = callback.call(Variant(GKMatch(match: match)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        }
    }

    @Callable
    func find_players_for_hosted_match(callback: Callable) {
        guard let activity else {
            _ = callback.call(Variant(TypedArray<GKPlayer?>()), Variant("Invalid game activity object"))
            return
        }

        activity.findPlayersForHostedMatch { players, error in
            _ = callback.call(Variant(Self.wrapPlayers(players)), GKError.from(error))
        }
    }

    @Callable
    func get_progress_on_achievement(achievement: GKAchievement) -> Double {
        guard let activity else { return 0 }
        return activity.progress(on: achievement.achievement)
    }

    @Callable
    func get_score_on_leaderboard(leaderboard: GKLeaderboard) -> GKLeaderboardScore? {
        guard let activity else { return nil }
        guard let score = activity.score(on: leaderboard.board) else { return nil }
        return GKLeaderboardScore(score: score)
    }

    @Callable
    func make_match_request() -> GKMatchRequest? {
        guard let request = activity?.makeMatchRequest() else { return nil }
        let wrapped = GKMatchRequest()
        wrapped.request = request
        return wrapped
    }

    @Callable
    func remove_achievements(_ achievements: VariantArray) {
        guard let activity else { return }
        let converted = Self.variantArrayToAchievements(achievements)
        activity.removeAchievements(converted)
    }

    @Callable
    func remove_scores_from_leaderboards(_ leaderboards: VariantArray) {
        guard let activity else { return }
        let converted = Self.variantArrayToLeaderboards(leaderboards)
        activity.removeScores(from: converted)
    }

    @Callable
    func set_achievement_completed(_ achievement: GKAchievement) {
        activity?.setAchievementCompleted(achievement.achievement)
    }

    @Callable
    func set_progress_on_achievement(achievement: GKAchievement, percentComplete: Double) {
        activity?.setProgress(on: achievement.achievement, to: percentComplete)
    }

    @Callable
    func set_score_on_leaderboard(leaderboard: GKLeaderboard, score: Int) {
        activity?.setScore(on: leaderboard.board, to: score)
    }

    @Callable
    func set_score_on_leaderboard_with_context(leaderboard: GKLeaderboard, score: Int, context: Int) {
        activity?.setScore(on: leaderboard.board, to: score, context: context)
    }
}
