#if os(macOS)
import SwiftGodotRuntime

func loadEmbeddedCoreMotionDocs() {
    _ = loadEmbeddedCoreMotionDocsOnce
}

private let loadEmbeddedCoreMotionDocsOnce: Void = {
    [
        PackageResources.CMAbsoluteAltitudeData_xml,
        PackageResources.CMAccelerometerData_xml,
        PackageResources.CMAltimeter_xml,
        PackageResources.CMAltitudeData_xml,
        PackageResources.CMDeviceMotion_xml,
        PackageResources.CMGyroData_xml,
        PackageResources.CMHeadphoneMotionManager_xml,
        PackageResources.CMMagnetometerData_xml,
        PackageResources.CMMotionActivity_xml,
        PackageResources.CMMotionActivityManager_xml,
        PackageResources.CMMotionManager_xml,
        PackageResources.CMPedometer_xml,
        PackageResources.CMPedometerData_xml,
    ].forEach(EditorInterop.loadHelp(buffer:))
}()
#endif
