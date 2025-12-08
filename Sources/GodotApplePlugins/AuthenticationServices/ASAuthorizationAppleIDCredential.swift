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
        case unsupported = 0
        case unknown = 1
        case likelyReal = 2
        
        static func from(_ status: ASUserDetectionStatus) -> UserDetectionStatus {
            switch status {
            case .unsupported: return .unsupported
            case .unknown: return .unknown
            case .likelyReal: return .likelyReal
            @unknown default: return .unknown
            }
        }
    }

    enum UserAgeRange: Int, CaseIterable {
        case unknown = 0
        case child = 1
        case notChild = 2
        
        static func from(_ range: ASUserAgeRange) -> UserAgeRange {
            switch range {
            case .unknown: return .unknown
            case .child: return .child
            case .notChild: return .notChild
            @unknown default: return .unknown
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
