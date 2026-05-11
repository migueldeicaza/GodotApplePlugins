//
//  AppKitIntegration.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/15/25.
//

#if canImport(AppKit)
import AppKit
import SwiftGodotRuntime

@MainActor
func presentOnTop(_ vc: NSViewController) {
    guard let window = NSApp.keyWindow ?? NSApp.mainWindow else {
        NSApp.activate(ignoringOtherApps: true)
        return
    }
    if let cv = window.contentViewController {
        cv.presentAsSheet(vc)
    } else {
        let panel = NSWindow(contentViewController: vc)
        panel.styleMask = [.titled, .resizable, .closable]
        panel.level = .floating
        panel.isReleasedWhenClosed = false
        panel.isReleasedWhenClosed = true
        panel.makeKeyAndOrderFront(nil)
    }
}
#endif
