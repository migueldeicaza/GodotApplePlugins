import GameKit
//
//  GKSavedGame.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/20/25.
//
@preconcurrency import SwiftGodotRuntime
import SwiftUI

#if canImport(UIKit)
    import UIKit
#else
    import AppKit
#endif

@Godot
class GKSavedGame: GKPlayer, @unchecked Sendable {
    var saved: GameKit.GKSavedGame?

    convenience init(saved: GameKit.GKSavedGame) {
        self.init()
        self.saved = saved
    }

    @Export var name: String {
        saved?.name ?? ""
    }

    @Export var deviceName: String {
        saved?.deviceName ?? ""
    }

    @Export var modificationDate: Double {
        saved?.modificationDate?.timeIntervalSince1970 ?? 0
    }

    @Callable
    func load_data(done: Callable) {
        guard let saved else {
            _ = done.call(
                Variant(PackedByteArray()), Variant(String("GKSavedGame: Instance was not setup")))
            return
        }
        saved.loadData { data, error in
            var ret: Variant
            if let data {
                ret = Variant(data.toPackedByteArray())
            } else {
                ret = Variant(PackedByteArray())
            }
            _ = done.call(ret, GKError.from(error))
        }
    }
}
