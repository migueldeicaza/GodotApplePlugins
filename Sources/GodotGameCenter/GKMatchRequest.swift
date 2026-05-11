//
//  GKMatchRequest.swift
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
class GKMatchRequest: RefCounted, @unchecked Sendable {
    var request = GameKit.GKMatchRequest()
    @Export var recipientResponse: Callable? {
        didSet {
            configureRecipientResponse()
        }
    }

    enum MatchType: Int, CaseIterable {
        case PEER_TO_PEER
        case HOSTED
        case TURN_BASED
        func toGameKit() -> GameKit.GKMatchType {
            switch self {
            case .PEER_TO_PEER: return .peerToPeer
            case .HOSTED: return .hosted
            case .TURN_BASED: return .turnBased
            }
        }
    }

    enum InviteRecipientResponse: Int, CaseIterable {
        case ACCEPTED
        case DECLINED
        case FAILED
        case INCOMPATIBLE
        case UNABLE_TO_CONNECT
        case NO_ANSWER
        case UNKNOWN
    }

    private static func mapRecipientResponse(_ response: GameKit.GKInviteRecipientResponse)
        -> InviteRecipientResponse
    {
        switch response {
        case .accepted: return .ACCEPTED
        case .declined: return .DECLINED
        case .failed: return .FAILED
        case .incompatible: return .INCOMPATIBLE
        case .unableToConnect: return .UNABLE_TO_CONNECT
        case .noAnswer: return .NO_ANSWER
        @unknown default: return .UNKNOWN
        }
    }

    private func configureRecipientResponse() {
        if #available(iOS 17.2, macOS 14.2, *) {
            guard let callback = recipientResponse else {
                request.recipientResponseHandler = nil
                return
            }
            request.recipientResponseHandler = { player, response in
                let mapped = Self.mapRecipientResponse(response)
                _ = callback.call(Variant(GKPlayer(player: player)), Variant(mapped.rawValue))
            }
        }
    }

    private static func variantDictionaryToFoundation(_ dictionary: VariantDictionary) -> [String: Any] {
        var result: [String: Any] = [:]
        for key in dictionary.keys() {
            guard let key, let keyString = String(key) else { continue }
            guard let value = variantToFoundation(dictionary[key]) else { continue }
            result[keyString] = value
        }
        return result
    }

    private static func variantArrayToFoundation(_ array: VariantArray) -> [Any] {
        var result: [Any] = []
        for value in array {
            guard let converted = variantToFoundation(value) else { continue }
            result.append(converted)
        }
        return result
    }

    private static func variantToFoundation(_ variant: Variant?) -> Any? {
        guard let variant else { return nil }

        switch variant.gtype {
        case .bool:
            return Bool(variant)
        case .int:
            if let int64Value = Int64(variant) {
                return Int(int64Value)
            }
            return nil
        case .float:
            return Double(variant)
        case .string, .stringName:
            return String(variant)
        case .array:
            guard let array = VariantArray(variant) else { return nil }
            return variantArrayToFoundation(array)
        case .dictionary:
            guard let dictionary = VariantDictionary(variant) else { return nil }
            return variantDictionaryToFoundation(dictionary)
        case .packedByteArray:
            guard let bytes = PackedByteArray(variant) else { return nil }
            return bytes.asData()
        default:
            return nil
        }
    }

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

    @Export var minPlayers: Int {
        get { request.minPlayers }
        set { request.minPlayers = newValue }
    }

    @Export var maxPlayers: Int {
        get { request.maxPlayers }
        set { request.maxPlayers = newValue }
    }

    @Export var playerGroup: Int {
        get { request.playerGroup }
        set { request.playerGroup = newValue }
    }

    @Export var playerAttributes: Int {
        get { Int(request.playerAttributes) }
        set { request.playerAttributes = UInt32(clamping: newValue) }
    }

    @Export var recipients: VariantArray {
        get {
            let result = VariantArray()
            for player in request.recipients ?? [] {
                result.append(Variant(GKPlayer(player: player)))
            }
            return result
        }
        set {
            var players: [GameKit.GKPlayer] = []
            for player in newValue {
                guard let player, let wrapped = player.asObject(GKPlayer.self) else { continue }
                players.append(wrapped.player)
            }
            request.recipients = players
        }
    }

    @Export var queueName: String {
        get {
            if #available(iOS 17.2, macOS 14.2, *) {
                return request.queueName ?? ""
            }
            return ""
        }
        set {
            if #available(iOS 17.2, macOS 14.2, *) {
                request.queueName = newValue.isEmpty ? nil : newValue
            }
        }
    }

    @Export var properties: VariantDictionary {
        get {
            if #available(iOS 17.2, macOS 14.2, *) {
                guard let properties = request.properties else {
                    return VariantDictionary()
                }
                return Self.foundationDictionaryToVariant(properties)
            }
            return VariantDictionary()
        }
        set {
            if #available(iOS 17.2, macOS 14.2, *) {
                let converted = Self.variantDictionaryToFoundation(newValue)
                request.properties = converted.isEmpty ? nil : converted
            }
        }
    }

    @Export var recipientProperties: VariantDictionary {
        get {
            if #available(iOS 17.2, macOS 14.2, *) {
                guard let recipientProperties = request.recipientProperties else {
                    return VariantDictionary()
                }
                let result = VariantDictionary()
                for (player, properties) in recipientProperties {
                    result[player.gamePlayerID] = Variant(Self.foundationDictionaryToVariant(properties))
                }
                return result
            }
            return VariantDictionary()
        }
        set {
            if #available(iOS 17.2, macOS 14.2, *) {
                var recipientByID: [String: GameKit.GKPlayer] = [:]
                for player in request.recipients ?? [] {
                    recipientByID[player.gamePlayerID] = player
                    recipientByID[player.teamPlayerID] = player
                }

                var converted: [GameKit.GKPlayer: [String: Any]] = [:]
                for key in newValue.keys() {
                    guard let key else { continue }

                    let player: GameKit.GKPlayer?
                    if let wrappedPlayer = key.asObject(GKPlayer.self) {
                        player = wrappedPlayer.player
                    } else if let playerID = String(key) {
                        player = recipientByID[playerID]
                    } else {
                        player = nil
                    }

                    guard let player else { continue }
                    guard
                        let value = Self.variantToFoundation(newValue[key]) as? [String: Any]
                    else { continue }
                    converted[player] = value
                }
                request.recipientProperties = converted.isEmpty ? nil : converted
            }
        }
    }

    @Export var defaultNumberOfPlayers: Int {
        get { request.defaultNumberOfPlayers }
        set { request.defaultNumberOfPlayers = newValue }
    }

    @Callable
    static func max_players_allowed_for_match(forType: MatchType) -> Int {
        return GameKit.GKMatchRequest.maxPlayersAllowedForMatch(of: forType.toGameKit())
    }

    @Export var inviteMessage: String {
        get { request.inviteMessage ?? "" }
        set { request.inviteMessage = newValue }
    }
}
