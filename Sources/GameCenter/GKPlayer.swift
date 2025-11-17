//
//  ApplePlayer.swift
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
class GKPlayer: RefCounted, @unchecked Sendable {
    var player: GameKit.GKPlayer = GameKit.GKPlayer()

    init(player: GameKit.GKPlayer) {
        self.player = player
        guard let ctxt = InitContext.createObject(className: GKPlayer.godotClassName) else {
            fatalError("Could not create object")
        }
        super.init(ctxt)

    }

    required init(_ context: InitContext) {
        player = GameKit.GKPlayer()
        super.init(context)
    }

    @Export var gamePlayerID: String { player.gamePlayerID }
    @Export var teamPlayerID: String { player.teamPlayerID }
    @Export var alias: String { player.alias }
    @Export var displayName: String { player.displayName }
    @Export var isInvitable: Bool { player.isInvitable }

    @Callable
    func scopedIDsArePersistent() -> Bool {
        player.scopedIDsArePersistent()
    }

    @Callable
    func load_photo(_ small: Bool, _ callback: Callable) {
        GD.print("request to load photo")
        player.loadPhoto(for: small ? .small : .normal) { img, error in
            GD.print("Result from loadPhoto: \(String(describing: img)) \(String(describing: error))")
            if let img, let png = img.pngData() {
                let array = PackedByteArray([UInt8](png))
                DispatchQueue.main.async {
                    _ = callback.call(Variant(array))
                }
            }
        }
    }
}
