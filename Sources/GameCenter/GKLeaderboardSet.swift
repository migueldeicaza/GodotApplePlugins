//
//  AppleLeaderboardSets.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/16/25.
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

    convenience init?(boardset: GameKit.GKLeaderboardSet) {
        self.init()
        self.boardset = boardset
    }
}
