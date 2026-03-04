import GameKit
//
//  GKMatchMakerViewController.swift
//  GodotApplePlugins
//
//
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
    import UIKit
#else
    import AppKit
#endif

@Godot
class GKMatchmakerViewController: RefCounted, @unchecked Sendable {
    enum MatchmakingMode: Int, CaseIterable {
        case DEFAULT
        case NEARBY_ONLY
        case AUTOMATCH_ONLY
        case INVITE_ONLY

        func toGameKit() -> GameKit.GKMatchmakingMode {
            switch self {
            case .DEFAULT: return .default
            case .NEARBY_ONLY: return .nearbyOnly
            case .AUTOMATCH_ONLY: return .automatchOnly
            case .INVITE_ONLY: return .inviteOnly
            }
        }

        static func from(_ value: GameKit.GKMatchmakingMode) -> MatchmakingMode {
            switch value {
            case .default: return .DEFAULT
            case .nearbyOnly: return .NEARBY_ONLY
            case .automatchOnly: return .AUTOMATCH_ONLY
            case .inviteOnly: return .INVITE_ONLY
            @unknown default: return .DEFAULT
            }
        }
    }

    private static func variantDictionaryToFoundation(_ dictionary: VariantDictionary) -> [String: Any] {
        var result: [String: Any] = [:]
        for key in dictionary.keys() {
            guard let key, let keyString = String(key) else { continue }
            guard let value = variantToFoundation(dictionary[key]) else { continue }
            result[keyString] = value
        }
        return result
    }

    private static func variantArrayToFoundation(_ array: VariantArray) -> [Any] {
        var result: [Any] = []
        for value in array {
            guard let converted = variantToFoundation(value) else { continue }
            result.append(converted)
        }
        return result
    }

    private static func variantToFoundation(_ variant: Variant?) -> Any? {
        guard let variant else { return nil }

        switch variant.gtype {
        case .bool:
            return Bool(variant)
        case .int:
            if let int64Value = Int64(variant) {
                return Int(int64Value)
            }
            return nil
        case .float:
            return Double(variant)
        case .string, .stringName:
            return String(variant)
        case .array:
            guard let array = VariantArray(variant) else { return nil }
            return variantArrayToFoundation(array)
        case .dictionary:
            guard let dictionary = VariantDictionary(variant) else { return nil }
            return variantDictionaryToFoundation(dictionary)
        case .packedByteArray:
            guard let bytes = PackedByteArray(variant) else { return nil }
            return bytes.asData()
        default:
            return nil
        }
    }

    class Proxy: NSObject, GameKit.GKMatchmakerViewControllerDelegate, GKLocalPlayerListener {
        func matchmakerViewControllerWasCancelled(
            _ viewController: GameKit.GKMatchmakerViewController
        ) {
            guard let base else { return }
            MainActor.assumeIsolated {
                #if os(macOS)
                    base.dialogController?.dismiss(viewController)
                #else
                    viewController.dismiss(animated: true)
                #endif
                base.cancelled.emit("")
            }
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController, didFailWithError error: any Error
        ) {
            GD.print("GKMVC: didFailWithError")
            base?.failed_with_error.emit(String(describing: error))
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController, didFind match: GameKit.GKMatch
        ) {
            base?.did_find_match.emit(GKMatch(match: match))
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            didFindHostedPlayers players: [GameKit.GKPlayer]
        ) {
            let result = VariantArray()
            for player in players {
                result.append(Variant(GKPlayer(player: player)))
            }
            base?.did_find_hosted_players.emit(result)
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            hostedPlayerDidAccept player: GameKit.GKPlayer
        ) {
            base?.hosted_player_did_accept.emit(GKPlayer(player: player))
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            getMatchPropertiesForRecipient recipient: GameKit.GKPlayer,
            withCompletionHandler completionHandler: @escaping ([String: Any]) -> Void
        ) {
            guard let base else {
                completionHandler([:])
                return
            }
            guard let callback = base.get_match_properties_for_recipient else {
                completionHandler([:])
                return
            }

            let value = callback.call(Variant(GKPlayer(player: recipient)))
            if let value, let dictionary = VariantDictionary(value) {
                completionHandler(GKMatchmakerViewController.variantDictionaryToFoundation(dictionary))
            } else {
                completionHandler([:])
            }
        }

        weak var base: GKMatchmakerViewController?
        init(_ base: GKMatchmakerViewController) {
            self.base = base
        }
    }

    @Signal("detail") var cancelled: SignalWithArguments<String>

    /// Matchmaking has failed with an error
    @Signal("message") var failed_with_error: SignalWithArguments<String>

    /// A peer-to-peer match has been found, the game should start
    @Signal("match") var did_find_match: SignalWithArguments<GKMatch>

    /// Players have been found for a server-hosted game, the game should start, receives an array of GKPlayers
    @Signal("players") var did_find_hosted_players: SignalWithArguments<VariantArray>
    @Signal("player") var hosted_player_did_accept: SignalWithArguments<GKPlayer>

    /// The view controller if we create it
    var vc: GameKit.GKMatchmakerViewController?
    /// Delegate class if the user is rolling his own
    var proxy: Proxy?
    @Export var get_match_properties_for_recipient: Callable?

    @Export var matchRequest: GKMatchRequest? {
        get {
            MainActor.assumeIsolated {
                guard let request = vc?.matchRequest else { return nil }
                let wrapped = GKMatchRequest()
                wrapped.request = request
                return wrapped
            }
        }
    }

