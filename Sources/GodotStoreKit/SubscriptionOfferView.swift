//
//  SubscriptionOfferView.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit
import SwiftUI

@Godot
class SubscriptionOfferView: RefCounted, @unchecked Sendable {
    @Export var title: String = "Redeeming Offer..."
    @Signal var success: SimpleSignal
    @Signal("message") var error: SignalWithArguments<String>

    // TODO: should this instead raise signals instead of the callback here?
    @Callable
    func present(callback: Callable) {
        Task { @MainActor in
            // To present offer code redemption, we usually use .offerCodeRedemption(isPresented: ...) on a view.
            // Since we are presenting a new view, we can create a dummy view that immediately presents the offer code sheet.
            // Or better, use the `SKPaymentQueue.default().presentCodeRedemptionSheet()` which is the older API but still works and is simpler for "presenting" something.
            // But for StoreKit 2, we should use the SwiftUI modifier.
            
            let view = OfferCodeWrapperView(title: title, offer: self)
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

struct OfferCodeWrapperView: View {
    @State private var isPresented = true
    let title: String
    let offer: SubscriptionOfferView

    var body: some View {
        if #available(iOS 15.0, macOS 15.0, *) {
            Text(title)
                .onAppear {
                    isPresented = true
                }
                .offerCodeRedemption(isPresented: $isPresented) { result in
                    switch result {
                    case .success:
                        _ = offer.success.emit()
                        dismissTopView()
                    case .failure(let error):
                        _ = offer.error.emit("Offer code redemption failed: \(error.localizedDescription)")
                        dismissTopView()
                    }
                }
        } else {
            Text("Offer redemption is not available on this OS version")
        }
    }
}
