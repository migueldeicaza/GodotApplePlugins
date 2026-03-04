//
//  GodotApplePlugins.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/14/25.
//

import SwiftGodotRuntime

#initSwiftExtension(
    cdecl: "godot_apple_plugins_start",
    types: [
        AVAudioSession.self,

        Foundation.self,
        AppleURL.self,

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
        GKLeaderboardScore.self,
        GKLeaderboardEntry.self,
        GKLeaderboardSet.self,
        GKInvite.self,
        GKMatch.self,
        GKMatchmaker.self,
        GKMatchmakerViewController.self,
        GKMatchRequest.self,
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

        ProductView.self,
        StoreProduct.self,
        StoreProductPurchaseOption.self,
        StoreProductSubscriptionOffer.self,
        StoreProductPaymentMode.self,
        StoreProductSubscriptionPeriod.self,
        StoreSubscriptionInfo.self,
        StoreSubscriptionInfoStatus.self,
        StoreSubscriptionInfoRenewalInfo.self,
        StoreTransaction.self,
        StoreKitManager.self,

        StoreView.self,
        SubscriptionOfferView.self,
        SubscriptionStoreView.self,

        AppleFilePicker.self,

        ASAuthorizationAppleIDCredential.self,
        ASPasswordCredential.self,
        ASAuthorizationController.self,
        ASWebAuthenticationSession.self,
    ],
    enums: [
        AVAudioSession.CategoryOptions.self,
        AVAudioSession.RouteSharingPolicy.self,
        AVAudioSession.SessionCategory.self,
        AVAudioSession.SessionMode.self,

        GKAccessPoint.Location.self,
        GKGameCenterViewController.State.self,
        GKLeaderboard.AppleLeaderboardType.self,
        GKLeaderboard.TimeScope.self,
        GKLeaderboard.PlayerScope.self,
        GKChallenge.ChallengeState.self,
        GKChallenge.ChallengeType.self,
        GKMatch.SendDataMode.self,
        GKMatchRequest.MatchType.self,
        GKMatchRequest.InviteRecipientResponse.self,
        GKTurnBasedMatchmakerViewController.MatchmakingMode.self,
        GKError.Code.self,

        ProductView.ViewStyle.self,
        StoreKitManager.StoreKitStatus.self,
        StoreKitManager.VerificationError.self,
        SubscriptionStoreView.ControlStyle.self,
        StoreProductSubscriptionOffer.OfferType.self,
        StoreProductSubscriptionPeriod.Unit.self,
        StoreSubscriptionInfoStatus.RenewalState.self,

        ASAuthorizationAppleIDCredential.UserDetectionStatus.self,
        ASAuthorizationAppleIDCredential.UserAgeRange.self,
    ],
    registerDocs: true
)
