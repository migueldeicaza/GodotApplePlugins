//
//  SubscriptionStoreView.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit
import SwiftUI

@Godot
class SubscriptionStoreView: RefCounted, @unchecked Sendable {
    @Export var groupID: String = ""
    @Export var productIDs: PackedStringArray = PackedStringArray()
    
    // Enum for SubscriptionStoreControlStyle
    enum ControlStyle: Int, CaseIterable {
        case AUTOMATIC
        case PICKER
        case BUTTONS
        case COMPACT_PICKER
        case PROMINENT_PICKER
        case PAGED_PICKER
        case PAGED_PROMINENT_PICKER
    }
    
    @Export(.enum) var controlStyle: ControlStyle = .AUTOMATIC

    struct ShowSubscriptionStoreView: View {
        @Environment(\.dismiss) private var dismiss
        var groupID: String
        var productIDs: [String]
    
        var controlStyle: ControlStyle
        var body: some View {
            Group {
                if groupID != "" {
                    StoreKit.SubscriptionStoreView(groupID: groupID)
                } else {
                    StoreKit.SubscriptionStoreView(productIDs: productIDs)
                }
            }
            .toolbar {
                ToolbarItem(placement: .cancellationAction) {
                    Button("Close") {
                        dismiss()
                    }
                }
            }
        }
    }
    
    @Callable
    func present() {
        MainActor.assumeIsolated {
            var ids: [String] = []
            if productIDs.count > 0 {
                for id in productIDs {
                    ids.append(id)
                }
            }

            let wrappedView = NavigationView {
                // Ugly, but not sure what to do other than AnyViewing itall.
                Group {
                    switch controlStyle {
                    case .AUTOMATIC:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .PICKER:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .BUTTONS:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .COMPACT_PICKER:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .PROMINENT_PICKER:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .PAGED_PICKER:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    case .PAGED_PROMINENT_PICKER:
                        if !groupID.isEmpty {
                            StoreKit.SubscriptionStoreView(groupID: groupID)
                                .subscriptionStoreControlStyle(.automatic)
                        } else {
                            StoreKit.SubscriptionStoreView(productIDs: ids)
                                .subscriptionStoreControlStyle(.automatic)
                        }
                    }
                }
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
