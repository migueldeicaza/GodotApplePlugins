//
//  StoreView.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit
import SwiftUI

@Godot
class StoreView: RefCounted, @unchecked Sendable {
    @Export var productIds: PackedStringArray = PackedStringArray()

    
    @Callable
    func present() {
        guard productIds.count > 0 else {
            GD.print("StoreView.present: no product IDs configured")
            return
        }
        
        var ids: [String] = []
        for id in productIds {
            ids.append(id)
        }
        
        Task { @MainActor in
            let view = StoreKit.StoreView(ids: ids) { product in
                Image(systemName: "cart")
            }
            
            let wrappedView = NavigationView {
                view
                    .toolbar {
                        ToolbarItem(placement: .cancellationAction) {
                            Button("Close") {
                                dismissTopView()
                            }
                        }
                    }
            }
            
            presentView(wrappedView)
        }
    }
    
    @Callable
    func dismiss() {
        Task { @MainActor in
            dismissTopView()
        }
    }
}
