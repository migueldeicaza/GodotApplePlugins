//
//  SwiftUIIntegration.swift
//
//  Created by Miguel de Icaza on 11/14/25.
//

@preconcurrency import SwiftGodotRuntime
import SwiftUI
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

#if canImport(UIKit)
@MainActor
func presentSwiftUIOverlayFromTopMost<V: View>(_ view: V) {
    guard let presenter = topMostViewController() else {
        return
    }

    let hosting = UIHostingController(rootView: view)
    hosting.modalPresentationStyle = .formSheet
    hosting.view.backgroundColor = .clear

    presenter.present(hosting, animated: true)
}
#endif

@MainActor
func presentView<V: View>(_ view: V) {
    #if canImport(UIKit)
    let controller = UIHostingController(rootView: view)
    presentOnTop(controller)
    #else
    let controller = NSHostingController(rootView: view)
    presentOnTop(controller)
    #endif
}

@MainActor
func dismissTopView() {
    #if canImport(UIKit)
    if let keyWindow = UIApplication.shared.connectedScenes
        .filter({ $0.activationState == .foregroundActive })
        .compactMap({ $0 as? UIWindowScene })
        .first?.windows
        .first(where: { $0.isKeyWindow }),
       let rootViewController = keyWindow.rootViewController {
        var topController = rootViewController
        while let presentedViewController = topController.presentedViewController {
            topController = presentedViewController
        }
        topController.dismiss(animated: true)
    }
    #else
    if let window = NSApplication.shared.keyWindow, let sheet = window.attachedSheet {
        window.endSheet(sheet)
    }
    #endif
}
