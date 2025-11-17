//
//  AppleAchievement.swift
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

    @Export var identifier: String { achievement.identifier }
    @Export var player: GKPlayer { GKPlayer(player: achievement.player) }
    @Export var percentComplete: Double { achievement.percentComplete }
    @Export var isCompleted: Bool { achievement.isCompleted }
    // TODO: lastReportedDate - how to encode Dates in Godot

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable()
    static func report_achivement(achivements: VariantArray, callback: Callable) {
        var array: [GameKit.GKAchievement] = []
        for va in achivements {
            guard let va else { continue }
            if let a = va.asObject(GKAchievement.self) {
                array.append(a.achievement)
            }
        }
        GameKit.GKAchievement.report(array) { error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else {
                _ = callback.call(nil)
            }
        }
    }

    /// The callback is invoked with nil on success, or a string with a description of the error
    @Callable
    static func reset_achivements(callback: Callable) {
        GameKit.GKAchievement.resetAchievements { error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else {
                _ = callback.call(nil)
            }
        }
    }

    @Callable
    func load_achievement_descriptions(callback: Callable) {
        GameKit.GKAchievementDescription.loadAchievementDescriptions { achievementDescriptions, error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else if let achievementDescriptions {
                let res = VariantArray()
                for ad in achievementDescriptions {
                    let ad = GKAchievementDescription(ad)
                    res.append(Variant(ad))
                }
                _ = callback.call(Variant(res))
            } else {
                _ = callback.call(Variant(VariantArray()))
            }
        }
    }
}

@Godot
class GKAchievementDescription: RefCounted, @unchecked Sendable {
    var achievementDescription: GameKit.GKAchievementDescription = GameKit.GKAchievementDescription()

    convenience init(_ ad: GameKit.GKAchievementDescription) {
        self.init()
        self.achievementDescription = ad
    }

    @Export var identifier: String { achievementDescription.identifier }
    @Export var title: String { achievementDescription.title }
    @Export var unachievedDescription: String { achievementDescription.unachievedDescription }
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

    /// Callback is invoked with either a string message on error or a PackedByteArray with the contents of a PNG image
    @Callable
    func load_image(callback: Callable) {
        achievementDescription.loadImage { image, error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else if let image, let png = image.pngData() {
                let array = PackedByteArray([UInt8](png))
                _ = callback.call(Variant(array))
            } else {
                _ = callback.call(Variant("Could not load image"))
            }
        }
    }
}
