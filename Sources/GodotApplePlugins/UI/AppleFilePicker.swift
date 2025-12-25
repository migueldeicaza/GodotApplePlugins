//
//  AppleFilePicker.swift
//  GodotApplePlugins
//
//  Created by Miguel de Icaza on 12/18/25.
//

@preconcurrency import SwiftGodotRuntime
import Foundation
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif
import UniformTypeIdentifiers

@Godot
public class AppleFilePicker: RefCounted, @unchecked Sendable {
    // AppleURL, path
    @Signal("url", "path") var file_selected: SignalWithArguments<AppleURL, String>
    
    // [AppleURL], [path]
    @Signal("urls", "paths") var files_selected: SignalWithArguments<TypedArray<AppleURL?>, PackedStringArray>

    @Signal var canceled: SimpleSignal
    
    // Maintain a strong reference to the delegate so it isn't deallocated
    private var delegate: AnyObject?
    
#if os(iOS)
    class PickerDelegate: NSObject, UIDocumentPickerDelegate {
        weak var parent: AppleFilePicker?
        let allowMultiple: Bool
        
        init(_ parent: AppleFilePicker, allowMultiple: Bool) {
            self.parent = parent
            self.allowMultiple = allowMultiple
        }
        
        func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentsAt urls: [URL]) {
            defer { parent?.delegate = nil }
            guard !urls.isEmpty else {
                parent?.canceled.emit()
                return
            }
            
            if allowMultiple {
                let appleUrls = TypedArray<AppleURL?>()
                let paths = PackedStringArray()
                
                for url in urls {
                    let appleURL = AppleURL()
                    appleURL.url = url
                    appleUrls.append(appleURL)
                    paths.append(url.path)
                }
                parent?.files_selected.emit(appleUrls, paths)
            } else {
                if let url = urls.first {
                    let appleURL = AppleURL()
                    appleURL.url = url
                    parent?.file_selected.emit(appleURL, url.path)
                }
            }
        }
        
        func documentPickerWasCancelled(_ controller: UIDocumentPickerViewController) {
            parent?.delegate = nil
            parent?.canceled.emit()
        }
    }
#endif
    
    @Callable
    func pick_document(allowedTypes: [String], allowMultiple: Bool = false) {
        // Convert string extensions/types to UTTypes
        var utTypes: [UTType] = []
        for ext in allowedTypes {
            if let type = UTType(filenameExtension: ext) {
                utTypes.append(type)
            } else if let type = UTType(ext) {
                 utTypes.append(type)
            }
        }
        
        // Default to content if no valid types provided, or maybe just public.item
        if utTypes.isEmpty {
            utTypes.append(.content)
        }
        
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            self.showPicker(types: utTypes, allowMultiple: allowMultiple)
        }
    }
    
    @MainActor
    private func showPicker(types: [UTType], allowMultiple: Bool) {
#if os(iOS)
        let picker = UIDocumentPickerViewController(forOpeningContentTypes: types, asCopy: false)
        let delegate = PickerDelegate(self, allowMultiple: allowMultiple)
        self.delegate = delegate
        picker.delegate = delegate
        picker.allowsMultipleSelection = allowMultiple
        
        presentOnTop(picker)
        
#elseif os(macOS)
        let panel = NSOpenPanel()
        panel.allowedContentTypes = types
        panel.allowsMultipleSelection = allowMultiple
        panel.canChooseDirectories = false
        panel.canChooseFiles = true
        
        let handler: (NSApplication.ModalResponse) -> Void = { [weak self] response in
            guard let self = self else { return }
            if response == .OK {
                if allowMultiple {
                    let appleUrls = TypedArray<AppleURL?>()
                    let paths = PackedStringArray()

                    for url in panel.urls {
                        let appleURL = AppleURL()
                        appleURL.url = url
                        appleUrls.append(appleURL)
                        paths.append(url.path)
                    }
                     self.files_selected.emit(appleUrls, paths)
                } else if let url = panel.url {
                    let appleURL = AppleURL()
                    appleURL.url = url
                    self.file_selected.emit(appleURL, url.path)
                }
            } else {
                self.canceled.emit()
            }
        }

        if let window = NSApplication.shared.keyWindow ?? NSApplication.shared.mainWindow {
            panel.beginSheetModal(for: window, completionHandler: handler)
        } else {
             panel.begin(completionHandler: handler)
        }
#endif
    }
}
