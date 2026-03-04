import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKVoiceChat: RefCounted, @unchecked Sendable {
    var chat: GameKit.GKVoiceChat? {
        didSet {
            configureStateHandler()
        }
    }

    @Signal("player", "state") var player_state_changed: SignalWithArguments<GKPlayer, Int>

    convenience init(chat: GameKit.GKVoiceChat) {
        self.init()
        self.chat = chat
        configureStateHandler()
    }

    private func configureStateHandler() {
        guard let chat else { return }
        chat.playerVoiceChatStateDidChangeHandler = { [weak self] player, state in
            self?.player_state_changed.emit(GKPlayer(player: player), Int(state.rawValue))
        }
    }

    deinit {
        chat?.playerVoiceChatStateDidChangeHandler = { _, _ in }
    }

    @Callable
    static func is_voip_allowed() -> Bool {
        GameKit.GKVoiceChat.isVoIPAllowed()
    }

    @Callable
    func start() {
        chat?.start()
    }

    @Callable
    func stop() {
        chat?.stop()
    }

    @Callable
    func set_player_muted(player: GKPlayer, muted: Bool) {
        chat?.setPlayer(player.player, muted: muted)
    }

    @Export var isActive: Bool {
        get { chat?.isActive ?? false }
        set { chat?.isActive = newValue }
    }

    @Export var volume: Double {
        get { Double(chat?.volume ?? 0) }
        set { chat?.volume = Float(newValue) }
    }

    @Export var name: String {
        chat?.name ?? ""
    }

    @Export var players: VariantArray {
        let result = VariantArray()
        chat?.players.forEach {
            result.append(Variant(GKPlayer(player: $0)))
        }
        return result
    }
}
