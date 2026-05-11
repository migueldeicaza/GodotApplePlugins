//
//  FoundationUUID.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/14/25.
//

@preconcurrency import SwiftGodotRuntime
import Foundation

@Godot
class Foundation: RefCounted, @unchecked Sendable {

    @Callable static func uuid() -> String {
        UUID().uuidString
    }
}
