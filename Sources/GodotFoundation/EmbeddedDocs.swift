#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedFoundationDocs() {
    _ = loadEmbeddedFoundationDocsOnce
}

private let loadEmbeddedFoundationDocsOnce: Void = {
    [
        PackageResources.AppleFilePicker_xml,
        PackageResources.AppleURL_xml,
        PackageResources.Foundation_xml,
        PackageResources.SignalProxy_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
