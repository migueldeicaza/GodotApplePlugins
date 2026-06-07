#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedAVFoundationDocs() {
    _ = loadEmbeddedAVFoundationDocsOnce
}

private let loadEmbeddedAVFoundationDocsOnce: Void = {
    [
        PackageResources.AVAudioSession_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
