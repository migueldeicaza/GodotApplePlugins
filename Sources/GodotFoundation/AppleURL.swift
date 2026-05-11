//
//  AppleURL.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/18/25.
//

@preconcurrency import SwiftGodotRuntime
import Foundation

@Godot
public class AppleURL: RefCounted, @unchecked Sendable {
    var url: URL?

    @Callable
    func get_path() -> String {
        url?.path(percentEncoded: false) ?? ""
    }

    @Callable
    func get_path_encoded() -> String {
        url?.path(percentEncoded: true) ?? ""
    }

    @Callable
    func set_value(_ str: String) -> Bool {
        if let newurl = URL(string: str) {
            url = newurl
            return true
        }
        return false
    }

    @Callable
    func set_from_filepath(_ path: String) {
        url = URL(fileURLWithPath: path)
    }

    @Callable
    func start_accessing_security_scoped_resource() -> Bool {
        return url?.startAccessingSecurityScopedResource() ?? false
    }
    
    @Callable
    func stop_accessing_security_scoped_resource() {
        url?.stopAccessingSecurityScopedResource()
    }
    
    @Callable
    func get_absolute_string() -> String {
        return url?.absoluteString ?? ""
    }

    @Callable
    func get_string() -> String {
        guard let url else { return "" }
        do {
            return try String(contentsOf: url)
        } catch {
            GD.print("Error reading string from URL: \(error.localizedDescription)")
            return ""
        }
    }
    
    @Callable
    func get_data() -> PackedByteArray {
        guard let url else { return PackedByteArray() }
        do {
            let data = try Data(contentsOf: url)
            return PackedByteArray([UInt8](data))
        } catch {
            GD.print("Error reading data from URL: \(error.localizedDescription)")
            return PackedByteArray()
        }
    }
}
