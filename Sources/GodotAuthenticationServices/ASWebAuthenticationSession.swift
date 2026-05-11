//
//  ASWebAuthenticationSession.swift
//  GodotApplePlugins
//
//  Created by Dragos Daian on 01/14/26.
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
class ASWebAuthenticationSession: RefCounted, @unchecked Sendable {
    /// Emitted when the auth flow completes successfully.
    /// The callback URL contains the authorization code / token, depending on your provider.
    @Signal("callback_url") var completed: SignalWithArguments<String>

    /// Emitted when the auth flow fails.
    @Signal("message") var failed: SignalWithArguments<String>

    /// Emitted when the user cancels the auth flow (e.g. closes the sheet).
    @Signal var canceled: SimpleSignal

    private var session: AuthenticationServices.ASWebAuthenticationSession?
    private var proxy: Proxy?

    private final class Proxy: NSObject, ASWebAuthenticationPresentationContextProviding {
        nonisolated override init() {
            super.init()
        }
#if canImport(UIKit)
        func presentationAnchor(for session: AuthenticationServices.ASWebAuthenticationSession) -> ASPresentationAnchor {
            // Godot apps should have a key window. If not, return a new window (may not present correctly).
            return UIApplication.shared.keyWindow ?? UIWindow(frame: .zero)
        }
#else
        private var fallbackWindow: NSWindow?

        func presentationAnchor(for session: AuthenticationServices.ASWebAuthenticationSession) -> ASPresentationAnchor {
            if let window = NSApplication.shared.keyWindow ?? NSApplication.shared.mainWindow {
                return window
            }
            // Fallback for unusual embedding cases.
            if fallbackWindow == nil {
                fallbackWindow = NSWindow(contentRect: NSRect(x: 0, y: 0, width: 1, height: 1),
                                          styleMask: [.titled],
                                          backing: .buffered,
                                          defer: true)
            }
            return fallbackWindow!
        }
#endif
    }

    /// Starts an OAuth-style web authentication flow.
    ///
    /// - Parameters:
    ///   - auth_url: The URL to open (provider auth endpoint).
    ///   - callback_scheme: Your app’s callback URL scheme (e.g. "mygame") or empty string to not restrict.
    ///   - prefers_ephemeral: If true, uses an ephemeral browser session (no shared cookies).
    ///
    /// Returns true if the session started.
    @Callable
    func start(auth_url: String, callback_scheme: String, prefers_ephemeral: Bool = false) -> Bool {
        guard let url = URL(string: auth_url) else {
            failed.emit("Invalid auth_url")
            return false
        }

        let scheme: String? = callback_scheme.isEmpty ? nil : callback_scheme

        // Cancel any existing session to avoid overlapping flows.
        session?.cancel()
        session = nil
        proxy = nil

        let proxy = Proxy()
        self.proxy = proxy

        let session = AuthenticationServices.ASWebAuthenticationSession(url: url, callbackURLScheme: scheme) { [weak self] callbackURL, error in
            guard let self else { return }

            DispatchQueue.main.async { [weak self] in
                guard let self else { return }
                defer {
                    self.session = nil
                    self.proxy = nil
                }

                if let error {
                    if let asError = error as? ASWebAuthenticationSessionError,
                       asError.code == .canceledLogin {
                        self.canceled.emit()
                        return
                    }
                    self.failed.emit(error.localizedDescription)
                    return
                }

                guard let callbackURL else {
                    self.failed.emit("Missing callback URL")
                    return
                }

                self.completed.emit(callbackURL.absoluteString)
            }
        }

        session.presentationContextProvider = proxy

        if #available(iOS 13.0, macOS 10.15, *) {
            session.prefersEphemeralWebBrowserSession = prefers_ephemeral
        }

        self.session = session

        // Must be started on the main thread (presentation).
        if Thread.isMainThread {
            return session.start()
        }

        var started = false
        DispatchQueue.main.sync {
            started = session.start()
        }
        return started
    }

    /// Cancels a running session.
    @Callable
    func cancel() {
        session?.cancel()
        session = nil
        proxy = nil
    }
}
