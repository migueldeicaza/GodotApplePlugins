//
//  StoreKitManager.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

import StoreKit
@preconcurrency import SwiftGodotRuntime

@Godot
public class StoreKitManager: RefCounted, @unchecked Sendable {
    // [StoreProduct], StoreKitStatus
    @Signal("products", "status") var products_request_completed:
        SignalWithArguments<TypedArray<StoreProduct?>, Int>
    // StoreTransaction, StoreKitStatus, error message
    @Signal("transaction", "status", "message") var purchase_completed:
        SignalWithArguments<StoreTransaction?, Int, String>
    // StoreTransaction
    @Signal("transaction") var transaction_updated: SignalWithArguments<StoreTransaction?>
    // StoreProduct
    @Signal("product") var purchase_intent: SignalWithArguments<StoreProduct?>

    // StoreKitStatus, error_message (empty on success)
    @Signal("status", "message") var restore_completed: SignalWithArguments<Int, String>

    // StoreKitStatus, error_message (empty on success)
    @Signal("status", "message") var refresh_completed: SignalWithArguments<Int, String>

    // This is only raised for verified results
    @Signal("status") var subscription_update: SignalWithArguments<StoreSubscriptionInfoStatus?>

    public enum StoreKitStatus: Int, CaseIterable {
        case OK
        /// Invalid product, the StoreProduct does not contains a valid product
        case INVALID_PRODUCT
        /// The operation was canceled
        case CANCELLED

        case UNVERIFIED_TRANSACTION

        case USER_CANCELLED

        case PURCHASE_PENDING

        case UNKNOWN_STATUS
    }
    private var updatesTask: Task<Void, Never>?
    private var subscriptionTask: Task<Void, Never>?
    private var intentsTask: Task<Void, Never>?

    required init(_ context: InitContext) {
        super.init(context)
        GD.print("Remember that you have now to connect your signals and call 'start'")
    }

    deinit {
        updatesTask?.cancel()
        intentsTask?.cancel()
        subscriptionTask?.cancel()
    }

    var started = false

    func start() {
        if started { return }
        startTransactionListener()
        startPurchaseIntentListener()
        started = true
    }

    func stop() {
        guard started else { return }
        updatesTask?.cancel()
        intentsTask?.cancel()
        subscriptionTask?.cancel()
        subscriptionTask = nil
        updatesTask = nil
        intentsTask = nil
        started = false
    }

    private func startTransactionListener() {
        updatesTask = Task {
            for await verificationResult in Transaction.updates {
                handleTransaction(verificationResult)
            }
        }
        subscriptionTask = Task {
            for await status in Product.SubscriptionInfo.Status.updates {
                guard case .verified(_) = status.transaction,
                    case .verified(_) = status.renewalInfo
                else {
                    // TODO: should raise an event here, just like the updateTask does
                    GD.print("Unverified transaction")
                    continue
                }
                Task { @MainActor in
                    Task { @MainActor in
                        self.subscription_update.emit(StoreSubscriptionInfoStatus(status))
                    }
                }
            }
        }
    }

    private func startPurchaseIntentListener() {
        if #available(iOS 17.4, macOS 14.4, *) {
            intentsTask = Task {
                for await intent in PurchaseIntent.intents {
                    let storeProduct = StoreProduct(intent.product)
                    await MainActor.run {
                        GD.print("Posting purchase_intent")

                        _ = self.purchase_intent.emit(storeProduct)
                    }
                }
            }
        }
    }

    private func handleTransaction(_ verificationResult: VerificationResult<Transaction>) {
        switch verificationResult {
        case .verified(let transaction):
            let storeTransaction = StoreTransaction(transaction)
            // Always finish the transaction if it's verified and we've received it
            // In a real app, we might want to wait until the user has unlocked content,
            // but for this binding, we'll emit the signal and finish it.
            // The user can check the transaction state.
            Task {
                await transaction.finish()
            }

            // Emit signal on main thread
            Task { @MainActor in
                GD.print("Posting transaction_updated")
                self.transaction_updated.emit(storeTransaction)
            }
        case .unverified(_, _):
            // TODO: would be nice to raise this one
            GD.print("Transaction: got an unverified one")
            break
        }
    }

    @Callable
    func request_products(productIds: PackedStringArray) {
        var ids: [String] = []
        ids.reserveCapacity(productIds.count)
        for id in productIds {
            ids.append(id)
        }
        Task { @MainActor in
            do {
                let products = try await Product.products(for: ids)
                let variantArray = TypedArray<StoreProduct?>()
                for product in products {
                    variantArray.append(StoreProduct(product))
                }
                _ = self.products_request_completed.emit(variantArray, StoreKitStatus.OK.rawValue)
            } catch {
                _ = self.products_request_completed.emit(
                    TypedArray<StoreProduct?>(), StoreKitStatus.CANCELLED.rawValue)
            }
        }
    }

    @Callable
    func purchase(product: StoreProduct) {
        purchase_with_options(product: product, options: [])
    }

    @Callable
    func purchase_with_options(
        product: StoreProduct, options: TypedArray<StoreProductPurchaseOption?>
    ) {
        guard let skProduct = product.product else {
            self.purchase_completed.emit(
                nil, StoreKitStatus.INVALID_PRODUCT.rawValue, "Invalid Product")
            return
        }

        var optionSet = Set<Product.PurchaseOption>()
        for option in options {
            guard let option, let llOption = option.purchaseOption else { continue }
            optionSet.insert(llOption)
        }
        Task {
            do {
                let result = try await skProduct.purchase(options: optionSet)

                switch result {
                case .success(let verification):
                    switch verification {
                    case .verified(let transaction):
                        let storeTransaction = StoreTransaction(transaction)
                        await transaction.finish()
                        await MainActor.run {
                            _ = self.purchase_completed.emit(
                                storeTransaction, StoreKitStatus.OK.rawValue, "")
                        }
                    case .unverified(_, let error):
                        await MainActor.run {
                            _ = self.purchase_completed.emit(
                                nil, StoreKitStatus.UNVERIFIED_TRANSACTION.rawValue,
                                "Unverified transaction: \(error.localizedDescription)")
                        }
                    }
                case .userCancelled:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(
                            nil, StoreKitStatus.USER_CANCELLED.rawValue, "User cancelled")
                    }
                case .pending:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(
                            nil, StoreKitStatus.PURCHASE_PENDING.rawValue, "Purchase pending")
                    }
                @unknown default:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(
                            nil, StoreKitStatus.UNKNOWN_STATUS.rawValue, "Unknown purchase result")
                    }
                }
            } catch {
                await MainActor.run {
                    _ = self.purchase_completed.emit(
                        nil, StoreKitStatus.CANCELLED.rawValue, error.localizedDescription)
                }
            }
        }
    }

    @Callable
    func restore_purchases() {
        Task {
            do {
                try await AppStore.sync()
                await MainActor.run {
                    _ = self.restore_completed.emit(StoreKitStatus.OK.rawValue, "")
                }
            } catch {
                await MainActor.run {
                    _ = self.restore_completed.emit(
                        StoreKitStatus.CANCELLED.rawValue, error.localizedDescription)
                }
            }
        }
    }

    @Callable
    func refresh_purchased_products() {
        Task {
            for await verificationResult in Transaction.currentEntitlements {
                await MainActor.run {
                    handleTransaction(verificationResult)
                }
            }
            await MainActor.run {
                _ = self.refresh_completed.emit(StoreKitStatus.OK.rawValue, "")
            }
        }
    }
}
