//
//  GKTurnBasedExchange.swift
//  GodotApplePlugins
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKTurnBasedExchange: RefCounted, @unchecked Sendable {
    var exchange: GameKit.GKTurnBasedExchange?

    convenience init(exchange: GameKit.GKTurnBasedExchange) {
        self.init()
        self.exchange = exchange
    }

    private static func wrapParticipants(_ participants: [GameKit.GKTurnBasedParticipant]?) -> VariantArray {
        let result = VariantArray()
        participants?.forEach {
            result.append(Variant(GKTurnBasedParticipant(participant: $0)))
        }
        return result
    }

    private static func wrapReplies(_ replies: [GameKit.GKTurnBasedExchangeReply]?) -> VariantArray {
        let result = VariantArray()
        replies?.forEach {
            result.append(Variant(GKTurnBasedExchangeReply(reply: $0)))
        }
        return result
    }

    private static func packedStringArrayToStringArray(_ arguments: PackedStringArray) -> [String] {
        var result: [String] = []
        for index in 0..<arguments.count {
            result.append(arguments[index])
        }
        return result
    }

    @Export var completionDate: Double {
        exchange?.completionDate?.timeIntervalSince1970 ?? 0
    }

    @Export var sendDate: Double {
        exchange?.sendDate.timeIntervalSince1970 ?? 0
    }

    @Export var timeoutDate: Double {
        exchange?.timeoutDate?.timeIntervalSince1970 ?? 0
    }

    @Export var sender: GKTurnBasedParticipant? {
        guard let sender = exchange?.sender else {
            return nil
        }
        return GKTurnBasedParticipant(participant: sender)
    }

    @Export var recipients: VariantArray {
        Self.wrapParticipants(exchange?.recipients)
    }

    @Export var data: PackedByteArray {
        exchange?.data?.toPackedByteArray() ?? PackedByteArray()
    }

    @Export var exchangeID: String {
        exchange?.exchangeID ?? ""
    }

    @Export var message: String {
        exchange?.message ?? ""
    }

    @Export var replies: VariantArray {
        Self.wrapReplies(exchange?.replies)
    }

    @Export var status: Int {
        Int(exchange?.status.rawValue ?? 0)
    }

    @Callable
    func cancel(localizableMessageKey: String, arguments: PackedStringArray, callback: Callable) {
        guard let exchange else {
            _ = callback.call(Variant("Invalid exchange object"))
            return
        }

        let arguments = Self.packedStringArrayToStringArray(arguments)
        exchange.cancel(withLocalizableMessageKey: localizableMessageKey, arguments: arguments) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func reply(
        localizableMessageKey: String, arguments: PackedStringArray, data: PackedByteArray,
        callback: Callable
    ) {
        guard let exchange else {
            _ = callback.call(Variant("Invalid exchange object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }

        let arguments = Self.packedStringArrayToStringArray(arguments)
        exchange.reply(
            withLocalizableMessageKey: localizableMessageKey,
            arguments: arguments,
            data: convertedData,
            completionHandler: { error in
                _ = callback.call(GKError.from(error))
            })
    }
}
