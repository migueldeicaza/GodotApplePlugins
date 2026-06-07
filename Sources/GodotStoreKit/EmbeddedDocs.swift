#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedStoreKitDocs() {
    _ = loadEmbeddedStoreKitDocsOnce
}

private let loadEmbeddedStoreKitDocsOnce: Void = {
    [
        PackageResources.ProductView_xml,
        PackageResources.StoreKitManager_xml,
        PackageResources.StoreProduct_xml,
        PackageResources.StoreProductPaymentMode_xml,
        PackageResources.StoreProductPurchaseOption_xml,
        PackageResources.StoreProductSubscriptionOffer_xml,
        PackageResources.StoreProductSubscriptionPeriod_xml,
        PackageResources.StoreSubscriptionInfo_xml,
        PackageResources.StoreSubscriptionInfoRenewalInfo_xml,
        PackageResources.StoreSubscriptionInfoStatus_xml,
        PackageResources.StoreTransaction_xml,
        PackageResources.StoreView_xml,
        PackageResources.SubscriptionOfferView_xml,
        PackageResources.SubscriptionStoreView_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
