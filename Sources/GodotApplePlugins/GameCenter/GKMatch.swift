//
//  GKMatch.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/18/25.
//


import Foundation
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

    private static func foundationNumberToVariant(_ number: NSNumber) -> Variant {
        if CFGetTypeID(number) == CFBooleanGetTypeID() {
            return Variant(number.boolValue)
        }

        let value = number.doubleValue
        if value.rounded(.towardZero) == value,
            value >= Double(Int64.min),
            value <= Double(Int64.max)
        {
            return Variant(Int64(value))
        }
        return Variant(value)
    }

    private static func foundationArrayToVariant(_ array: [Any]) -> VariantArray {
        let result = VariantArray()
        for element in array {
            guard let converted = foundationToVariant(element) else { continue }
            result.append(converted)
        }
        return result
    }

    private static func foundationDictionaryToVariant(_ dictionary: [String: Any]) -> VariantDictionary {
        let result = VariantDictionary()
        for (key, value) in dictionary {
            guard let converted = foundationToVariant(value) else { continue }
            result[key] = converted
        }
        return result
    }

    private static func foundationToVariant(_ value: Any) -> Variant? {
        switch value {
        case let value as Bool:
            return Variant(value)
        case let value as Int:
            return Variant(value)
        case let value as Int8:
            return Variant(Int64(value))
        case let value as Int16:
            return Variant(Int64(value))
        case let value as Int32:
            return Variant(Int64(value))
        case let value as Int64:
            return Variant(value)
        case let value as UInt:
            return value <= UInt(Int64.max) ? Variant(Int64(value)) : Variant(Double(value))
        case let value as UInt8:
            return Variant(Int64(value))
        case let value as UInt16:
            return Variant(Int64(value))
        case let value as UInt32:
            return Variant(Int64(value))
        case let value as UInt64:
            return value <= UInt64(Int64.max) ? Variant(Int64(value)) : Variant(Double(value))
        case let value as Float:
            return Variant(Double(value))
        case let value as Double:
            return Variant(value)
        case let value as String:
            return Variant(value)
        case let value as NSString:
            return Variant(String(value))
        case let value as [Any]:
            return Variant(foundationArrayToVariant(value))
        case let value as NSArray:
            return Variant(foundationArrayToVariant(value.compactMap { $0 }))
        case let value as [String: Any]:
            return Variant(foundationDictionaryToVariant(value))
        case let value as NSDictionary:
            var result: [String: Any] = [:]
            for case let (key as NSString, entry) in value {
                result[String(key)] = entry
            }
            return Variant(foundationDictionaryToVariant(result))
        case let value as NSNumber:
            return foundationNumberToVariant(value)
        case let value as Data:
            return Variant(value.toPackedByteArray())
        default:
            return nil
        }
    }

    enum SendDataMode: Int, CaseIterable {
        case RELIABLE
        case UNRELIABLE
        func toGameKit() -> GameKit.GKMatch.SendDataMode {
            switch self {
            case .RELIABLE: return .reliable
            case .UNRELIABLE: return .unreliable
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

    @Signal("data", "player") var data_received: SignalWithArguments<PackedByteArray, GKPlayer>
    @Signal("data", "recipient", "from_remote_player") var data_received_for_recipient_from_player: SignalWithArguments<PackedByteArray,GKPlayer,GKPlayer>

    // The boolean indicates if it is connected (true) or disconncted(false
    @Signal("player", "connected") var player_changed: SignalWithArguments<GKPlayer, Bool>
    @Signal("message") var did_fail_with_error: SignalWithArguments<String>

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

    @Export var properties: VariantDictionary {
        if #available(iOS 17.2, macOS 14.2, *) {
            guard let properties = gkmatch.properties else {
                return VariantDictionary()
            }
            return Self.foundationDictionaryToVariant(properties)
        } else {
            return VariantDictionary()
        }
    }

    @Export var playerProperties: VariantDictionary {
        if #available(iOS 17.2, macOS 14.2, *) {
            guard let playerProperties = gkmatch.playerProperties else {
                return VariantDictionary()
            }

            let result = VariantDictionary()
            for (player, properties) in playerProperties {
                result[player.gamePlayerID] = Variant(Self.foundationDictionaryToVariant(properties))
            }
            return result
        } else {
            return VariantDictionary()
        }
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
