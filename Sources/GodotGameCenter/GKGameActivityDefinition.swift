//
//  GKGameActivityDefinition.swift
//  GodotApplePlugins
//
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
class GKGameActivityDefinition: RefCounted, @unchecked Sendable {
    var rawDefinition: AnyObject?

    @available(iOS 26.0, macOS 26.0, *)
    convenience init(definition: GameKit.GKGameActivityDefinition) {
        self.init()
        self.rawDefinition = definition
    }

    private static func unavailableError(_ method: String) -> Variant? {
        let error = NSError(
            domain: GKErrorDomain,
            code: GameKit.GKError.Code.apiNotAvailable.rawValue,
            userInfo: [NSLocalizedDescriptionKey: "\(method) requires iOS 26/macOS 26"]
        )
        return GKError.from(error)
    }

    @Export var title: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.title
        }
        return ""
    }
    @Export var details: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.details ?? ""
        }
        return ""
    }
    @Export var fallbackURL: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.fallbackURL?.absoluteString ?? ""
        }
        return ""
    }
    @Export var groupIdentifier: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.groupIdentifier ?? ""
        }
        return ""
    }
    @Export var identifier: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.identifier
        }
        return ""
    }

    @Export var maxPlayers: Variant? {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition,
            let value = definition.__maxPlayers
        {
            return Variant(value.intValue)
        }
        return nil
    }

    @Export var minPlayers: Variant? {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition,
            let value = definition.__minPlayers
        {
            return Variant(value.intValue)
        }
        return nil
    }

    @Export var playStyle: Int {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.playStyle.rawValue
        }
        return 0
    }
    @Export var releaseState: Int {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return Int(definition.releaseState.rawValue)
        }
        return 0
    }
    @Export var supportsPartyCode: Bool {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.supportsPartyCode
        }
        return false
    }
    @Export var supportsUnlimitedPlayers: Bool {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        {
            return definition.supportsUnlimitedPlayers
        }
        return false
    }

    @Export var defaultProperties: TypedDictionary<String, String> {
        get {
            var result = TypedDictionary<String, String>()
            if #available(iOS 26.0, macOS 26.0, *),
                let definition = rawDefinition as? GameKit.GKGameActivityDefinition
            {
                for (key, value) in definition.defaultProperties {
                    result[key] = value
                }
            }
            return result
        }
    }

    @Callable
    func load_achievement_descriptions(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        else {
            _ = callback.call(Variant(TypedArray<GKAchievementDescription?>()), Variant("Invalid game activity definition object"))
            return
        }

        definition.loadAchievementDescriptions { descriptions, error in
            let result = TypedArray<GKAchievementDescription?>()
            descriptions?.forEach { result.append(GKAchievementDescription($0)) }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }

    @Callable
    func load_leaderboards(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        else {
            _ = callback.call(Variant(TypedArray<GKLeaderboard?>()), Variant("Invalid game activity definition object"))
            return
        }

        definition.loadLeaderboards { leaderboards, error in
            let result = TypedArray<GKLeaderboard?>()
            leaderboards?.forEach { result.append(GKLeaderboard(board: $0)) }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }

    @Callable
    func load_image(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKGameActivityDefinition
        else {
            _ = callback.call(nil, Variant("Invalid game activity definition object"))
            return
        }

        definition.loadImage { image, error in
            if let image, let godotImage = image.asGodotImage() {
                _ = callback.call(godotImage, nil)
            } else if let error {
                _ = callback.call(nil, GKError.from(error))
            } else {
                _ = callback.call(nil, Variant("Could not load image"))
            }
        }
    }

    @Callable
    static func load_game_activity_definitions(callback: Callable) {
        if #available(iOS 26.0, macOS 26.0, *) {
            GameKit.GKGameActivityDefinition.loadGameActivityDefinitions { definitions, error in
                let result = TypedArray<GKGameActivityDefinition?>()
                definitions?.forEach {
                    result.append(GKGameActivityDefinition(definition: $0))
                }
                _ = callback.call(Variant(result), GKError.from(error))
            }
        } else {
            _ = callback.call(Variant(TypedArray<GKGameActivityDefinition?>()), unavailableError("load_game_activity_definitions"))
        }
    }

    @Callable
    static func load_game_activity_definitions_with_ids(
        ids: PackedStringArray,
        callback: Callable
    ) {
        guard #available(iOS 26.0, macOS 26.0, *) else {
            _ = callback.call(Variant(TypedArray<GKGameActivityDefinition?>()), unavailableError("load_game_activity_definitions_with_ids"))
            return
        }
        var swiftIDs: [String] = []
        for index in 0..<ids.count {
            swiftIDs.append(ids[index])
        }

        GameKit.GKGameActivityDefinition.loadGameActivityDefinitions(IDs: swiftIDs) {
            definitions, error in
            let result = TypedArray<GKGameActivityDefinition?>()
            definitions?.forEach {
                result.append(GKGameActivityDefinition(definition: $0))
            }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }
}
