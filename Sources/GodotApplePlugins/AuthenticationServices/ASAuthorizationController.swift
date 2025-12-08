//
//  ASAuthorizationController.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/07/25.
//

import Foundation
import AuthenticationServices
import SwiftGodotRuntime
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

@Godot
class ASAuthorizationController: RefCounted, @unchecked Sendable {
    /// Can be either ASAuthorizationAppleIDCredential, ASPasswordCredential or nil for others
    @Signal
    var authorization_completed: SignalWithArguments<RefCounted?>
    
    @Signal
    var authorization_failed: SignalWithArguments<String>

    var controller: AuthenticationServices.ASAuthorizationController?
    var proxy: Proxy?

    // maybe need to make this conform to ASAuthorizationControllerPresentationContextProviding
    // but I should add support for the DisplayServer's UIViewController extraction
    class Proxy: NSObject, ASAuthorizationControllerDelegate {
        weak var base: ASAuthorizationController?
        
        init(_ base: ASAuthorizationController) {
            self.base = base
        }

        @MainActor
        func authorizationController(controller: AuthenticationServices.ASAuthorizationController, didCompleteWithAuthorization authorization: ASAuthorization) {
            guard let base else { return }
            
            if let appleIDCredential = authorization.credential as? AuthenticationServices.ASAuthorizationAppleIDCredential {
                let wrapped = ASAuthorizationAppleIDCredential(credential: appleIDCredential)
                base.authorization_completed.emit(wrapped)
            } else if let passwordCredential = authorization.credential as? AuthenticationServices.ASPasswordCredential {
                let wrapped = ASPasswordCredential(credential: passwordCredential)
                base.authorization_completed.emit(wrapped)
            } else {
                // Unknown credential type, might be the enterprise credential, but I dont think any games need that.
                base.authorization_completed.emit(nil)
            }
        }

        @MainActor
        func authorizationController(controller: AuthenticationServices.ASAuthorizationController, didCompleteWithError error: Error) {
            base?.authorization_failed.emit(error.localizedDescription)
        }
    }

    // The more specific version of it
    @Callable
    func perform_apple_id_request(scopeStrings: VariantArray) {
        var requestedScopes: [ASAuthorization.Scope] = []
        for vscope in scopeStrings {
            guard let scope = String(vscope) else { continue }
            if scope == "email" {
                requestedScopes.append(.email)
            } else if scope == "full_name" {
                requestedScopes.append(.fullName)
            }
        }

        MainActor.assumeIsolated {
            let provider = ASAuthorizationAppleIDProvider()
            let request = provider.createRequest()
            
            request.requestedScopes = requestedScopes
            
            let controller = AuthenticationServices.ASAuthorizationController(authorizationRequests: [request])
            self.controller = controller
            
            let proxy = Proxy(self)
            self.proxy = proxy
            
            controller.delegate = proxy

            // Since most folks would use Godot, we might not need this
            // controller.presentationContextProvider = proxy

            controller.performRequests()
        }
    }

    // Just a general purpose easy-to-use version
    @Callable
    func initiate_signin() {
        MainActor.assumeIsolated {
            let appleIDProvider = ASAuthorizationAppleIDProvider()
            let request = appleIDProvider.createRequest()
            request.requestedScopes = [.fullName, .email]

            let controller = AuthenticationServices.ASAuthorizationController(authorizationRequests: [request])
            self.controller = controller
            
            let proxy = Proxy(self)
            self.proxy = proxy
            
            controller.delegate = proxy

            // Since most folks would use Godot, we might not need this
            //controller.presentationContextProvider = proxy

            controller.performRequests()
        }
    }
}
