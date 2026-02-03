//
//  GKError.swift
//  GodotApplePlugins
//
//

import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
public class GKError: RefCounted, @unchecked Sendable {
    @Export var code: Int = 0
    @Export var message: String = ""
    @Export var domain: String = ""

    public enum Code: Int, CaseIterable {
        // Configuration Errors
        case GAME_UNRECOGNIZED
        case NOT_SUPPORTED
        case APP_UNLISTED

        // Communication Errors
        case UNKNOWN
        case CANCELLED
        case COMMUNICATIONS_FAILURE
        case INVALID_PLAYER
        case INVALID_PARAMETER
        case GAME_SESSION_REQUEST_INVALID
        case API_NOT_AVAILABLE
        case CONNECTION_TIMEOUT
        case API_OBSOLETE

        // Player-Related Errors
        case USER_DENIED
        case INVALID_CREDENTIALS
        case NOT_AUTHENTICATED
        case AUTHENTICATION_IN_PROGRESS
        case PARENTAL_CONTROLS_BLOCKED
        case PLAYER_STATUS_EXCEEDS_MAXIMUM_LENGTH
        case PLAYER_STATUS_INVALID
        case UNDERAGE
        case PLAYER_PHOTO_FAILURE
        case UBIQUITY_CONTAINER_UNAVAILABLE
        case NOT_AUTHORIZED
        case ICLOUD_UNAVAILABLE
        case LOCKDOWN_MODE

        // Friend List Errors
        case FRIEND_LIST_DESCRIPTION_MISSING
        case FRIEND_LIST_RESTRICTED
        case FRIEND_LIST_DENIED
        case FRIEND_REQUEST_NOT_AVAILABLE

        // Matchmaking Errors
        case MATCH_REQUEST_INVALID
        case UNEXPECTED_CONNECTION
        case INVITATIONS_DISABLED
        case MATCH_NOT_CONNECTED
        case RESTRICTED_TO_AUTOMATCH

        // Turn-Based Game Errors
        case TURN_BASED_MATCH_DATA_TOO_LARGE
        case TURN_BASED_TOO_MANY_SESSIONS
        case TURN_BASED_INVALID_PARTICIPANT
        case TURN_BASED_INVALID_TURN
        case TURN_BASED_INVALID_STATE

        // Leaderboard Errors
        case SCORE_NOT_SET

        // Challenges Errors
        case CHALLENGE_INVALID  // Deprecated

        // Enumeration Cases
        case DEBUG_MODE
    }

    convenience init(error: Error) {
        self.init()
        self.message = error.localizedDescription
        self.domain = (error as NSError).domain
        self.code = Self.mapCode(error)
    }

    static func from(_ error: Error?) -> Variant? {
        guard let error else { return nil }
        return Variant(GKError(error: error))
    }

    static func mapCode(_ error: Error) -> Int {
        if let gkError = error as? GameKit.GKError {
            switch gkError.code {
            // Configuration Errors
            case .gameUnrecognized: return Code.GAME_UNRECOGNIZED.rawValue
            case .notSupported: return Code.NOT_SUPPORTED.rawValue
            case .appUnlisted: return Code.APP_UNLISTED.rawValue

            // Communication Errors
            case .unknown: return Code.UNKNOWN.rawValue
            case .cancelled: return Code.CANCELLED.rawValue
            case .communicationsFailure: return Code.COMMUNICATIONS_FAILURE.rawValue
            case .invalidPlayer: return Code.INVALID_PLAYER.rawValue
            case .invalidParameter: return Code.INVALID_PARAMETER.rawValue
            case .gameSessionRequestInvalid: return Code.GAME_SESSION_REQUEST_INVALID.rawValue
            case .apiNotAvailable: return Code.API_NOT_AVAILABLE.rawValue
            case .connectionTimeout: return Code.CONNECTION_TIMEOUT.rawValue
            case .apiObsolete: return Code.API_OBSOLETE.rawValue

            // Player-Related Errors
            case .userDenied: return Code.USER_DENIED.rawValue
            case .invalidCredentials: return Code.INVALID_CREDENTIALS.rawValue
            case .notAuthenticated: return Code.NOT_AUTHENTICATED.rawValue
            case .authenticationInProgress: return Code.AUTHENTICATION_IN_PROGRESS.rawValue
            case .parentalControlsBlocked: return Code.PARENTAL_CONTROLS_BLOCKED.rawValue
            case .playerStatusExceedsMaximumLength:
                return Code.PLAYER_STATUS_EXCEEDS_MAXIMUM_LENGTH.rawValue
            case .playerStatusInvalid: return Code.PLAYER_STATUS_INVALID.rawValue
            case .underage: return Code.UNDERAGE.rawValue
            case .playerPhotoFailure: return Code.PLAYER_PHOTO_FAILURE.rawValue
            case .ubiquityContainerUnavailable: return Code.UBIQUITY_CONTAINER_UNAVAILABLE.rawValue
            case .notAuthorized: return Code.NOT_AUTHORIZED.rawValue
            case .iCloudUnavailable: return Code.ICLOUD_UNAVAILABLE.rawValue
            case .lockdownMode: return Code.LOCKDOWN_MODE.rawValue

            // Friend List Errors
            case .friendListDescriptionMissing: return Code.FRIEND_LIST_DESCRIPTION_MISSING.rawValue
            case .friendListRestricted: return Code.FRIEND_LIST_RESTRICTED.rawValue
            case .friendListDenied: return Code.FRIEND_LIST_DENIED.rawValue
            case .friendRequestNotAvailable: return Code.FRIEND_REQUEST_NOT_AVAILABLE.rawValue

            // Matchmaking Errors
            case .matchRequestInvalid: return Code.MATCH_REQUEST_INVALID.rawValue
            case .unexpectedConnection: return Code.UNEXPECTED_CONNECTION.rawValue
            case .invitationsDisabled: return Code.INVITATIONS_DISABLED.rawValue
            case .matchNotConnected: return Code.MATCH_NOT_CONNECTED.rawValue
            case .restrictedToAutomatch: return Code.RESTRICTED_TO_AUTOMATCH.rawValue

            // Turn-Based Game Errors
            case .turnBasedMatchDataTooLarge: return Code.TURN_BASED_MATCH_DATA_TOO_LARGE.rawValue
            case .turnBasedTooManySessions: return Code.TURN_BASED_TOO_MANY_SESSIONS.rawValue
            case .turnBasedInvalidParticipant: return Code.TURN_BASED_INVALID_PARTICIPANT.rawValue
            case .turnBasedInvalidTurn: return Code.TURN_BASED_INVALID_TURN.rawValue
            case .turnBasedInvalidState: return Code.TURN_BASED_INVALID_STATE.rawValue

            // Leaderboard Errors
            case .scoreNotSet: return Code.SCORE_NOT_SET.rawValue

            // Challenges Errors
            case .challengeInvalid: return Code.CHALLENGE_INVALID.rawValue  // Deprecated

            // Enumeration Cases
            case .debugMode: return Code.DEBUG_MODE.rawValue

            @unknown default: return Code.UNKNOWN.rawValue
            }
        }
        return Code.UNKNOWN.rawValue
    }
}
