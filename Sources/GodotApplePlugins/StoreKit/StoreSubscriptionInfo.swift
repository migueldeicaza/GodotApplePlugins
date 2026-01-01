//
//  StoreSubscriptionInfo.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 1/1/26.
//


@preconcurrency import SwiftGodotRuntime
import StoreKit

@Godot
class StoreSubscriptionInfo: RefCounted, @unchecked Sendable {
    var product: Product.SubscriptionInfo?

    convenience init(_ product: Product.SubscriptionInfo) {
        self.init()
        self.product = product
    }

    @Export var subscriptionGroupID: String {
        product?.subscriptionGroupID ?? ""
    }

    @Export var groupDisplayName: String {
        product?.groupDisplayName ?? ""
    }

    @Export var groupLevel: Int {
        product?.groupLevel ?? -1
    }

    @MainActor
    func processStatus(single: Bool = false, callback: Callable, method: @escaping () async throws -> [Product.SubscriptionInfo.Status]) {
        Task {
            do {
                let value = try await method()
                if single, let v = value.first {
                    let result = StoreSubscriptionInfoStatus(v)
                    Task { @MainActor in
                        _ = callback.call(Variant(result))
                    }

                } else {
                    let result = TypedArray<StoreSubscriptionInfoStatus?>()
                    for status in value {
                        result.append(StoreSubscriptionInfoStatus(status))
                    }
                    Task { @MainActor in
                        _ = callback.call(Variant(result))
                    }
                }
            } catch {
                let s = String(describing: error)
                Task { @MainActor in
                    _ = callback.call(Variant(s))
                }
            }
        }
    }

    @Callable
    func getStatus(callback: Callable) {
        guard let product else {
            return
        }
        MainActor.assumeIsolated {
            processStatus(callback: callback) {
                try await product.status
            }
        }
    }

    @Callable
    func status_for_group_id(_ group_id: String, status: Callable) {
        MainActor.assumeIsolated {
            processStatus(callback: status) {
                try await Product.SubscriptionInfo.status(for: group_id)
            }
        }
    }

    @Callable
    func status_for_transaction(_ transaction_id: Int, status: Callable) {
        guard #available(macOS 15.4, *) else {
            return
        }
        MainActor.assumeIsolated {
            processStatus(single: true, callback: status) {
                if let v = try await Product.SubscriptionInfo.status(transactionID: UInt64(bitPattern: Int64(transaction_id))) {
                    return [v]
                } else {
                    return []
                }
            }
        }
    }
}

@Godot
class StoreSubscriptionInfoStatus: RefCounted, @unchecked Sendable {
    var status: Product.SubscriptionInfo.Status?

    convenience init(_ status: Product.SubscriptionInfo.Status) {
        self.init()
        self.status = status
    }

    enum RenewalState: Int, CaseIterable {
        case UNKNOWN
        case EXPIRED
        case SUBSCRIBED
        case IN_BILLING_RETRY_PERIOD
        case IN_GRACE_PERIOD
        case REVOKED

        static func fromRenewalState(_ state: Product.SubscriptionInfo.RenewalState) -> RenewalState {
            if state == Product.SubscriptionInfo.RenewalState.expired {
                return .EXPIRED
            }
            if state == Product.SubscriptionInfo.RenewalState.subscribed {
                return .SUBSCRIBED
            }
            if state == Product.SubscriptionInfo.RenewalState.inBillingRetryPeriod {
                return .IN_BILLING_RETRY_PERIOD
            }
            if state == Product.SubscriptionInfo.RenewalState.inGracePeriod {
                return .IN_GRACE_PERIOD
            }
            if state == Product.SubscriptionInfo.RenewalState.revoked {
                return .REVOKED
            }
            return .UNKNOWN
        }
    }

    @Export var state: RenewalState {
        guard let status else { return .UNKNOWN }
        return RenewalState.fromRenewalState(status.state)
    }

    @Export var renewal_info: StoreSubscriptionInfoRenewalInfo? {
        switch status?.renewalInfo {
        case .verified(let info):
            return StoreSubscriptionInfoRenewalInfo(info)
        default:
            return nil
        }
    }

    @Export var transaction: StoreTransaction? {
        switch status?.transaction {
        case .verified(let txn):
            return StoreTransaction(txn)
        default:
            return nil
        }
    }
}

@Godot
class StoreSubscriptionInfoRenewalInfo: RefCounted, @unchecked Sendable {
    var renewalInfo: Product.SubscriptionInfo.RenewalInfo?

    convenience init(_ renewalInfo: Product.SubscriptionInfo.RenewalInfo) {
        self.init()
        self.renewalInfo = renewalInfo
    }

    @Export var original_transaction_id: Int {
        guard let renewalInfo else { return 0 }

        return Int(bitPattern: UInt(renewalInfo.originalTransactionID))
    }

    @Export var app_account_token: String {
        guard let renewalInfo else { return "" }

        return renewalInfo.appAccountToken?.description ?? ""
    }

    @Export var app_transaction_id: String {
        guard let renewalInfo else { return "" }

        return renewalInfo.appTransactionID
    }

    @Export var current_product_id: String {
        guard let renewalInfo else { return "" }

        return renewalInfo.currentProductID
    }
}
