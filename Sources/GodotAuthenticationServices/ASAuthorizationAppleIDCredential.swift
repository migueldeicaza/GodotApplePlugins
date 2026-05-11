//
//  ASAuthorizationAppleIDCredential.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/07/25.
//

import Foundation
import AuthenticationServices
import SwiftGodotRuntime

@Godot
class ASAuthorizationAppleIDCredential: RefCounted, @unchecked Sendable {
    var credential: AuthenticationServices.ASAuthorizationAppleIDCredential?

    convenience init(credential: AuthenticationServices.ASAuthorizationAppleIDCredential) {
        self.init()
        self.credential = credential
    }

    @Export
    var user: String {
        credential?.user ?? ""
    }

    @Export
    var state: String {
        credential?.state ?? ""
    }

    @Export
    var email: String {
        credential?.email ?? ""
    }

    @Export
    var fullName: VariantDictionary {
        credential?.fullName?.toGodotDictionary() ?? VariantDictionary()
    }

    @Export
    var identityToken: PackedByteArray {
        guard let data = credential?.identityToken else { return PackedByteArray() }
        let res = PackedByteArray(Array(data))
        return res
    }

    @Export
    var authorizationCode: PackedByteArray {
        guard let data = credential?.authorizationCode else { return PackedByteArray() }
        let res = PackedByteArray()
        for byte in data { res.append(value: Int64(byte)) }
        return res
    }

    enum UserDetectionStatus: Int, CaseIterable {
        case UNSUPPORTED = 0
        case UNKNOWN = 1
        case LIKELY_REAL = 2

        static func from(_ status: ASUserDetectionStatus) -> UserDetectionStatus {
            switch status {
            case .unsupported: return .UNSUPPORTED
            case .unknown: return .UNKNOWN
            case .likelyReal: return .LIKELY_REAL
            @unknown default: return .UNKNOWN
            }
        }
    }

    enum UserAgeRange: Int, CaseIterable {
        case NOT_KNOWN = 0
        case CHILD = 1
        case NOT_CHILD = 2

        static func from(_ range: ASUserAgeRange) -> UserAgeRange {
            switch range {
            case .unknown: return .NOT_KNOWN
            case .child: return .CHILD
            case .notChild: return .NOT_CHILD
            @unknown default: return .NOT_KNOWN
            }
        }
    }

    @Export
    var realUserStatus: UserDetectionStatus {
        UserDetectionStatus.from(credential?.realUserStatus ?? .unsupported)
    }

    @Export
    var userAgeRange: UserAgeRange {
        UserAgeRange.from(credential?.userAgeRange ?? .unknown)
    }

    @Export
    var authorizedScopes: VariantArray {
        let array = VariantArray()
        guard let credential else { return array }
        
        for scope in credential.authorizedScopes {
            if scope == .email {
                array.append(Variant("email"))
            } else if scope == .fullName {
                if let name = credential.fullName {
                    array.append(Variant(name.toGodotDictionary()))
                }
            }
        }
        return array
    }
}

extension PersonNameComponents {
    func toGodotDictionary() -> VariantDictionary {
        let dict = VariantDictionary()
        if let namePrefix { dict["name_prefix"] = Variant(namePrefix) }
        if let givenName { dict["given_name"] = Variant(givenName) }
        if let middleName { dict["middle_name"] = Variant(middleName) }
        if let familyName { dict["family_name"] = Variant(familyName) }
        if let nameSuffix { dict["name_suffix"] = Variant(nameSuffix) }
        if let nickname { dict["nickname"] = Variant(nickname) }
        return dict
    }
}
