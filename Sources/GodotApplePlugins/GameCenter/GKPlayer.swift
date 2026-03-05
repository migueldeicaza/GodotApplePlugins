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
    @Export var guestIdentifier: String { player.guestIdentifier ?? "" }

    @Callable
    func scopedIDsArePersistent() -> Bool {
        player.scopedIDsArePersistent()
    }

    /// Callback is invoked with two parameters:
    /// (imageData: Image, erro: String)
    ///
    /// One of those two is nil.
    @Callable
    func load_photo(_ small: Bool, _ callback: Callable) {
        player.loadPhoto(for: small ? .small : .normal) { img, error in
            DispatchQueue.main.async {
                if let img {
                    if let godotImage = img.asGodotImage() {
                        _ = callback.call(Variant(godotImage), nil)
                    } else {
                        _ = callback.call(nil, Variant(String("Could not convert image")))
                    }
                }
                if let error {
                    _ = callback.call(nil, Variant(String(describing: error)))
                    return
                }
            }
        }
    }

    @Callable
    static func anonymous_guest_player(identifier: String) -> GKPlayer? {
        let player = GameKit.GKPlayer.anonymousGuestPlayer(withIdentifier: identifier)
        return GKPlayer(player: player)
    }
}