    @Export var canStartWithMinimumPlayers: Bool {
        get {
            MainActor.assumeIsolated {
                if #available(iOS 15.0, macOS 12.0, tvOS 15.0, visionOS 1.0, *) {
                    return vc?.canStartWithMinimumPlayers ?? false
                }
                return false
            }
        }
        set {
            MainActor.assumeIsolated {
                if #available(iOS 15.0, macOS 12.0, tvOS 15.0, visionOS 1.0, *), let vc {
                    vc.canStartWithMinimumPlayers = newValue
                }
            }
        }
    }

    @Export(.enum) var matchmakingMode: MatchmakingMode {
        get {
            MainActor.assumeIsolated {
                if #available(iOS 14.0, macOS 11.0, tvOS 14.0, visionOS 1.0, *), let vc {
                    return MatchmakingMode.from(vc.matchmakingMode)
                }
                return .DEFAULT
            }
        }
        set {
            MainActor.assumeIsolated {
                if #available(iOS 14.0, macOS 11.0, tvOS 14.0, visionOS 1.0, *), let vc {
                    vc.matchmakingMode = newValue.toGameKit()
                }
            }
        }
    }

    @Export var isHosted: Bool {
        get {
            MainActor.assumeIsolated {
                vc?.isHosted ?? false
            }
        }
        set {
            MainActor.assumeIsolated {
                vc?.isHosted = newValue
            }
        }
    }

    #if os(macOS)
        /// When the user triggers the presentation, on macOS, we keep track of it
        var dialogController: GKDialogController? = nil
    #endif

    @MainActor
    private static func make_wrapper(for controller: GameKit.GKMatchmakerViewController) -> GKMatchmakerViewController {
        let wrapper = GKMatchmakerViewController()
        let proxy = Proxy(wrapper)
        wrapper.vc = controller
        wrapper.proxy = proxy
        controller.matchmakerDelegate = proxy
        return wrapper
    }

    /// Returns a view controller for the specified request, configure the various callbacks, and then
    /// call `present` on it.
    @Callable static func create_controller(request: GKMatchRequest) -> GKMatchmakerViewController?
    {
        MainActor.assumeIsolated {
            guard let controller = GameKit.GKMatchmakerViewController(matchRequest: request.request) else { return nil }
            return make_wrapper(for: controller)
        }
    }

    @Callable static func create_controller_from_invite(invite: GKInvite) -> GKMatchmakerViewController?
    {
        MainActor.assumeIsolated {
            guard let invite = invite.invite else { return nil }
            guard let controller = GameKit.GKMatchmakerViewController(invite: invite) else { return nil }
            return make_wrapper(for: controller)
        }
    }

    // This is used for the custom request that is vastly simpler than rolling your own
    class RequestMatchDelegate: NSObject, GameKit.GKMatchmakerViewControllerDelegate,
        @unchecked Sendable
    {
        #if os(macOS)
            var dialogController: GKDialogController?
        #endif
        private let callback: Callable
        let done: () -> Void
        init(_ callback: Callable, done: @escaping () -> Void = {}) {
            self.callback = callback
            self.done = done
        }

        func matchmakerViewController(
            _ viewController: GameKit.GKMatchmakerViewController,
            didFind match: GameKit.GKMatch
        ) {
            _ = self.callback.call(Variant(GKMatch(match: match)), nil)
        }

        func matchmakerViewControllerWasCancelled(
            _ source: GameKit.GKMatchmakerViewController
        ) {
            MainActor.assumeIsolated {
                #if os(iOS)
                    source.dismiss(animated: true)
                #else
                    dialogController?.dismiss(source)

                #endif
                let error = NSError(
                    domain: GKErrorDomain, code: GKError.Code.CANCELLED.rawValue,
                    userInfo: [NSLocalizedDescriptionKey: "Cancelled"])
                _ = self.callback.call(nil, GKError.from(error))
                done()
            }
        }

        func matchmakerViewController(
            _ source: GameKit.GKMatchmakerViewController,
            didFailWithError: (any Error)
        ) {
            _ = self.callback.call(nil, GKError.from(didFailWithError))
            done()
        }
    }

    /// Convenience method that is a version that sets up `create_controller` and calls the callback
    /// with two arguments, the first is the match on success, and the second is an error on failure, which can be
    /// one of the following strings: "cancelled",
    @Callable static func request_match(request: GKMatchRequest, callback: Callable) {
        MainActor.assumeIsolated {
            if let vc = GameKit.GKMatchmakerViewController(matchRequest: request.request) {
                var hold: RequestMatchDelegate?

                hold = RequestMatchDelegate(
                    callback,
                    done: {
                        hold = nil
                    })
                vc.matchmakerDelegate = hold
                GKMatchmakerViewController.present(controller: vc) {
                    #if os(macOS)
                        hold?.dialogController = $0 as? GKDialogController
                    #endif
                }
            }
        }
    }

    @Callable func present() {
        guard let vc else {
            return
        }
        GKMatchmakerViewController.present(controller: vc) { v in
            #if os(macOS)
                dialogController = v as? GKDialogController
            #endif
        }
    }

    @Callable
    func set_hosted_player_did_connect(player: GKPlayer, didConnect: Bool) {
        MainActor.assumeIsolated {
            vc?.setHostedPlayer(player.player, didConnect: didConnect)
        }
    }

    @Callable
    func add_players_to_match(match: GKMatch) {
        MainActor.assumeIsolated {
            vc?.addPlayers(to: match.gkmatch)
        }
    }

    static func present(
        controller: GameKit.GKMatchmakerViewController, track: @MainActor (AnyObject) -> Void
    ) {
        MainActor.assumeIsolated {
            #if os(iOS)
                presentOnTop(controller)
            #else
                let dialogController = GKDialogController.shared()
                dialogController.parentWindow = NSApplication.shared.mainWindow
                dialogController.present(controller)
                track(dialogController)
            #endif
        }
    }
}
