import GameKit
@preconcurrency import SwiftGodotRuntime

@Godot
class GKNotificationBanner: RefCounted, @unchecked Sendable {
    @Callable
    static func show(title: String, message: String) {
        #if os(iOS)
        GameKit.GKNotificationBanner.show(withTitle: title, message: message, completionHandler: nil)
        #endif
    }

    @Callable
    static func show_with_duration(title: String, message: String, duration: Double) {
        #if os(iOS)
        GameKit.GKNotificationBanner.show(
            withTitle: title,
            message: message,
            duration: duration,
            completionHandler: nil
        )
        #endif
    }
}
