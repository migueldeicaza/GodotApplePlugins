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
class AppleAchievement: RefCounted, @unchecked Sendable {
    let achievement: GKAchievement

    required init(_ context: InitContext) {
        fatalError("AppleAchievement should only be constructed via GameCenterManager")
    }

    init?(identifier: String, player: ApplePlayer?) {
        if let player {
            self.achievement = GKAchievement(identifier: identifier, player: player.player)
        } else {
            self.achievement = GKAchievement(identifier: identifier)
        }
        guard let ctx = InitContext.createObject(className: AppleAchievement.godotClassName) else {
            return nil
        }

        super.init(ctx)
    }

    @Export var identifier: String { achievement.identifier }
    @Export var player: ApplePlayer { ApplePlayer(player: achievement.player) }
    @Export var percentComplete: Double { achievement.percentComplete }
    @Export var isCompleted: Bool { achievement.isCompleted }
    // TODO: lastReportedDate - how to encode Dates in Godot
    
}

@Godot
class AppleAchievementDescription: RefCounted, @unchecked Sendable {
    var achievementDescription: GKAchievementDescription
    required init(_ context: InitContext) {
        fatalError("AppleAchievement should only be constructed via GameCenterManager")
    }

    init?(_ ad: GKAchievementDescription) {
        self.achievementDescription = ad
        guard let ctx = InitContext.createObject(className: AppleAchievementDescription.godotClassName) else {
            return nil
        }

        super.init(ctx)
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
