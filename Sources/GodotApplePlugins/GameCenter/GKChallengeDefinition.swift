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
class GKChallengeDefinition: RefCounted, @unchecked Sendable {
    var definition: GameKit.GKChallengeDefinition?

    convenience init(definition: GameKit.GKChallengeDefinition) {
        self.init()
        self.definition = definition
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
        definition?.details ?? ""
    }

    @Export var durationOptions: VariantArray {
        let result = VariantArray()
        for option in definition?.durationOptions ?? [] {
            result.append(Variant(Self.dateComponentsToVariant(option)))
        }
        return result
    }

    @Export var groupIdentifier: String {
        definition?.groupIdentifier ?? ""
    }

    @Export var identifier: String {
        definition?.identifier ?? ""
    }

    @Export var isRepeatable: Bool {
        definition?.isRepeatable ?? false
    }

    @Export var leaderboard: GKLeaderboard? {
        guard let leaderboard = definition?.leaderboard else { return nil }
        return GKLeaderboard(board: leaderboard)
    }

    @Export var releaseState: Int {
        Int(definition?.releaseState.rawValue ?? 0)
    }

    @Export var title: String {
        definition?.title ?? ""
    }

    @Callable
    func has_active_challenges(callback: Callable) {
        guard let definition else {
            _ = callback.call(Variant(false), Variant("Invalid challenge definition object"))
            return
        }

        definition.hasActiveChallenges { hasActiveChallenges, error in
            _ = callback.call(Variant(hasActiveChallenges), GKError.from(error))
        }
    }

    @Callable
    static func load_challenge_definitions(callback: Callable) {
        GameKit.GKChallengeDefinition.loadChallengeDefinitions { definitions, error in
            let wrapped = TypedArray<GKChallengeDefinition?>()
            definitions?.forEach {
                wrapped.append(GKChallengeDefinition(definition: $0))
            }
            _ = callback.call(Variant(wrapped), GKError.from(error))
        }
    }

    @Callable
    func load_image(callback: Callable) {
        guard let definition else {
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
