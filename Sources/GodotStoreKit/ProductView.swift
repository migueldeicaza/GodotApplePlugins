//
//  ProductView.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit
import SwiftUI

@Godot
class ProductView: RefCounted, @unchecked Sendable {
    @Export var productId: String = ""
    @Export var prefersPromotionalIcon: Bool = false
    @Export var systemIconName: String = "cart"

    // Enum for ProductViewStyle
    enum ViewStyle: Int, CaseIterable {
        case AUTOMATIC = 0
        case COMPACT = 1
        case LARGE = 2
        case REGULAR = 3
    }
    
    @Export(.enum) var style: ViewStyle = .AUTOMATIC

    @Callable
    func present() {
        guard !productId.isEmpty else { return }
        
        Task { @MainActor in
            let view = StoreKit.ProductView(id: productId) {
                Image(systemName: systemIconName)
            }
            
            switch style {
            case .AUTOMATIC:
                self.presentWrapped(view.productViewStyle(.automatic))
            case .COMPACT:
                self.presentWrapped(view.productViewStyle(.compact))
            case .LARGE:
                self.presentWrapped(view.productViewStyle(.large))
            case .REGULAR:
                self.presentWrapped(view.productViewStyle(.regular))
            }
        }
    }
    
    @MainActor
    private func presentWrapped<V: View>(_ view: V) {
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
    
    @Callable
    func dismiss() {
        Task { @MainActor in
            dismissTopView()
        }
    }
}
