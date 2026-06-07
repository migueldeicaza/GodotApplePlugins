#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedGameCenterDocs() {
    _ = loadEmbeddedGameCenterDocsOnce
}

private let loadEmbeddedGameCenterDocsOnce: Void = {
    [
        PackageResources.GKAccessPoint_xml,
        PackageResources.GKAchievement_xml,
        PackageResources.GKAchievementChallenge_xml,
        PackageResources.GKAchievementDescription_xml,
        PackageResources.GKChallenge_xml,
        PackageResources.GKChallengeDefinition_xml,
        PackageResources.GKError_xml,
        PackageResources.GKGameActivity_xml,
        PackageResources.GKGameActivityDefinition_xml,
        PackageResources.GKGameCenterViewController_xml,
        PackageResources.GKInvite_xml,
        PackageResources.GKLeaderboard_xml,
        PackageResources.GKLeaderboardEntry_xml,
        PackageResources.GKLeaderboardScore_xml,
        PackageResources.GKLeaderboardSet_xml,
        PackageResources.GKLocalPlayer_xml,
        PackageResources.GKMatch_xml,
        PackageResources.GKMatchRequest_xml,
        PackageResources.GKMatchmaker_xml,
        PackageResources.GKMatchmakerViewController_xml,
        PackageResources.GKNotificationBanner_xml,
        PackageResources.GKPlayer_xml,
        PackageResources.GKSavedGame_xml,
        PackageResources.GKScoreChallenge_xml,
        PackageResources.GKTurnBasedExchange_xml,
        PackageResources.GKTurnBasedExchangeReply_xml,
        PackageResources.GKTurnBasedMatch_xml,
        PackageResources.GKTurnBasedMatchmakerViewController_xml,
        PackageResources.GKTurnBasedParticipant_xml,
        PackageResources.GKVoiceChat_xml,
        PackageResources.GameCenterManager_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
