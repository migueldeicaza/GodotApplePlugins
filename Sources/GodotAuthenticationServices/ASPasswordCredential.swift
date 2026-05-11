//
//  ASPasswordCredential.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/07/25.
//

import Foundation
import AuthenticationServices
import SwiftGodotRuntime

@Godot
class ASPasswordCredential: RefCounted, @unchecked Sendable {
    var credential: AuthenticationServices.ASPasswordCredential?

    convenience init(credential: AuthenticationServices.ASPasswordCredential) {
        self.init()
        self.credential = credential
    }

    @Export
    var user: String {
        credential?.user ?? ""
    }

    @Export
    var password: String {
        credential?.password ?? ""
    }
}
