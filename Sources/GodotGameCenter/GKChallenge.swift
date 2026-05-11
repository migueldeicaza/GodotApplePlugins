import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKChallenge: RefCounted, @unchecked Sendable {
    var challenge: GameKit.GKChallenge?

    enum ChallengeState: Int, CaseIterable {
        case INVALID
        case PENDING
        case COMPLETED
        case DECLINED

        static func from(_ state: GameKit.GKChallengeState) -> ChallengeState {
            switch state {
            case .invalid: return .INVALID
            case .pending: return .PENDING
            case .completed: return .COMPLETED
            case .declined: return .DECLINED
            @unknown default: return .INVALID
            }
        }
    }

    enum ChallengeType: Int, CaseIterable {
        case SCORE
        case ACHIEVEMENT
        case UNKNOWN
    }

    convenience init(challenge: GameKit.GKChallenge) {
        self.init()
        self.challenge = challenge
    }

    static func wrap(_ challenge: GameKit.GKChallenge) -> GKChallenge {
        if let score = challenge as? GameKit.GKScoreChallenge {
            return GKScoreChallenge(challenge: score)
        }
        if let achievement = challenge as? GameKit.GKAchievementChallenge {
            return GKAchievementChallenge(challenge: achievement)
        }
        return GKChallenge(challenge: challenge)
    }

    @Export var issuingPlayer: GKPlayer? {
        guard let player = challenge?.issuingPlayer else {
            return nil
        }
        return GKPlayer(player: player)
    }

    @Export var receivingPlayer: GKPlayer? {
        guard let player = challenge?.receivingPlayer else {
            return nil
        }
        return GKPlayer(player: player)
    }

    @Export var message: String {
        challenge?.message ?? ""
    }

    @Export(.enum) var state: ChallengeState {
        ChallengeState.from(challenge?.state ?? .invalid)
    }

    @Export var issueDate: Double {
        challenge?.issueDate.timeIntervalSince1970 ?? 0
    }

    @Export var completionDate: Double {
        challenge?.completionDate?.timeIntervalSince1970 ?? 0
    }

    @Export(.enum) var challengeType: ChallengeType {
        if challenge is GameKit.GKScoreChallenge {
            return .SCORE
        }
        if challenge is GameKit.GKAchievementChallenge {
            return .ACHIEVEMENT
        }
        return .UNKNOWN
    }

    @Callable
    func decline() {
        challenge?.decline()
    }

    @Callable
    static func load_received_challenges(callback: Callable) {
        GameKit.GKChallenge.loadReceivedChallenges { challenges, error in
            let wrapped = VariantArray()
            challenges?.forEach {
                wrapped.append(Variant(wrap($0)))
            }
            _ = callback.call(Variant(wrapped), GKError.from(error))
        }
    }
}

@Godot
class GKScoreChallenge: GKChallenge, @unchecked Sendable {
    convenience init(challenge: GameKit.GKScoreChallenge) {
        self.init()
        self.challenge = challenge
    }

    private var scoreChallenge: GameKit.GKScoreChallenge? {
        challenge as? GameKit.GKScoreChallenge
    }

    @Export var score: Int {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            return scoreChallenge?.leaderboardEntry?.score ?? 0
        } else {
            return Int(scoreChallenge?.score?.value ?? 0)
        }
    }

    @Export var formattedScore: String {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            return scoreChallenge?.leaderboardEntry?.formattedScore ?? ""
        } else {
            return scoreChallenge?.score?.formattedValue ?? ""
        }
    }

    @Export var rank: Int {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            return scoreChallenge?.leaderboardEntry?.rank ?? 0
        } else {
            return scoreChallenge?.score?.rank ?? 0
        }
    }

    @Export var context: Int {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            return Int(scoreChallenge?.leaderboardEntry?.context ?? 0)
        } else {
            return Int(scoreChallenge?.score?.context ?? 0)
        }
    }

    @Export var player: GKPlayer? {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            guard let player = scoreChallenge?.leaderboardEntry?.player else {
                return nil
            }
            return GKPlayer(player: player)
        } else {
            guard let player = scoreChallenge?.score?.player else {
                return nil
            }
            return GKPlayer(player: player)
        }
    }

    @Export var leaderboardIdentifier: String {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            return ""
        } else {
            return scoreChallenge?.score?.leaderboardIdentifier ?? ""
        }
    }

    @Export var leaderboardEntry: GKLeaderboardEntry? {
        if #available(iOS 17.4, macOS 14.4, tvOS 17.4, visionOS 1.1, *) {
            guard let entry = scoreChallenge?.leaderboardEntry else {
                return nil
            }
            return GKLeaderboardEntry(store: entry)
        }
        return nil
    }
}

@Godot
class GKAchievementChallenge: GKChallenge, @unchecked Sendable {
    convenience init(challenge: GameKit.GKAchievementChallenge) {
        self.init()
        self.challenge = challenge
    }

    private var achievementChallenge: GameKit.GKAchievementChallenge? {
        challenge as? GameKit.GKAchievementChallenge
    }

    @Export var achievement: GKAchievement? {
        guard let achievement = achievementChallenge?.achievement else {
            return nil
        }
        return GKAchievement(achievement: achievement)
    }
}
