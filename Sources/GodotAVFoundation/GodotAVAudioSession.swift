//
//  GodotAVAudioSession.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/15/25.
//
import SwiftGodotRuntime

extension AVAudioSession {
    public enum SessionCategory: Int, CaseIterable {
        case AMBIENT
        case MULTI_ROUTE
        case PLAY_AND_RECORD
        case PLAYBACK
        case RECORD
        case SOLO_AMBIENT
        case UNKNOWN
    }

    public enum SessionMode: Int, CaseIterable {
        case DEFAULT
        case GAME_CHAT
        case MEASUREMENT
        case MOVIE_PLAYBACK
        case SPOKEN_AUDIO
        case VIDEO_CHAT
        case VOICE_CHAT
        case VOICE_PROMPT
    }

    public enum RouteSharingPolicy: Int, CaseIterable {
        case ROUTE_SHARING_DEFAULT
        case LONG_FORM_AUDIO
        case INDEPENDENT
        case LONG_FORM
    }
    
    public enum CategoryOptions: Int, CaseIterable {
        case MIX_WITH_OTHERS = 1
        case DUCK_OTHERS = 2
        case ALLOW_BLUETOOTH = 4
        case DEFAULT_TO_SPEAKER = 8
        case INTERRUPT_SPOKEN_AUDIO_AND_MIX_WITH_OTHERS = 17
        case ALLOW_BLUETOOTH_A2DP = 32
        case ALLOW_AIRPLAY = 64
        case OVERRIDE_MUTED_MICROPHONE_INTERRUPTION = 128
    }
}
#if os(iOS) || os(tvOS) || os(visionOS)
import AVFoundation

extension AVAudioSession.SessionCategory {
    func toAVAudioSessionCategory() -> AVFoundation.AVAudioSession.Category {
        switch self {
        case .AMBIENT:
            return .ambient
        case .MULTI_ROUTE:
            return .multiRoute
        case .PLAY_AND_RECORD:
            return .playAndRecord
        case .PLAYBACK:
            return .playback
        case .RECORD:
            return .record
        case .SOLO_AMBIENT:
            return .soloAmbient
        case .UNKNOWN:
            return .ambient
        }
    }
}

extension AVAudioSession.SessionMode {
    func toAVAudioSessionMode() -> AVFoundation.AVAudioSession.Mode {
        switch self {
        case .DEFAULT: return .default
        case .GAME_CHAT: return .gameChat
        case .MEASUREMENT: return .measurement
        case .MOVIE_PLAYBACK: return .moviePlayback
        case .SPOKEN_AUDIO: return .spokenAudio
        case .VIDEO_CHAT: return .videoChat
        case .VOICE_CHAT: return .voiceChat
        case .VOICE_PROMPT: return .voicePrompt
        }
    }
}

extension AVAudioSession.RouteSharingPolicy {
    func toAVAudioSessionRouteSharingPolicy() -> AVFoundation.AVAudioSession.RouteSharingPolicy {
        switch self {
        case .ROUTE_SHARING_DEFAULT: return .default
        case .LONG_FORM_AUDIO: return .longFormAudio
        case .INDEPENDENT: return .independent
        case .LONG_FORM: return .longFormAudio
        }
    }
}

@Godot
public class AVAudioSession: RefCounted, @unchecked Sendable {
    @Export var currentCategory: SessionCategory {
        get {
            switch AVFoundation.AVAudioSession.sharedInstance().category {
            case .ambient:
                return .AMBIENT
            case .multiRoute:
                return .MULTI_ROUTE
            case .playback:
                return .PLAYBACK
            case .playAndRecord:
                return .PLAY_AND_RECORD
            case .soloAmbient:
                return .SOLO_AMBIENT
            default:
                return .UNKNOWN
            }
        }
        set {
            try? AVFoundation.AVAudioSession.sharedInstance().setCategory(newValue.toAVAudioSessionCategory())
        }
    }

    @Callable
    public func set_category(category: SessionCategory, mode: SessionMode, policy: RouteSharingPolicy, options: Int = 0) -> GodotError {
       do {
           try AVFoundation.AVAudioSession.sharedInstance().setCategory(
               category.toAVAudioSessionCategory(),
               mode: mode.toAVAudioSessionMode(),
               policy: policy.toAVAudioSessionRouteSharingPolicy(),
               options: AVFoundation.AVAudioSession.CategoryOptions(rawValue: UInt(options))
           )
           return .ok
       } catch {
           return .failed
       }
    }
}
#else
@Godot
public class AVAudioSession: RefCounted, @unchecked Sendable {
    @Export var currentCategory: SessionCategory {
        get {
            return .UNKNOWN
        }
        set {
            // ignore
        }
    }

    @Callable
    public func set_category(category: SessionCategory, mode: SessionMode, policy: RouteSharingPolicy, options: Int = 0) -> GodotError {
        return .ok
    }
}
#endif
