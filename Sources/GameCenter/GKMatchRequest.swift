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

enum MatchType: Int, CaseIterable {
    case peerToPeer
    case hosted
    case turnBased
    var gamekit: GameKit.GKMatchType {
        switch self {
        case .peerToPeer: return .peerToPeer
        case .hosted: return .hosted
        case .turnBased: return .turnBased
        }
    }
}

@Godot
class GKMatchRequest: RefCounted, @unchecked Sendable {
    var request = GameKit.GKMatchRequest()

    @Export var minPlayers: Int {
        get { request.minPlayers }
        set { request.minPlayers = newValue }
    }

    @Export var maxPlayers: Int {
        get { request.maxPlayers }
        set { request.maxPlayers = newValue }
    }
    @Export var defaultNumberOfPlayers: Int {
        get { request.defaultNumberOfPlayers }
        set { request.defaultNumberOfPlayers = newValue }
    }

    // TODO: add support for enumerations in both _argumentPropInfo and fetchArguments
    @Callable
    static func maxPlayersAllowedForMatch(forType: Int) -> Int {
        GameKit.GKMatchRequest.maxPlayersAllowedForMatch(of: MatchType(rawValue: forType)!.gamekit)
    }

    @Export var inviteMessage: String {
        get { request.inviteMessage ?? "" }
        set { request.inviteMessage = newValue }
    }
}
//
// This is one part missing
//extension RawArguments {
//    func fetchArgument(at: Int) -> MatchType {
//        let v: Int = fetchArgument(at: at)
//        return MatchType(rawValue: v)!
//    }
//}
