//
//  AppleAchievement.swift
//  GodotApplePlugins
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
class GKAchievement: RefCounted, @unchecked Sendable {
    var achievement: GameKit.GKAchievement = GameKit.GKAchievement()

    private static func variantArrayToPlayers(_ values: VariantArray) -> [GameKit.GKPlayer] {
        var players: [GameKit.GKPlayer] = []
        for value in values {
            guard let value, let wrapped = value.asObject(GKPlayer.self) else { continue }
            players.append(wrapped.player)
        }
        return players
    }

    convenience init(identifier: String, player: GKPlayer?) {
        self.init()

        if let player {
            self.achievement = GameKit.GKAchievement(identifier: identifier, player: player.player)
        } else {
            self.achievement = GameKit.GKAchievement(identifier: identifier)
        }
    }

    convenience init(achievement: GameKit.GKAchievement) {
        self.init()
        self.achievement = achievement
    }

    @Callable
    static func make(identifier: String) -> GKAchievement {
        GKAchievement(identifier: identifier, player: nil)
    }

    @Callable
    static func make_for_player(identifier: String, player: GKPlayer) -> GKAchievement {
        GKAchievement(identifier: identifier, player: player)
    }

    @Export var identifier: String {
        get { achievement.identifier }
        set { achievement.identifier = newValue }
    }
    @Export var player: GKPlayer { GKPlayer(player: achievement.player) }
    @Export var percentComplete: Double {
        get { achievement.percentComplete }
        set { achievement.percentComplete = newValue }
    }
    @Export var isCompleted: Bool { achievement.isCompleted }
    @Export var showsCompletionBanner: Bool {
        get { achievement.showsCompletionBanner }
        set { achievement.showsCompletionBanner = newValue }
    }
    @Export var lastReportedDate: Double {
        achievement.lastReportedDate.timeIntervalSince1970
    }

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable()
    static func report_achievement(achievements: VariantArray, callback: Callable) {
        var array: [GameKit.GKAchievement] = []
        for va in achievements {
            guard let va else { continue }
            if let a = va.asObject(GKAchievement.self) {
                array.append(a.achievement)
            }
        }
        GameKit.GKAchievement.report(array) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable
    static func reset_achievements(callback: Callable) {
        GameKit.GKAchievement.resetAchievements { error in
            _ = callback.call(GKError.from(error))
        }
    }

    /// Callback is invoked with two arguments an `Array[GKachievement]` and an error argument
    /// on success the error i snil
    @Callable
    static func load_achievements(callback: Callable) {
        GameKit.GKAchievement.loadAchievements { achievements, error in
            let res = TypedArray<GKAchievement?>()

            if let achievements {
                for ad in achievements {
                    let ad = GKAchievement(achievement: ad)
                    res.append(ad)
                }
            }
            _ = callback.call(Variant(res), GKError.from(error))
        }
    }

    @Callable
    func select_challengeable_players(players: VariantArray, callback: Callable) {
        let sourcePlayers = Self.variantArrayToPlayers(players)
        achievement.selectChallengeablePlayers(sourcePlayers) { challengeablePlayers, error in
            let result = TypedArray<GKPlayer?>()
            challengeablePlayers?.forEach { result.append(GKPlayer(player: $0)) }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }

    @Callable
    func challenge_compose_controller(message: String, players: VariantArray) {
        let sourcePlayers = Self.variantArrayToPlayers(players)

        #if os(visionOS)
        achievement.challengeComposeController(
            withMessage: message,
            players: sourcePlayers,
            completion: nil
        )
        #else
        if #available(iOS 17.0, macOS 14.0, tvOS 17.0, *) {
            achievement.challengeComposeController(
                withMessage: message,
                players: sourcePlayers,
                completion: nil
            )
        } else {
            achievement.challengeComposeController(
                withMessage: message,
                players: sourcePlayers,
                completionHandler: nil
            )
        }
        #endif
    }
}

@Godot
class GKAchievementDescription: RefCounted, @unchecked Sendable {
    var achievementDescription: GameKit.GKAchievementDescription =
        GameKit.GKAchievementDescription()

    convenience init(_ ad: GameKit.GKAchievementDescription) {
        self.init()
        self.achievementDescription = ad
    }

    @Export var identifier: String { achievementDescription.identifier }
    @Export var title: String { achievementDescription.title }
    @Export var unachievedDescription: String { achievementDescription.unachievedDescription }
    @Export var achievedDescription: String { achievementDescription.achievedDescription }
    @Export var maximumPoints: Int { achievementDescription.maximumPoints }
    @Export var isHidden: Bool { achievementDescription.isHidden }
    @Export var isReplayable: Bool { achievementDescription.isReplayable }
    @Export var groupIdentifier: String { achievementDescription.groupIdentifier ?? "" }
    @Export var activityIdentifier: String {
        if #available(iOS 26.0, macOS 26.0, tvOS 26.0, visionOS 26.0, *) {
            return achievementDescription.activityIdentifier
        } else {
            return ""
        }
    }
    @Export var activityProperties: TypedDictionary<String, String> {
        var result = TypedDictionary<String, String>()
        if #available(iOS 26.0, macOS 26.0, tvOS 26.0, visionOS 26.0, *) {
            for (key, value) in achievementDescription.activityProperties {
                result[key] = value
            }
        }
        return result
    }
    @Export var releaseState: Int {
        if #available(iOS 18.4, macOS 15.4, tvOS 18.4, visionOS 2.4, *) {
            return Int(achievementDescription.releaseState.rawValue)
        } else {
            return 0
        }
    }
    /// A double with the valur or nil
    @Export var rarityPercent: Variant? {
        if let rp = achievementDescription.rarityPercent {
            return Variant(rp)
        } else {
            return nil
        }
    }

    /// Callback is invoked with two arguments an Image witht he image and an error argument
    /// either one can be nil.
    @Callable
    func load_image(callback: Callable) {
        achievementDescription.loadImage { image, error in
            if let error {
                _ = callback.call(nil, GKError.from(error))
            } else if let image, let godotImage = image.asGodotImage() {
                _ = callback.call(godotImage, nil)
            } else {
                _ = callback.call(nil, Variant("Could not load image"))
            }
        }
    }

    /// Callback is invoked with two arguments an array of GKachievementDescriptions and an error argument
    /// either one can be nil.
    @Callable
    static func load_achievement_descriptions(callback: Callable) {
        GameKit.GKAchievementDescription.loadAchievementDescriptions {
            achievementDescriptions, error in
            let res = TypedArray<GKAchievementDescription?>()

            if let achievementDescriptions {
                for ad in achievementDescriptions {
                    let ad = GKAchievementDescription(ad)
                    res.append(ad)
                }
            }
            _ = callback.call(Variant(res), GKError.from(error))
        }
    }

    @Callable
    static func incomplete_achievement_image() -> Variant? {
        return GameKit.GKAchievementDescription.incompleteAchievementImage().asGodotImage()
    }

    @Callable
    static func placeholder_completed_achievement_image() -> Variant? {
        return GameKit.GKAchievementDescription.placeholderCompletedAchievementImage().asGodotImage()
    }
}
