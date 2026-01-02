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
        GKAchievementDescription.self,
        GKGameCenterViewController.self,
        GKLocalPlayer.self,
        GKLeaderboard.self,
        GKLeaderboardEntry.self,
        GKLeaderboardSet.self,
        GKMatch.self,
        GKMatchmakerViewController.self,
        GKMatchRequest.self,
        GKPlayer.self,
        GKSavedGame.self,

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
        ASAuthorizationController.self
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
        GKMatch.SendDataMode.self,
        GKMatchRequest.MatchType.self,

        ProductView.ViewStyle.self,
        StoreKitManager.StoreKitStatus.self,
        StoreKitManager.VerificationError.self,
        SubscriptionStoreView.ControlStyle.self,
        StoreProductSubscriptionOffer.OfferType.self,
        StoreProductSubscriptionPeriod.Unit.self,
        StoreSubscriptionInfoStatus.RenewalState.self,

        ASAuthorizationAppleIDCredential.UserDetectionStatus.self,
        ASAuthorizationAppleIDCredential.UserAgeRange.self
    ],
    registerDocs: true
)
