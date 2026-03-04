//
//  AppleLeaderboardSets.swift
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
class GKLeaderboardSet: RefCounted, @unchecked Sendable {
    var boardset = GameKit.GKLeaderboardSet()

    convenience init(boardset: GameKit.GKLeaderboardSet) {
        self.init()
        self.boardset = boardset
    }

    @Export var title: String { boardset.title }
    @Export var identifier: String { boardset.identifier ?? "" }
    @Export var groupIdentifier: String { boardset.groupIdentifier ?? "" }

    @Callable
    func load_leaderboards(callback: Callable) {
        boardset.loadLeaderboards { leaderboards, error in
            let result = TypedArray<GKLeaderboard?>()
            leaderboards?.forEach {
                result.append(GKLeaderboard(board: $0))
            }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }

    @Callable
    static func load_leaderboard_sets(callback: Callable) {
        GameKit.GKLeaderboardSet.loadLeaderboardSets { sets, error in
            let result = TypedArray<GKLeaderboardSet?>()
            sets?.forEach {
                result.append(GKLeaderboardSet(boardset: $0))
            }
            _ = callback.call(Variant(result), GKError.from(error))
        }
    }

    @Callable
    func load_image(callback: Callable) {
        boardset.loadImage { image, error in
            if let image, let godotImage = image.asGodotImage() {
                _ = callback.call(godotImage, nil)
            } else if let error {
                _ = callback.call(nil, GKError.from(error))
            } else {
                _ = callback.call(nil, Variant("Could not load leaderboard set image"))
            }
        }
    }
}
