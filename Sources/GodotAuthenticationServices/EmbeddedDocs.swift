#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedAuthenticationServicesDocs() {
    _ = loadEmbeddedAuthenticationServicesDocsOnce
}

private let loadEmbeddedAuthenticationServicesDocsOnce: Void = {
    [
        PackageResources.ASAuthorizationAppleIDCredential_xml,
        PackageResources.ASAuthorizationController_xml,
        PackageResources.ASPasswordCredential_xml,
        PackageResources.ASWebAuthenticationSession_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
