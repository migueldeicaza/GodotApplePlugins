//
//  StoreKitManager.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit

@Godot
public class StoreKitManager: RefCounted, @unchecked Sendable {
    // [StoreProduct], StoreKitStatus
    @Signal("products", "status") var products_request_completed: SignalWithArguments<TypedArray<StoreProduct?>, Int>
    // StoreTransaction, StoreKitStatus, error message
    @Signal("transaction", "status", "message") var purchase_completed: SignalWithArguments<StoreTransaction?, Int, String>
    // StoreTransaction
    @Signal("transaction") var transaction_updated: SignalWithArguments<StoreTransaction?>
    // StoreProduct
    @Signal("product") var purchase_intent: SignalWithArguments<StoreProduct?>

    // StoreKitStatus, error_message (empty on success)
    @Signal("status", "message") var restore_completed: SignalWithArguments<Int, String>

    public enum StoreKitStatus: Int, CaseIterable {
        case ok
        /// Invalid product, the StoreProduct does not contains a valid product
        case invalidProduct
        /// The oepration was canceled
        case cancelled

        case unverifiedTransaction

        case userCancelled

        case purchasePending

        case unknownStatus
    }
    private var updatesTask: Task<Void, Never>?
    private var intentsTask: Task<Void, Never>?

    required init(_ context: InitContext) {
        super.init(context)
        GD.print("Remember that you have now to connect your signals and call 'start'")
    }
    
    deinit {
        updatesTask?.cancel()
        intentsTask?.cancel()
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
        Task {
            do {
                var ids: [String] = []
                for id in productIds {
                    ids.append(id)
                }
                let products = try await Product.products(for: ids)
                let storeProducts = products.map { StoreProduct($0) }
                let variantArray = TypedArray<StoreProduct?>()
                for sp in storeProducts {
                    variantArray.append(sp)
                }
                
                await MainActor.run {
                    _ = self.products_request_completed.emit(variantArray, StoreKitStatus.ok.rawValue)
                }
            } catch {
                await MainActor.run {
                    _ = self.products_request_completed.emit(TypedArray<StoreProduct?>(), StoreKitStatus.cancelled.rawValue)
                }
            }
        }
    }

    @Callable
    func purchase(product: StoreProduct) {
        purchase_with_options(product: product, options: [])
    }

    @Callable
    func purchase_with_options(product: StoreProduct, options: TypedArray<StoreProductPurchaseOption?>) {
        guard let skProduct = product.product else {
            self.purchase_completed.emit(nil, StoreKitStatus.invalidProduct.rawValue, "Invalid Product")
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
                            _ = self.purchase_completed.emit(storeTransaction, StoreKitStatus.ok.rawValue, "")
                        }
                    case .unverified(_, let error):
                        await MainActor.run {
                            _ = self.purchase_completed.emit(nil, StoreKitStatus.unverifiedTransaction.rawValue, "Unverified transaction: \(error.localizedDescription)")
                        }
                    }
                case .userCancelled:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(nil, StoreKitStatus.userCancelled.rawValue, "User cancelled")
                    }
                case .pending:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(nil, StoreKitStatus.purchasePending.rawValue, "Purchase pending")
                    }
                @unknown default:
                    await MainActor.run {
                        _ = self.purchase_completed.emit(nil, StoreKitStatus.unknownStatus.rawValue, "Unknown purchase result")
                    }
                }
            } catch {
                await MainActor.run {
                    _ = self.purchase_completed.emit(nil, StoreKitStatus.cancelled.rawValue, error.localizedDescription)
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
                    _ = self.restore_completed.emit(StoreKitStatus.ok.rawValue, "")
                }
            } catch {
                await MainActor.run {
                    _ = self.restore_completed.emit(StoreKitStatus.cancelled.rawValue, error.localizedDescription)
                }
            }
        }
    }
}
