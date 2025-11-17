//
//  OSIntegration.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 11/17/25.
//
import SwiftGodotRuntime
import Foundation

extension PackedByteArray {
    public func asData() -> Data? {
        return withUnsafeAccessToData { ptr, count in Data (bytes: ptr, count: count) }
    }
}

extension Data {
    public func toPackedByteArray() -> PackedByteArray {
        let byteArray = [UInt8](self)
        return PackedByteArray(byteArray)
    }
}
