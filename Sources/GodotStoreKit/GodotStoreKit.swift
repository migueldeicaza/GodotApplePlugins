import SwiftGodotRuntime

private func makeGodotApplePluginsStoreKitTypes() -> [ExtensionInitializationLevel: [Object.Type]] {
    do {
        return try [
            ProductView.self,
            StoreProduct.self,
            StoreProductPurchaseOption.self,
            StoreProductSubscriptionOffer.self,
            StoreProductPaymentMode.self,
            StoreProductSubscriptionPeriod.self,
            StoreSubscriptionInfo.self,
            StoreSubscriptionInfoStatus.self,
            StoreSubscriptionInfoRenewalInfo.self,
            StoreTransaction.self,
            StoreKitManager.self,
            StoreView.self,
            SubscriptionOfferView.self,
            SubscriptionStoreView.self,
        ].prepareForRegistration()
    } catch {
        fatalError("Failed to prepare StoreKit registrations: \(error)")
    }
}

private let godotApplePluginsStoreKitTypes = makeGodotApplePluginsStoreKitTypes()

public let godotApplePluginsStoreKitMinimumInitializationLevel = minimumInitializationLevel(
    for: godotApplePluginsStoreKitTypes
)

public func godotApplePluginsStoreKitInitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsStoreKitTypes[level]?.forEach(register)
    if level == .scene {
        registerEnum(ProductView.ViewStyle.self)
        registerEnum(StoreKitManager.StoreKitStatus.self)
        registerEnum(StoreKitManager.VerificationError.self)
        registerEnum(SubscriptionStoreView.ControlStyle.self)
        registerEnum(StoreProductSubscriptionOffer.OfferType.self)
        registerEnum(StoreProductSubscriptionPeriod.Unit.self)
        registerEnum(StoreSubscriptionInfoStatus.RenewalState.self)
    } else if level == .editor {
#if os(macOS)
        loadEmbeddedStoreKitDocs()
#endif
    }
}

public func godotApplePluginsStoreKitDeinitialize(level: ExtensionInitializationLevel) {
    godotApplePluginsStoreKitTypes[level]?.reversed().forEach(unregister)
}

@_cdecl("godot_apple_plugins_storekit_start")
public func godotApplePluginsStoreKitStart(interface: OpaquePointer?, library: OpaquePointer?, extension: OpaquePointer?) -> UInt8 {
    guard let interface, let library, let `extension` else {
        print("Error: Not all parameters were initialized.")
        return 0
    }

    initializeSwiftModule(
        interface,
        library,
        `extension`,
        initHook: godotApplePluginsStoreKitInitialize,
        deInitHook: godotApplePluginsStoreKitDeinitialize,
        minimumInitializationLevel: godotApplePluginsStoreKitMinimumInitializationLevel
    )
    return 1
}
