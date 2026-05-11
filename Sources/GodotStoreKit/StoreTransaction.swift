//
//  StoreTransaction.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/21/25.
//

@preconcurrency import SwiftGodotRuntime
import StoreKit

@Godot
class StoreTransaction: RefCounted, @unchecked Sendable {
    var transaction: Transaction?

    convenience init(_ transaction: Transaction) {
        self.init()
        self.transaction = transaction
    }

    @Export var transactionId: Int { Int(bitPattern: UInt(transaction?.id ?? 0)) }
    @Export var originalID: Int { Int(bitPattern: UInt(transaction?.originalID ?? 0)) }
    @Export var productID: String { transaction?.productID ?? "" }
    @Export var purchaseDate: Double { transaction?.purchaseDate.timeIntervalSince1970 ?? 0 }
    @Export var expirationDate: Double { transaction?.expirationDate?.timeIntervalSince1970 ?? 0 }
    @Export var revocationDate: Double { transaction?.revocationDate?.timeIntervalSince1970 ?? 0 }
    @Export var isUpgraded: Bool { transaction?.isUpgraded ?? false }
    
    @Export var ownershipType: String {
        guard let transaction else { return "unknown" }
        switch transaction.ownershipType {
        case .purchased: return "purchased"
        case .familyShared: return "familyShared"
        default: return "unknown"
        }
    }

    @Callable
    func finish() {
        Task {
            await transaction?.finish()
        }
    }
}
