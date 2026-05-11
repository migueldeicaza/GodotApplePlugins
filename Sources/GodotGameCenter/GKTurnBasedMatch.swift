//
//  GKTurnBasedMatch.swift
//  GodotApplePlugins
//

@preconcurrency import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKTurnBasedMatch: RefCounted, @unchecked Sendable {
    var match: GameKit.GKTurnBasedMatch?

    convenience init(match: GameKit.GKTurnBasedMatch) {
        self.init()
        self.match = match
    }

    private static func wrapParticipants(_ participants: [GameKit.GKTurnBasedParticipant]?) -> VariantArray {
        let result = VariantArray()
        participants?.forEach {
            result.append(Variant(GKTurnBasedParticipant(participant: $0)))
        }
        return result
    }

    private static func wrapExchanges(_ exchanges: [GameKit.GKTurnBasedExchange]?) -> VariantArray {
        let result = VariantArray()
        exchanges?.forEach {
            result.append(Variant(GKTurnBasedExchange(exchange: $0)))
        }
        return result
    }

    private static func variantArrayToParticipants(_ values: VariantArray) -> [GameKit.GKTurnBasedParticipant] {
        var participants: [GameKit.GKTurnBasedParticipant] = []
        for value in values {
            guard let value, let wrapped = value.asObject(GKTurnBasedParticipant.self) else { continue }
            guard let participant = wrapped.participant else { continue }
            participants.append(participant)
        }
        return participants
    }

    private static func variantArrayToExchanges(_ values: VariantArray) -> [GameKit.GKTurnBasedExchange] {
        var exchanges: [GameKit.GKTurnBasedExchange] = []
        for value in values {
            guard let value, let wrapped = value.asObject(GKTurnBasedExchange.self) else { continue }
            guard let exchange = wrapped.exchange else { continue }
            exchanges.append(exchange)
        }
        return exchanges
    }

    private static func packedStringArrayToStringArray(_ arguments: PackedStringArray) -> [String] {
        var result: [String] = []
        for index in 0..<arguments.count {
            result.append(arguments[index])
        }
        return result
    }

    @Export var participants: VariantArray {
        Self.wrapParticipants(match?.participants)
    }

    @Export var currentParticipant: GKTurnBasedParticipant? {
        guard let currentParticipant = match?.currentParticipant else {
            return nil
        }
        return GKTurnBasedParticipant(participant: currentParticipant)
    }

    @Export var isActivePlayer: Bool {
        guard
            let currentParticipant = match?.currentParticipant,
            let currentPlayer = currentParticipant.player
        else {
            return false
        }
        return currentPlayer.gamePlayerID == GameKit.GKLocalPlayer.local.gamePlayerID
    }

    @Export var matchData: PackedByteArray {
        match?.matchData?.toPackedByteArray() ?? PackedByteArray()
    }

    @Export var matchDataMaximumSize: Int {
        match?.matchDataMaximumSize ?? 0
    }

    @Export var message: String {
        match?.message ?? ""
    }

    @Export var matchID: String {
        match?.matchID ?? ""
    }

    @Export var creationDate: Double {
        match?.creationDate.timeIntervalSince1970 ?? 0
    }

    @Export var status: Int {
        Int(match?.status.rawValue ?? 0)
    }

    @Export var activeExchanges: VariantArray {
        Self.wrapExchanges(match?.activeExchanges)
    }

    @Export var completedExchanges: VariantArray {
        Self.wrapExchanges(match?.completedExchanges)
    }

    @Export var exchanges: VariantArray {
        Self.wrapExchanges(match?.exchanges)
    }

    @Export var exchangeDataMaximumSize: Int {
        match?.exchangeDataMaximumSize ?? 0
    }

    @Export var exchangeMaxInitiatedExchangesPerPlayer: Int {
        match?.exchangeMaxInitiatedExchangesPerPlayer ?? 0
    }

    @Callable
    func load_match_data(callback: Callable) {
        guard let match else {
            _ = callback.call(Variant(PackedByteArray()), Variant("Invalid match object"))
            return
        }

        match.loadMatchData { data, error in
            let payload = data?.toPackedByteArray() ?? PackedByteArray()
            _ = callback.call(Variant(payload), GKError.from(error))
        }
    }

    @Callable
    func save_current_turn(data: PackedByteArray, callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }

        match.saveCurrentTurn(withMatch: convertedData) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func end_turn(nextParticipants: VariantArray, timeout: Double, data: PackedByteArray, callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }

        let participants = Self.variantArrayToParticipants(nextParticipants)
        match.endTurn(
            withNextParticipants: participants,
            turnTimeout: TimeInterval(timeout),
            match: convertedData,
            completionHandler: { error in
                _ = callback.call(GKError.from(error))
            })
    }

    @Callable
    func participant_quit_in_turn(
        outcome: Int, nextParticipants: VariantArray, timeout: Double, data: PackedByteArray,
        callback: Callable
    ) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }
        guard let outcome = GameKit.GKTurnBasedMatch.Outcome(rawValue: outcome) else {
            _ = callback.call(Variant("Invalid outcome value"))
            return
        }

        let participants = Self.variantArrayToParticipants(nextParticipants)
        match.participantQuitInTurn(
            with: outcome,
            nextParticipants: participants,
            turnTimeout: TimeInterval(timeout),
            match: convertedData,
            completionHandler: { error in
                _ = callback.call(GKError.from(error))
            })
    }

    @Callable
    func participant_quit_out_of_turn(outcome: Int, callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let outcome = GameKit.GKTurnBasedMatch.Outcome(rawValue: outcome) else {
            _ = callback.call(Variant("Invalid outcome value"))
            return
        }

        match.participantQuitOutOfTurn(with: outcome) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func end_match_in_turn(data: PackedByteArray, callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }

        match.endMatchInTurn(withMatch: convertedData) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func remove(callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }

        match.remove { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func save_merged_match(data: PackedByteArray, resolvedExchanges: VariantArray, callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(Variant("Could not convert PackedByteArray to Data"))
            return
        }

        let exchanges = Self.variantArrayToExchanges(resolvedExchanges)
        match.saveMergedMatch(convertedData, withResolvedExchanges: exchanges) { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func send_exchange(
        participants: VariantArray,
        data: PackedByteArray,
        localizableMessageKey: String,
        arguments: PackedStringArray,
        timeout: Double,
        callback: Callable
    ) {
        guard let match else {
            _ = callback.call(nil, Variant("Invalid match object"))
            return
        }
        guard let convertedData = data.asData() else {
            _ = callback.call(nil, Variant("Could not convert PackedByteArray to Data"))
            return
        }

        let participants = Self.variantArrayToParticipants(participants)
        let arguments = Self.packedStringArrayToStringArray(arguments)
        match.sendExchange(
            to: participants,
            data: convertedData,
            localizableMessageKey: localizableMessageKey,
            arguments: arguments,
            timeout: TimeInterval(timeout),
            completionHandler: { exchange, error in
                if let exchange {
                    _ = callback.call(Variant(GKTurnBasedExchange(exchange: exchange)), nil)
                } else {
                    _ = callback.call(nil, GKError.from(error))
                }
            })
    }

    @Callable
    func send_reminder(
        participants: VariantArray,
        localizableMessageKey: String,
        arguments: PackedStringArray,
        callback: Callable
    ) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }

        let participants = Self.variantArrayToParticipants(participants)
        let arguments = Self.packedStringArrayToStringArray(arguments)
        match.sendReminder(
            to: participants,
            localizableMessageKey: localizableMessageKey,
            arguments: arguments,
            completionHandler: { error in
                _ = callback.call(GKError.from(error))
            })
    }

    @Callable
    func set_localizable_message_with_key(key: String, arguments: PackedStringArray) {
        guard let match else { return }

        let argumentList = Self.packedStringArrayToStringArray(arguments)
        match.setLocalizableMessageWithKey(
            key,
            arguments: argumentList.isEmpty ? nil : argumentList
        )
    }

    @Callable
    func accept_invite(callback: Callable) {
        guard let match else {
            _ = callback.call(nil, Variant("Invalid match object"))
            return
        }

        match.acceptInvite { acceptedMatch, error in
            if let acceptedMatch {
                _ = callback.call(Variant(GKTurnBasedMatch(match: acceptedMatch)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        }
    }

    @Callable
    func decline_invite(callback: Callable) {
        guard let match else {
            _ = callback.call(Variant("Invalid match object"))
            return
        }

        match.declineInvite { error in
            _ = callback.call(GKError.from(error))
        }
    }

    @Callable
    func rematch(callback: Callable) {
        guard let match else {
            _ = callback.call(nil, Variant("Invalid match object"))
            return
        }

        match.rematch { rematched, error in
            if let rematched {
                _ = callback.call(Variant(GKTurnBasedMatch(match: rematched)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        }
    }

    @Callable
    static func exchange_timeout_default() -> Double {
        GKExchangeTimeoutDefault
    }

    @Callable
    static func exchange_timeout_none() -> Double {
        GKExchangeTimeoutNone
    }

    @Callable
    static func turn_timeout_default() -> Double {
        GKTurnTimeoutDefault
    }

    @Callable
    static func turn_timeout_none() -> Double {
        GKTurnTimeoutNone
    }

    @Callable
    static func load_matches(callback: Callable) {
        GameKit.GKTurnBasedMatch.loadMatches { matches, error in
            let wrapped = VariantArray()
            matches?.forEach {
                wrapped.append(Variant(GKTurnBasedMatch(match: $0)))
            }
            _ = callback.call(Variant(wrapped), GKError.from(error))
        }
    }

    @Callable
    static func load(matchID: String, callback: Callable) {
        GameKit.GKTurnBasedMatch.load(withID: matchID) { match, error in
            if let match {
                _ = callback.call(Variant(GKTurnBasedMatch(match: match)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        }
    }

    @Callable
    static func find(request: GKMatchRequest, callback: Callable) {
        GameKit.GKTurnBasedMatch.find(for: request.request) { match, error in
            if let match {
                _ = callback.call(Variant(GKTurnBasedMatch(match: match)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        }
    }

}
