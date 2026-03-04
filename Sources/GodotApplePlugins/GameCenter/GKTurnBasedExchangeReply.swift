//
//  GKTurnBasedExchangeReply.swift
//  GodotApplePlugins
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKTurnBasedExchangeReply: RefCounted, @unchecked Sendable {
    var reply: GameKit.GKTurnBasedExchangeReply?

    convenience init(reply: GameKit.GKTurnBasedExchangeReply) {
        self.init()
        self.reply = reply
    }

    @Export var data: PackedByteArray {
        reply?.data?.toPackedByteArray() ?? PackedByteArray()
    }

    @Export var message: String {
        reply?.message ?? ""
    }

    @Export var recipient: GKTurnBasedParticipant? {
        guard let recipient = reply?.recipient else {
            return nil
        }
        return GKTurnBasedParticipant(participant: recipient)
    }

    @Export var replyDate: Double {
        reply?.replyDate.timeIntervalSince1970 ?? 0
    }
}
