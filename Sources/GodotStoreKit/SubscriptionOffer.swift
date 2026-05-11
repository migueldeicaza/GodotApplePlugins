//
//  SubscriptionOffer.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/14/25.
//


@preconcurrency import SwiftGodotRuntime
import StoreKit
import SwiftUI

class StoreProductSubscriptionOffer: RefCounted, @unchecked Sendable {
    var offer: Product.SubscriptionOffer?
    convenience init(offer:  Product.SubscriptionOffer) {
        self.init()
        self.offer = offer
    }

    public enum OfferType: Int, CaseIterable {
        case INTRODUCTORY
        case PROMOTIONAL
        case WIN_BACK
        case UNKNOWN
    }

    @Export var offerId: String { offer?.id ?? ""}
    @Export var type: OfferType {
        guard let t = offer?.type else {
            return .UNKNOWN
        }
        if t == .introductory { return .INTRODUCTORY }
        if t == .promotional { return .PROMOTIONAL }
        if #available(macOS 15.0, iOS 18.0, *) {
            if t == .winBack { return .WIN_BACK }
        }
        return .UNKNOWN
    }

    @Export var typeLocalized: String {
        return offer?.type.localizedDescription ?? ""
    }

    @Export var displayPrice: String {
        offer?.displayPrice ?? "0"
    }

    @Export var priceDecimal: String {
        offer?.price.description ?? "0"
    }

    @Export var paymentMode: StoreProductPaymentMode? {
        guard let offer else { return nil }
        return StoreProductPaymentMode(paymentMode: offer.paymentMode)
    }

    @Export var period: StoreProductSubscriptionPeriod? {
        guard let offer else { return nil }
        return StoreProductSubscriptionPeriod(offer.period)
    }
}

@Godot
class StoreProductPaymentMode: RefCounted, @unchecked Sendable {
    var paymentMode: Product.SubscriptionOffer.PaymentMode?
    convenience init(paymentMode: Product.SubscriptionOffer.PaymentMode) {
        self.init()
        self.paymentMode = paymentMode
    }

    @Callable static func get_free_trial() -> StoreProductPaymentMode {
        return StoreProductPaymentMode(paymentMode: .freeTrial)
    }
    @Callable static func get_pay_as_you_go() -> StoreProductPaymentMode {
        return StoreProductPaymentMode(paymentMode: .payAsYouGo)
    }
    @Callable static func pay_up_front() -> StoreProductPaymentMode {
        return StoreProductPaymentMode(paymentMode: .payUpFront)
    }

    @Export var localized_description: String { paymentMode?.localizedDescription ?? "" }
}
