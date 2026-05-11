//
//  SwiftUIIntegration.swift
//
//  Created by Miguel de Icaza on 11/14/25.
//
//
//  StoreViewHelpers.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
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
#else
#endif

// Helper to present a SwiftUI view from Godot
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

// Helper to dismiss the top view controller/window
@MainActor
func dismissTopView() {
    #if canImport(UIKit)
    // Find the top view controller and dismiss it
    if let keyWindow = UIApplication.shared.connectedScenes
        .filter({ $0.activationState == .foregroundActive })
        .compactMap({ $0 as? UIWindowScene })
        .first?.windows
        .filter({ $0.isKeyWindow }).first,
       let rootViewController = keyWindow.rootViewController {

        var topController = rootViewController
        while let presentedViewController = topController.presentedViewController {
            topController = presentedViewController
        }
        topController.dismiss(animated: true)
    }
    #else
    // On macOS, we might need to close the window or sheet
    // Implementation depends on how presentOnTop works on macOS.
    // Assuming presentOnTop presents as a sheet or new window.
    // For now, let's try to find the key window and close its sheet if it has one.
    if let window = NSApplication.shared.keyWindow {
        if let sheet = window.attachedSheet {
            window.endSheet(sheet)
        } else {
            // If it was presented as a separate window, we might need to close it.
            // But presentOnTop usually presents as a modal/sheet.
        }
    }
    #endif
}
