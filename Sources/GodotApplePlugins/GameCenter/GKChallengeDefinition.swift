import GameKit
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

@Godot
class GKChallengeDefinition: RefCounted, @unchecked Sendable {
    var rawDefinition: AnyObject?

    @available(iOS 26.0, macOS 26.0, *)
    convenience init(definition: GameKit.GKChallengeDefinition) {
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

    private static func dateComponentsToVariant(_ components: DateComponents) -> VariantDictionary {
        let result = VariantDictionary()
        if let year = components.year { result["year"] = Variant(year) }
        if let month = components.month { result["month"] = Variant(month) }
        if let day = components.day { result["day"] = Variant(day) }
        if let hour = components.hour { result["hour"] = Variant(hour) }
        if let minute = components.minute { result["minute"] = Variant(minute) }
        if let second = components.second { result["second"] = Variant(second) }
        if let nanosecond = components.nanosecond { result["nanosecond"] = Variant(nanosecond) }
        if let weekday = components.weekday { result["weekday"] = Variant(weekday) }
        if let weekOfMonth = components.weekOfMonth { result["weekOfMonth"] = Variant(weekOfMonth) }
        if let weekOfYear = components.weekOfYear { result["weekOfYear"] = Variant(weekOfYear) }
        if let quarter = components.quarter { result["quarter"] = Variant(quarter) }
        return result
    }

    @Export var details: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return definition.details ?? ""
        }
        return ""
    }

    @Export var durationOptions: VariantArray {
        let result = VariantArray()
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            for option in definition.durationOptions {
                result.append(Variant(Self.dateComponentsToVariant(option)))
            }
        }
        return result
    }

    @Export var groupIdentifier: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return definition.groupIdentifier ?? ""
        }
        return ""
    }

    @Export var identifier: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return definition.identifier
        }
        return ""
    }

    @Export var isRepeatable: Bool {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return definition.isRepeatable
        }
        return false
    }

    @Export var leaderboard: GKLeaderboard? {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition,
            let leaderboard = definition.leaderboard
        {
            return GKLeaderboard(board: leaderboard)
        }
        return nil
    }

    @Export var releaseState: Int {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return Int(definition.releaseState.rawValue)
        }
        return 0
    }

    @Export var title: String {
        if #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        {
            return definition.title
        }
        return ""
    }

    @Callable
    func has_active_challenges(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        else {
            _ = callback.call(Variant(false), Variant("Invalid challenge definition object"))
            return
        }

        definition.hasActiveChallenges { hasActiveChallenges, error in
            _ = callback.call(Variant(hasActiveChallenges), GKError.from(error))
        }
    }

    @Callable
    static func load_challenge_definitions(callback: Callable) {
        if #available(iOS 26.0, macOS 26.0, *) {
            GameKit.GKChallengeDefinition.loadChallengeDefinitions { definitions, error in
                let wrapped = TypedArray<GKChallengeDefinition?>()
                definitions?.forEach {
                    wrapped.append(GKChallengeDefinition(definition: $0))
                }
                _ = callback.call(Variant(wrapped), GKError.from(error))
            }
        } else {
            _ = callback.call(Variant(TypedArray<GKChallengeDefinition?>()), unavailableError("load_challenge_definitions"))
        }
    }

    @Callable
    func load_image(callback: Callable) {
        guard #available(iOS 26.0, macOS 26.0, *),
            let definition = rawDefinition as? GameKit.GKChallengeDefinition
        else {
            _ = callback.call(nil, Variant("Invalid challenge definition object"))
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
}
