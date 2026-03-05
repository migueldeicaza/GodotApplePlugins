import Foundation
import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKMatchedPlayers: RefCounted, @unchecked Sendable {
    var rawMatchedPlayers: AnyObject?

    @available(iOS 17.2, macOS 14.2, tvOS 17.2, visionOS 1.1, *)
    convenience init(matchedPlayers: GameKit.GKMatchedPlayers) {
        self.init()
        self.rawMatchedPlayers = matchedPlayers
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

    @Export var properties: VariantDictionary {
        if #available(iOS 17.2, macOS 14.2, tvOS 17.2, visionOS 1.1, *),
            let matchedPlayers = rawMatchedPlayers as? GameKit.GKMatchedPlayers,
            let properties = matchedPlayers.properties
        {
            return Self.foundationDictionaryToVariant(properties)
        }
        return VariantDictionary()
    }

    @Export var players: TypedArray<GKPlayer?> {
        let result = TypedArray<GKPlayer?>()
        if #available(iOS 17.2, macOS 14.2, tvOS 17.2, visionOS 1.1, *),
            let matchedPlayers = rawMatchedPlayers as? GameKit.GKMatchedPlayers
        {
            matchedPlayers.players.forEach {
                result.append(GKPlayer(player: $0))
            }
        }
        return result
    }

    @Export var playerProperties: VariantDictionary {
        if #available(iOS 17.2, macOS 14.2, tvOS 17.2, visionOS 1.1, *),
            let matchedPlayers = rawMatchedPlayers as? GameKit.GKMatchedPlayers,
            let playerProperties = matchedPlayers.playerProperties
        {
            let result = VariantDictionary()
            for (player, properties) in playerProperties {
                result[player.gamePlayerID] = Variant(Self.foundationDictionaryToVariant(properties))
            }
            return result
        }
        return VariantDictionary()
    }
}
