//
//  GKMatch.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/18/25.
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
class GKMatch: RefCounted, @unchecked Sendable {
    var gkmatch = GameKit.GKMatch()
    var delegate: Proxy?

    enum SendDataMode: Int, CaseIterable {
        case reliable
        case unreliable
        func toGameKit() -> GameKit.GKMatch.SendDataMode {
            switch self {
            case .reliable: return .reliable
            case .unreliable: return .unreliable
            }
        }
    }

    convenience init(match: GameKit.GKMatch) {
        self.init()
        self.gkmatch = match

        // This is just so the proxy is not deallocated right away
        self.delegate =  Proxy(self)
        gkmatch.delegate = self.delegate
    }

    class Proxy: NSObject, GKMatchDelegate {
        weak var base: GKMatch?

        init(_ base: GKMatch) {
            self.base = base
        }

        func match(
            _ match: GameKit.GKMatch,
            didReceive: Data,
            fromRemotePlayer: GameKit.GKPlayer
        ) {
            base?.data_received.emit(didReceive.toPackedByteArray(), GKPlayer(player: fromRemotePlayer))
        }

        func match(
            _ match: GameKit.GKMatch,
            didReceive: Data,
            forRecipient: GameKit.GKPlayer,
            fromRemotePlayer: GameKit.GKPlayer
        ) {
            base?.data_received_for_recipient_from_player.emit(
                didReceive.toPackedByteArray(),
                GKPlayer(player: forRecipient),
                GKPlayer(player: fromRemotePlayer)
            )
        }

        func match(
            _ match: GameKit.GKMatch,
            player: GameKit.GKPlayer,
            didChange: GKPlayerConnectionState
        ) {
            base?.player_changed.emit(
                GKPlayer(player: player),
                didChange == .connected
            )
        }

        func match(
            _ match: GameKit.GKMatch,
            didFailWithError: (any Error)?
        ) {
            let res: String
            if let didFailWithError {
                res = didFailWithError.localizedDescription
            } else {
                res = "Generic error"
            }
            base?.did_fail_with_error.emit(res)
        }

        func match(
            _ match: GameKit.GKMatch,
            shouldReinviteDisconnectedPlayer: GameKit.GKPlayer
        ) -> Bool {
            guard let base, let cb = base.should_reinvite_disconnected_player else {
                return false
            }
            let retV = cb.call(Variant(GKPlayer(player: shouldReinviteDisconnectedPlayer)))
            if let ret = Bool(retV) {
                return ret
            }
            return false
        }
    }

    @Signal var data_received: SignalWithArguments<PackedByteArray, GKPlayer>
    @Signal var data_received_for_recipient_from_player: SignalWithArguments<PackedByteArray,GKPlayer,GKPlayer>

    // The boolean indicates if it is connected (true) or disconncted(false
    @Signal var player_changed: SignalWithArguments<GKPlayer, Bool>
    @Signal var did_fail_with_error: SignalWithArguments<String>

    // Connect to a function that accepts a GKPlayer and returns a boolean
    @Export var should_reinvite_disconnected_player: Callable?

    @Export var expected_player_count: Int { gkmatch.expectedPlayerCount }
    @Export var players: VariantArray {
        let result = VariantArray()
        for player in gkmatch.players {
            result.append(Variant(GKPlayer(player: player)))
        }
        return result
    }

    // TODO: these Godot errors could be better, or perhaps we should return a string?   But I do not like
    // the idea of returning an empty string to say "ok"
    @Callable
    func send(data: PackedByteArray, toPlayers: VariantArray, dataMode: SendDataMode) -> GodotError {
        guard let sdata = data.asData() else {
            return .failed
        }
        var to: [GameKit.GKPlayer] = []
        for po in toPlayers {
            guard let po, let player = po.asObject(GKPlayer.self) else {
                continue
            }
            to.append(player.player)
        }
        do {
            try gkmatch.send(sdata, to: to, dataMode: dataMode.toGameKit())
            return .ok
        } catch {
            return .failed
        }
    }

    @Callable
    func send_data_to_all_players(data: PackedByteArray, dataMode: SendDataMode) -> GodotError {
        guard let sdata = data.asData() else {
            return .failed
        }
        do {
            try gkmatch.sendData(toAllPlayers: sdata, with: dataMode.toGameKit())
            return .ok
        } catch {
            return .failed
        }
    }

    @Callable
    func disconnect() {
        gkmatch.disconnect()
    }
}
