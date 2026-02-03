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
}
