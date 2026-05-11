//
//  GKInvite.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 3/3/26.
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKInvite: RefCounted, @unchecked Sendable {
    var invite: GameKit.GKInvite?

    convenience init(invite: GameKit.GKInvite) {
        self.init()
        self.invite = invite
    }

    @Export var sender: GKPlayer? {
        guard let sender = invite?.sender else {
            return nil
        }
        return GKPlayer(player: sender)
    }

    @Export var playerAttributes: Int {
        Int(invite?.playerAttributes ?? 0)
    }

    @Export var playerGroup: Int {
        invite?.playerGroup ?? 0
    }

    @Export var isHosted: Bool {
        invite?.isHosted ?? false
    }
}
