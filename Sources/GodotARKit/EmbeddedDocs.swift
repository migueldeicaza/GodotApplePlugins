#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedARKitDocs() {
    _ = loadEmbeddedARKitDocsOnce
}

private let loadEmbeddedARKitDocsOnce: Void = {
    [
        PackageResources.ARAnchor_xml,
        PackageResources.ARBodyAnchor_xml,
        PackageResources.ARBodySkeleton_xml,
        PackageResources.ARBodyTrackingConfiguration_xml,
        PackageResources.ARCamera_xml,
        PackageResources.ARCoachingOverlay_xml,
        PackageResources.ARCollaborationData_xml,
        PackageResources.AREnvironmentProbeAnchor_xml,
        PackageResources.ARFaceAnchor_xml,
        PackageResources.ARFrame_xml,
        PackageResources.ARGeoAnchor_xml,
        PackageResources.ARGeoTrackingConfiguration_xml,
        PackageResources.ARHandAnchor_xml,
        PackageResources.ARHandSkeleton_xml,
        PackageResources.ARImageAnchor_xml,
        PackageResources.ARLightEstimate_xml,
        PackageResources.ARMeshAnchor_xml,
        PackageResources.ARPlaneAnchor_xml,
        PackageResources.ARPointCloud_xml,
        PackageResources.ARRaycastQuery_xml,
        PackageResources.ARRaycastResult_xml,
        PackageResources.ARSession_xml,
        PackageResources.ARTrackedRaycast_xml,
        PackageResources.ARWorldMap_xml,
        PackageResources.ARWorldTrackingConfiguration_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
