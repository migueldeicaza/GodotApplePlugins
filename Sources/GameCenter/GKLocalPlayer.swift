//
//  GKLocalPlayer.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/17/25.
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
class GKLocalPlayer: GKPlayer, @unchecked Sendable {
    var local: GameKit.GKLocalPlayer

    required init(_ context: InitContext) {
        local = GameKit.GKLocalPlayer.local
        super.init(context)
        player = local
    }

    init() {
        local = GameKit.GKLocalPlayer.local
        super.init(player: GameKit.GKLocalPlayer.local)
    }

    @Export var isAuthenticated: Bool { local.isAuthenticated }
    @Export var isUnderage: Bool { local.isUnderage }
    @Export var isMultiplayerGamingRestricted: Bool { local.isMultiplayerGamingRestricted }
    @Export var isPersonalizedCommunicationRestricted: Bool { local.isPersonalizedCommunicationRestricted }

    func friendDispatch(_ callback: Callable, _ friends: [GameKit.GKPlayer]?, _ error: (any Error)?) {
        if let error {
            _ = callback.call(Variant(error.localizedDescription))
        } else {
            let array = VariantArray()
            if let friends {
                for friend in friends {
                    let gkplayer = GKPlayer(player: friend)
                    array.append(Variant(gkplayer))
                }
            }
            _ = callback.call(Variant(array))
        }
    }
    /// Loads the friends, on error you get a string back, on success an array containing GKPlayer objects
    @Callable func load_friends(callback: Callable) {
        local.loadFriends { friends, error in
            self.friendDispatch(callback, friends, error)
        }
    }

    /// Loads the friends, on error you get a string back, on success an array containing GKPlayer objects
    @Callable func load_challengeable_friends(callback: Callable) {
        local.loadChallengableFriends { friends, error in
            self.friendDispatch(callback, friends, error)
        }
    }

    /// Loads the friends, on error you get a string back, on success an array containing GKPlayer objects
    @Callable func load_recent_friends(callback: Callable) {
        local.loadRecentPlayers  { friends, error in
            self.friendDispatch(callback, friends, error)
        }
    }

    /// On error, you get called with a string parameter
    /// On success, you get an array with the following values:
    /// - String: The URL for the public encryption key.
    /// - PackedByteArray: verification signature that GameKit generates, or nil
    /// - PackedByteArray: A random NSString that GameKit uses to compute the hash and randomize it.
    /// - Int: The signature’s creation date and time timestamp
    func fetch_items_for_identity_verification_signature(callback: Callable) {
        local.fetchItems { url, data, salt, timestamp, error in
            if let error {
                _ = callback.call(Variant(error.localizedDescription))
            } else {
                let encodeData = data?.toPackedByteArray()
                let encodeSalt = salt?.toPackedByteArray()

                let result = VariantArray();
                result.append(Variant(url?.description ?? ""))
                result.append(encodeData != nil ? Variant(encodeData) : nil)
                result.append(encodeSalt != nil ? Variant(encodeSalt) : nil)
                result.append(Variant(timestamp))

                _ = callback.call(Variant(result))
            }
        }
    }
}
