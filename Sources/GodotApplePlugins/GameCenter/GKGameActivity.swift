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

@Godot
class GKGameActivity: RefCounted, @unchecked Sendable {
    var rawActivity: AnyObject?

    @available(iOS 26.0, macOS 26.0, *)
    convenience init(activity: GameKit.GKGameActivity) {
        self.init()
        self.rawActivity = activity
    }

    private static func unavailableError(_ method: String) -> Variant? {
        let error = NSError(
            domain: GKErrorDomain,
            code: GameKit.GKError.Code.apiNotAvailable.rawValue,
            userInfo: [NSLocalizedDescriptionKey: "\(method) requires iOS 26/macOS 26"]
        )
        return GKError.from(error)
    }

    private static func wrapAchievements(_ achievements: Set<GameKit.GKAchievement>) -> VariantArray {
        let result = VariantArray()
        achievements.forEach {
            result.append(Variant(GKAchievement(achievement: $0)))
        }
        return result
    }

    private static func wrapLeaderboardScores(_ scores: Set<GameKit.GKLeaderboardScore>) -> VariantArray {
        let result = VariantArray()
        scores.forEach {
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
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return Self.wrapAchievements(activity.achievements)
        }
        return VariantArray()
    }

    @Export var activityDefinition: GKGameActivityDefinition? {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            let wrapped = GKGameActivityDefinition()
            wrapped.rawDefinition = activity.activityDefinition
            return wrapped
        }
        return nil
    }

    @Export var creationDate: Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.creationDate.timeIntervalSince1970
        }
        return 0
    }

    @Export var duration: Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.duration
        }
        return 0
    }

    @Export var endDate: Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.endDate?.timeIntervalSince1970 ?? 0
        }
        return 0
    }

    @Export var identifier: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.identifier
        }
        return ""
    }

    @Export var lastResumeDate: Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.lastResumeDate?.timeIntervalSince1970 ?? 0
        }
        return 0
    }

    @Export var leaderboardScores: VariantArray {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return Self.wrapLeaderboardScores(activity.leaderboardScores)
        }
        return VariantArray()
    }

    @Export var partyCode: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.partyCode ?? ""
        }
        return ""
    }

    @Export var partyURL: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.partyURL?.absoluteString ?? ""
        }
        return ""
    }

    @Export var properties: VariantDictionary {
        get {
            let result = VariantDictionary()
            if #available(iOS 26.0, macOS 26.0, *),
                let activity = rawActivity as? GameKit.GKGameActivity
            {
                for (key, value) in activity.properties {
                    result[key] = Variant(value)
                }
            }
            return result
        }
        set {
            if #available(iOS 26.0, macOS 26.0, *),
                let activity = rawActivity as? GameKit.GKGameActivity
            {
                var converted: [String: String] = [:]
                for key in newValue.keys() {
                    guard let key else { continue }
                    guard let keyString = String(key) else { continue }
                    guard let valueVariant = newValue[key] else { continue }
                    guard let valueString = String(valueVariant) else { continue }
                    converted[keyString] = valueString
                }
                activity.properties = converted
            }
        }
    }

    @Export var startDate: Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.startDate?.timeIntervalSince1970 ?? 0
        }
        return 0
    }

    @Export var state: Int {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return Int(activity.state.rawValue)
        }
        return 0
    }

    @Callable
    static func check_pending_game_activity_existence(callback: Callable) {
        if #available(iOS 26.0, macOS 26.0, *) {
            GameKit.GKGameActivity.checkPendingGameActivityExistence { exists in
                _ = callback.call(Variant(exists), nil)
            }
        } else {
            _ = callback.call(Variant(false), unavailableError("check_pending_game_activity_existence"))
        }
    }

    @Callable
    static func create_with_definition(definition: GKGameActivityDefinition) -> GKGameActivity? {
        guard #available(iOS 26.0, macOS 26.0, *),
            let nativeDefinition = definition.rawDefinition as? GameKit.GKGameActivityDefinition
        else { return nil }
        return GKGameActivity(activity: GameKit.GKGameActivity(definition: nativeDefinition))
    }

    @Callable
    static func is_valid_party_code(_ partyCode: String) -> Bool {
        if #available(iOS 26.0, macOS 26.0, *) {
            return GameKit.GKGameActivity.isValidPartyCode(partyCode)
        }
        return false
    }

    @Callable
    static func valid_party_code_alphabet() -> PackedStringArray {
        let result = PackedStringArray()
        if #available(iOS 26.0, macOS 26.0, *) {
            for item in GameKit.GKGameActivity.validPartyCodeAlphabet {
                result.append(item)
            }
        }
        return result
    }

    @Callable
    static func start_with_definition(definition: GKGameActivityDefinition, callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *) else {
            _ = callback.call(nil, unavailableError("start_with_definition"))
            return
        }
        guard let nativeDefinition = definition.rawDefinition as? GameKit.GKGameActivityDefinition else {
            _ = callback.call(nil, Variant("Invalid activity definition object"))
            return
        }

        do {
            let activity = try GameKit.GKGameActivity.start(definition: nativeDefinition)
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
        guard #available(iOS 26.0, macOS 26.0, *) else {
            _ = callback.call(nil, unavailableError("start_with_definition_and_party_code"))
            return
        }
        guard let nativeDefinition = definition.rawDefinition as? GameKit.GKGameActivityDefinition else {
            _ = callback.call(nil, Variant("Invalid activity definition object"))
            return
        }

        do {
            let activity = try GameKit.GKGameActivity.start(definition: nativeDefinition, partyCode: partyCode)
            _ = callback.call(Variant(GKGameActivity(activity: activity)), nil)
        } catch {
            _ = callback.call(nil, GKError.from(error))
        }
    }

    @Callable
    func start() {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.start()
        }
    }

    @Callable
    func pause() {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.pause()
        }
    }

    @Callable
    func resume() {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.resume()
        }
    }

    @Callable
    func end() {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.end()
        }
    }

    @Callable
    func find_match(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        else {
            _ = callback.call(nil, Self.unavailableError("find_match"))
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
        guard #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        else {
            _ = callback.call(Variant(TypedArray<GKPlayer?>()), Self.unavailableError("find_players_for_hosted_match"))
            return
        }

        activity.findPlayersForHostedMatch { players, error in
            _ = callback.call(Variant(Self.wrapPlayers(players)), GKError.from(error))
        }
    }

    @Callable
    func get_progress_on_achievement(achievement: GKAchievement) -> Double {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            return activity.progress(on: achievement.achievement)
        }
        return 0
    }

    @Callable
    func get_score_on_leaderboard(leaderboard: GKLeaderboard) -> GKLeaderboardScore? {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity,
            let score = activity.score(on: leaderboard.board)
        {
            return GKLeaderboardScore(score: score)
        }
        return nil
    }

    @Callable
    func make_match_request() -> GKMatchRequest? {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity,
            let request = activity.makeMatchRequest()
        {
            let wrapped = GKMatchRequest()
            wrapped.request = request
            return wrapped
        }
        return nil
    }

    @Callable
    func remove_achievements(_ achievements: VariantArray) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            let converted = Self.variantArrayToAchievements(achievements)
            activity.removeAchievements(converted)
        }
    }

    @Callable
    func remove_scores_from_leaderboards(_ leaderboards: VariantArray) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            let converted = Self.variantArrayToLeaderboards(leaderboards)
            activity.removeScores(from: converted)
        }
    }

    @Callable
    func set_achievement_completed(_ achievement: GKAchievement) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.setAchievementCompleted(achievement.achievement)
        }
    }

    @Callable
    func set_progress_on_achievement(achievement: GKAchievement, percentComplete: Double) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.setProgress(on: achievement.achievement, to: percentComplete)
        }
    }

    @Callable
    func set_score_on_leaderboard(leaderboard: GKLeaderboard, score: Int) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.setScore(on: leaderboard.board, to: score)
        }
    }

    @Callable
    func set_score_on_leaderboard_with_context(leaderboard: GKLeaderboard, score: Int, context: Int) {
        if #available(iOS 26.0, macOS 26.0, *),
            let activity = rawActivity as? GameKit.GKGameActivity
        {
            activity.setScore(on: leaderboard.board, to: score, context: context)
        }
    }
}
