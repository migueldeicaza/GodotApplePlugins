import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKLeaderboardScore: RefCounted, @unchecked Sendable {
    var score: GameKit.GKLeaderboardScore?

    convenience init(score: GameKit.GKLeaderboardScore) {
        self.init()
        self.score = score
    }

    @Export var context: Int {
        get { Int(score?.context ?? 0) }
        set { score?.context = newValue }
    }

    @Export var leaderboardID: String {
        get { score?.leaderboardID ?? "" }
        set { score?.leaderboardID = newValue }
    }

    @Export var player: GKPlayer? {
        get {
            guard let player = score?.player else { return nil }
            return GKPlayer(player: player)
        }
        set {
            if let player = newValue?.player {
                score?.player = player
            }
        }
    }

    @Export var value: Int {
        get { score?.value ?? 0 }
        set { score?.value = newValue }
    }
}
