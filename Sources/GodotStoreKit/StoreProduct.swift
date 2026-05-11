//
//  StoreProduct.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit

@Godot
class StoreProduct: RefCounted, @unchecked Sendable {
    var product: Product?

    convenience init(_ product: Product) {
        self.init()
        self.product = product
    }
    
    @Export var productId: String { product?.id ?? "" }
    @Export var displayName: String { product?.displayName ?? "" }
    @Export var descriptionValue: String { product?.description ?? "" }
    @Export var price: Double { 
        guard let product else { return 0.0 }
        return Double(truncating: product.price as NSNumber)
    }
    @Export var displayPrice: String { product?.displayPrice ?? "" }
    @Export var isFamilyShareable: Bool { product?.isFamilyShareable ?? false }
    
    // Helper to get the JSON representation if needed for more details
    @Export var jsonRepresentation: String {
        guard let product else { return "" }
        return "\(product)"
    }
}

@Godot
class StoreProductPurchaseOption: RefCounted, @unchecked Sendable {
    var purchaseOption: Product.PurchaseOption?

    convenience init(_ purchaseOption: Product.PurchaseOption) {
        self.init()
        self.purchaseOption = purchaseOption
    }

    @Callable
    static func app_account_token(stringUuidToken: String) -> StoreProductPurchaseOption? {
        guard let token = UUID(uuidString: stringUuidToken) else { return nil }
        let purchaseOption = Product.PurchaseOption.appAccountToken(token)
        return StoreProductPurchaseOption(purchaseOption)
    }

    @Callable
    static func win_back_offer(offer: StoreProductSubscriptionOffer?) -> StoreProductPurchaseOption? {
        guard let skoffer = offer?.offer else {
            return nil
        }
        if #available(iOS 18.0, macOS 15.0, *) {
            let purchaseOption = Product.PurchaseOption.winBackOffer(skoffer)
            return StoreProductPurchaseOption(purchaseOption)
        } else {
            return nil
        }
    }

    @Callable
    static func quantity(value: Int) -> StoreProductPurchaseOption? {
        return StoreProductPurchaseOption(Product.PurchaseOption.quantity(value))
    }

    @Callable
    static func introductory_offer_elligibility(jws: String) -> StoreProductPurchaseOption? {
        return StoreProductPurchaseOption(Product.PurchaseOption.introductoryOfferEligibility(compactJWS: jws))
    }

    @Callable
    static func simulate_ask_to_buy_in_sandbox(enabled: Bool) -> StoreProductPurchaseOption? {
        return StoreProductPurchaseOption(Product.PurchaseOption.simulatesAskToBuyInSandbox(enabled))
    }
}
