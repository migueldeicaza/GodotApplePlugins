//
//  GKMatchmaker.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 3/3/26.
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKMatchmaker: RefCounted, @unchecked Sendable {
    @Signal("player", "is_reachable") var nearby_player_reachable:
        SignalWithArguments<GKPlayer, Bool>
    @Signal("player") var player_joining_group_activity: SignalWithArguments<GKPlayer>

    private var shared: GameKit.GKMatchmaker {
        GameKit.GKMatchmaker.shared()
    }

    private func unsupportedError(_ method: String) -> Variant? {
        let error = NSError(
            domain: GKErrorDomain,
            code: GameKit.GKError.Code.apiNotAvailable.rawValue,
            userInfo: [NSLocalizedDescriptionKey: "\(method) requires a newer OS version"]
        )
        return GKError.from(error)
    }

    private func wrapPlayers(_ players: [GameKit.GKPlayer]?) -> TypedArray<GKPlayer?> {
        let result = TypedArray<GKPlayer?>()
        players?.forEach { result.append(GKPlayer(player: $0)) }
        return result
    }

    @Callable
    func find_match(request: GKMatchRequest, callback: Callable) {
        shared.findMatch(for: request.request, withCompletionHandler: { match, error in
            if let match {
                _ = callback.call(Variant(GKMatch(match: match)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        })
    }

    @Callable
    func match_for_invite(invite: GKInvite, callback: Callable) {
        guard let invite = invite.invite else {
            _ = callback.call(nil, Variant("Invalid invite object"))
            return
        }

        shared.match(for: invite, completionHandler: { match, error in
            if let match {
                _ = callback.call(Variant(GKMatch(match: match)), nil)
            } else {
                _ = callback.call(nil, GKError.from(error))
            }
        })
    }

    @Callable
    func finish_matchmaking(match: GKMatch) {
        shared.finishMatchmaking(for: match.gkmatch)
    }

    @Callable
    func find_players(request: GKMatchRequest, callback: Callable) {
        shared.findPlayers(forHostedRequest: request.request, withCompletionHandler: { players, error in
            _ = callback.call(Variant(self.wrapPlayers(players)), GKError.from(error))
        })
    }

    @Callable
    func find_matched_players(request: GKMatchRequest, callback: Callable) {
        if #available(iOS 17.2, macOS 14.2, tvOS 17.2, visionOS 1.1, *) {
            shared.findMatchedPlayers(request.request, withCompletionHandler: { matchedPlayers, error in
                if let matchedPlayers {
                    _ = callback.call(Variant(GKMatchedPlayers(matchedPlayers: matchedPlayers)), nil)
                } else {
                    _ = callback.call(nil, GKError.from(error))
                }
            })
        } else {
            _ = callback.call(nil, unsupportedError("find_matched_players"))
        }
    }

    @Callable
    func add_players(match: GKMatch, request: GKMatchRequest, callback: Callable) {
        shared.addPlayers(to: match.gkmatch, matchRequest: request.request, completionHandler: { error in
            _ = callback.call(GKError.from(error))
        })
    }

    @Callable
    func query_activity(callback: Callable) {
        shared.queryActivity(completionHandler: { activity, error in
            _ = callback.call(Variant(activity), GKError.from(error))
        })
    }

    @Callable
    func query_queue_activity(queueName: String, callback: Callable) {
        if #available(iOS 17.2, macOS 14.2, *) {
            shared.queryQueueActivity(queueName, withCompletionHandler: { activity, error in
                _ = callback.call(Variant(activity), GKError.from(error))
            })
        } else {
            _ = callback.call(nil, unsupportedError("query_queue_activity"))
        }
    }

    @Callable
    func query_player_group_activity(groupID: Int, callback: Callable) {
        shared.queryPlayerGroupActivity(groupID, withCompletionHandler: { activity, error in
            _ = callback.call(Variant(activity), GKError.from(error))
        })
    }

    @Callable
    func cancel() {
        shared.cancel()
    }

    @Callable
    func cancel_pending_invite(player: GKPlayer) {
        shared.cancelPendingInvite(to: player.player)
    }

    @Callable
    func start_browsing_for_nearby_players() {
        shared.startBrowsingForNearbyPlayers(handler: { player, isReachable in
            self.nearby_player_reachable.emit(GKPlayer(player: player), isReachable)
        })
    }

    @Callable
    func stop_browsing_for_nearby_players() {
        shared.stopBrowsingForNearbyPlayers()
    }

    @Callable
    func start_group_activity() {
        #if !os(tvOS)
            if #available(iOS 16.2, macOS 13.1, *) {
                shared.startGroupActivity(playerHandler: { player in
                    self.player_joining_group_activity.emit(GKPlayer(player: player))
                })
            }
        #endif
    }

    @Callable
    func stop_group_activity() {
        #if !os(tvOS)
            if #available(iOS 16.2, macOS 13.1, *) {
                shared.stopGroupActivity()
            }
        #endif
    }
}
