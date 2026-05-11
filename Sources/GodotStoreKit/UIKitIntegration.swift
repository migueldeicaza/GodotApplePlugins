//
//  UIKitIntegration.swift
//  SwiftGodotAppleTemplate
//
//  Created by Miguel de Icaza on 11/14/25.
//
#if canImport(UIKit)
import UIKit
import SwiftGodotRuntime

extension UIApplication {
    var activeWindowScene: UIWindowScene? {
        let scenes = connectedScenes
            .compactMap { $0 as? UIWindowScene }
        return scenes.first { $0.activationState == .foregroundActive }
            ?? scenes.first { $0.activationState == .foregroundInactive }
    }

    var keyWindow: UIWindow? {
        if let scene = activeWindowScene {
            return scene.windows.first { $0.isKeyWindow } ?? scene.windows.first
        }
        return windows.first { $0.isKeyWindow } ?? windows.first
    }

    var topMostViewController: UIViewController? {
        guard let root = keyWindow?.rootViewController else { return nil }
        return root.mostVisibleViewController
    }
}

extension UIViewController {
    var mostVisibleViewController: UIViewController {
        if let nav = self as? UINavigationController {
            return nav.visibleViewController?.mostVisibleViewController ?? nav
        }
        if let tab = self as? UITabBarController {
            return tab.selectedViewController?.mostVisibleViewController ?? tab
        }
        if let presented = presentedViewController {
            return presented.mostVisibleViewController
        }
        return self
    }
}

@MainActor
func topMostViewController() -> UIViewController? {
    UIApplication.shared.topMostViewController
}

@MainActor
func presentOnTop(_ vc: UIViewController) {
    guard let top = topMostViewController() else {
        print("Could not find the top view controller")
        return
    }
    guard !top.isBeingDismissed else {
        DispatchQueue.main.async {
            presentOnTop(vc)
        }
        return
    }
    top.present(vc, animated: true)
}
#endif
