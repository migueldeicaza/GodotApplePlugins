import SwiftGodotRuntime

private func makeGodotApplePluginsGameCenterTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            GameCenterManager.self,
            GKAccessPoint.self,
            GKAchievement.self,
            GKAchievementChallenge.self,
            GKAchievementDescription.self,
            GKChallenge.self,
            GKChallengeDefinition.self,
            GKGameCenterViewController.self,
            GKGameActivity.self,
            GKGameActivityDefinition.self,
            GKLocalPlayer.self,
            GKLeaderboard.self,
            GKLeaderboardEntry.self,
            GKLeaderboardScore.self,
            GKLeaderboardSet.self,
            GKInvite.self,
            GKMatch.self,
            GKMatchmaker.self,
            GKMatchmakerViewController.self,
            GKMatchRequest.self,
            GKMatchedPlayers.self,
            GKNotificationBanner.self,
            GKScoreChallenge.self,
            GKTurnBasedExchange.self,
            GKTurnBasedExchangeReply.self,
            GKTurnBasedMatch.self,
            GKTurnBasedMatchmakerViewController.self,
            GKTurnBasedParticipant.self,
            GKVoiceChat.self,
            GKPlayer.self,
            GKSavedGame.self,
            GKError.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare Game Center registrations: \(error)")
    }
}

private let godotApplePluginsGameCenterTypes = makeGodotApplePluginsGameCenterTypes()

public let godotApplePluginsGameCenterMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsGameCenterTypes
)

public func godotApplePluginsGameCenterInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsGameCenterTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(GKAccessPoint.Location.self)
        registerEnum(GKGameCenterViewController.State.self)
        registerEnum(GKLeaderboard.AppleLeaderboardType.self)
        registerEnum(GKLeaderboard.TimeScope.self)
        registerEnum(GKLeaderboard.PlayerScope.self)
        registerEnum(GKMatch.SendDataMode.self)
        registerEnum(GKMatchRequest.MatchType.self)
        registerEnum(GKMatchRequest.InviteRecipientResponse.self)
        registerEnum(GKTurnBasedMatchmakerViewController.MatchmakingMode.self)
        registerEnum(GKError.Code.self)
    } else if level == .editor {
#if os(macOS)
        loadEmbeddedGameCenterDocs()
#endif
    }
}

public func godotApplePluginsGameCenterDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsGameCenterTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_game_center_start")
public func godotApplePluginsGameCenterStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsGameCenterInitialize,
        deInitHook: godotApplePluginsGameCenterDeinitialize,
        minimumInitializationLevel: godotApplePluginsGameCenterMinimumInitializationLevel
    )
    return 1
}
