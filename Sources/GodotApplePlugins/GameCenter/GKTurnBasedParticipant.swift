//
//  GKTurnBasedParticipant.swift
//  GodotApplePlugins
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKTurnBasedParticipant: RefCounted, @unchecked Sendable {
    var participant: GameKit.GKTurnBasedParticipant?

    convenience init(participant: GameKit.GKTurnBasedParticipant) {
        self.init()
        self.participant = participant
    }

    @Export var player: GKPlayer? {
        guard let player = participant?.player else {
            return nil
        }
        return GKPlayer(player: player)
    }

    @Export var status: Int {
        Int(participant?.status.rawValue ?? 0)
    }

    @Export var lastTurnDate: Double {
        participant?.lastTurnDate?.timeIntervalSince1970 ?? 0
    }

    @Export var timeoutDate: Double {
        participant?.timeoutDate?.timeIntervalSince1970 ?? 0
    }

    @Export var matchOutcome: Int {
        get {
            Int(participant?.matchOutcome.rawValue ?? 0)
        }
        set {
            guard let participant else { return }
            guard let outcome = GameKit.GKTurnBasedMatch.Outcome(rawValue: newValue) else { return }
            participant.matchOutcome = outcome
        }
    }
}
