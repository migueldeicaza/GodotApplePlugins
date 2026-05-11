//
//  ARSession.swift
//  GodotApplePlugins
//
//

@preconcurrency import SwiftGodotRuntime
import Foundation

#if canImport(ARKit) && os(iOS)
import ARKit

@Godot
class ARSession: RefCounted, @unchecked Sendable {
    enum RunOption: Int, CaseIterable {
        case RESET_TRACKING = 1
        case REMOVE_EXISTING_ANCHORS = 2
        case STOP_TRACKED_RAYCASTS = 4
        case RESET_SCENE_RECONSTRUCTION = 8
    }

    @Signal("frame") var frame_updated: SignalWithArguments<ARFrame>
    @Signal("anchors") var anchors_added: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_updated: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_removed: SignalWithArguments<VariantArray>
    @Signal("camera") var camera_tracking_changed: SignalWithArguments<ARCamera>
    @Signal("message") var session_failed: SignalWithArguments<String>
    @Signal var session_interrupted: SimpleSignal
    @Signal var interruption_ended: SimpleSignal
    @Signal("hand") var hand_anchor_updated: SignalWithArguments<ARHandAnchor>
    @Signal("mesh") var mesh_anchor_added: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_updated: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_removed: SignalWithArguments<ARMeshAnchor>
    @Signal("collaborationData") var collaboration_data_received: SignalWithArguments<ARCollaborationData>

    private var delegateProxy: Proxy?
    private var lastConfiguration: ARWorldTrackingConfiguration?

    lazy var session: ARKit.ARSession = {
        let session = ARKit.ARSession()
        let proxy = Proxy(self)
        delegateProxy = proxy
        session.delegate = proxy
        return session
    }()

    class Proxy: NSObject, ARSessionDelegate {
        weak var base: ARSession?

        init(_ base: ARSession) {
            self.base = base
        }

        func session(_ session: ARKit.ARSession, didUpdate frame: ARKit.ARFrame) {
            DispatchQueue.main.async { [weak base] in
                base?.frame_updated.emit(ARFrame(frame: frame))
            }
        }

        func session(_ session: ARKit.ARSession, didAdd anchors: [ARKit.ARAnchor]) {
            DispatchQueue.main.async { [weak base] in
                base?.anchors_added.emit(wrapAnchors(anchors))
                for anchor in anchors {
                    if let meshAnchor = anchor as? ARKit.ARMeshAnchor {
                        base?.mesh_anchor_added.emit(ARMeshAnchor(anchor: meshAnchor))
                    }
                }
            }
        }

        func session(_ session: ARKit.ARSession, didUpdate anchors: [ARKit.ARAnchor]) {
            DispatchQueue.main.async { [weak base] in
                base?.anchors_updated.emit(wrapAnchors(anchors))
                for anchor in anchors {
                    if let meshAnchor = anchor as? ARKit.ARMeshAnchor {
                        base?.mesh_anchor_updated.emit(ARMeshAnchor(anchor: meshAnchor))
                    }
                }
            }
        }

        func session(_ session: ARKit.ARSession, didRemove anchors: [ARKit.ARAnchor]) {
            DispatchQueue.main.async { [weak base] in
                base?.anchors_removed.emit(wrapAnchors(anchors))
                for anchor in anchors {
                    if let meshAnchor = anchor as? ARKit.ARMeshAnchor {
                        base?.mesh_anchor_removed.emit(ARMeshAnchor(anchor: meshAnchor))
                    }
                }
            }
        }

        func session(_ session: ARKit.ARSession, cameraDidChangeTrackingState camera: ARKit.ARCamera) {
            DispatchQueue.main.async { [weak base] in
                base?.camera_tracking_changed.emit(ARCamera(camera: camera))
            }
        }

        func session(_ session: ARKit.ARSession, didFailWithError error: Error) {
            DispatchQueue.main.async { [weak base] in
                base?.session_failed.emit(error.localizedDescription)
            }
        }

        func sessionWasInterrupted(_ session: ARKit.ARSession) {
            DispatchQueue.main.async { [weak base] in
                base?.session_interrupted.emit()
            }
        }

        func sessionInterruptionEnded(_ session: ARKit.ARSession) {
            DispatchQueue.main.async { [weak base] in
                base?.interruption_ended.emit()
            }
        }

        @available(iOS 13.0, *)
        func session(_ session: ARKit.ARSession, didOutputCollaborationData data: ARKit.ARSession.CollaborationData) {
            DispatchQueue.main.async { [weak base] in
                base?.collaboration_data_received.emit(ARCollaborationData(collaborationData: data))
            }
        }
    }

    @Export var identifier: String {
        if #available(iOS 13.0, *) {
            return session.identifier.uuidString
        }
        return ""
    }

    @Export var currentFrame: ARFrame? {
        guard let frame = session.currentFrame else { return nil }
        return ARFrame(frame: frame)
    }

    @Export var anchors: VariantArray {
        guard let frame = session.currentFrame else { return VariantArray() }
        return wrapAnchors(frame.anchors)
    }

    @Export var configuration: ARWorldTrackingConfiguration? {
        lastConfiguration
    }

    @Callable
    static func is_supported() -> Bool {
        ARKit.ARWorldTrackingConfiguration.isSupported
    }

    @Callable
    func run(configuration: ARWorldTrackingConfiguration, options: Int = 0) {
        lastConfiguration = configuration
        session.run(configuration.makeNative(), options: Self.makeRunOptions(options))
    }

    @Callable
    func run_body_tracking(configuration: ARBodyTrackingConfiguration, options: Int = 0) {
        session.run(configuration.makeNative(), options: Self.makeRunOptions(options))
    }

    @Callable
    func run_geo_tracking(configuration: ARGeoTrackingConfiguration, options: Int = 0) {
        if #available(iOS 14.0, *) {
            session.run(configuration.makeNative(), options: Self.makeRunOptions(options))
        } else {
            session_failed.emit("Geo tracking requires iOS 14.0 or later.")
        }
    }

    @Callable
    func run_face_tracking(configuration: ARFaceTrackingConfiguration, options: Int = 0) {
        session.run(configuration.makeNative(), options: Self.makeRunOptions(options))
    }

    @Callable
    func run_image_tracking(configuration: ARImageTrackingConfiguration, options: Int = 0) {
        session.run(configuration.makeNative(), options: Self.makeRunOptions(options))
    }

    @Callable
    func pause() {
        session.pause()
    }

    @Callable
    func add_anchor(anchor: ARAnchor) {
        guard let nativeAnchor = anchor.anchor else { return }
        session.add(anchor: nativeAnchor)
    }

    @Callable
    func remove_anchor(anchor: ARAnchor) {
        guard let nativeAnchor = anchor.anchor else { return }
        session.remove(anchor: nativeAnchor)
    }

    @Callable
    func remove_all_anchors() {
        guard let frame = session.currentFrame else { return }
        for anchor in frame.anchors {
            session.remove(anchor: anchor)
        }
    }

    @Callable
    func set_world_origin(relativeTransform: Transform3D) {
        session.setWorldOrigin(relativeTransform: simdTransform(relativeTransform))
    }

    @Callable
    func capture_high_resolution_frame(callback: Callable) {
        guard #available(iOS 16.0, *) else {
            _ = callback.call(nil, Variant("High resolution frame capture is unavailable on this OS version"))
            return
        }
        session.captureHighResolutionFrame { frame, error in
            DispatchQueue.main.async {
                if let frame {
                    _ = callback.call(Variant(ARFrame(frame: frame)), mapError(error))
                } else {
                    _ = callback.call(nil, mapError(error))
                }
            }
        }
    }

    @Callable
    func update_with_collaboration_data(data: ARCollaborationData) {
        if #available(iOS 13.0, *) {
            guard let native = data.collaborationData else { return }
            session.update(with: native)
        }
    }

    @Callable
    func get_current_world_map(callback: Callable) {
        session.getCurrentWorldMap { worldMap, error in
            let wrappedWorldMap = worldMap.map { ARWorldMap(worldMap: $0) }
            let mappedError = mapError(error)
            DispatchQueue.main.async {
                if let wrappedWorldMap {
                    _ = callback.call(Variant(wrappedWorldMap), mappedError)
                } else {
                    _ = callback.call(nil, mappedError)
                }
            }
        }
    }

    @Callable
    func run_with_world_map(configuration: ARWorldTrackingConfiguration, worldMap: ARWorldMap) {
        lastConfiguration = configuration
        let nativeConfig = configuration.makeNative()
        if let map = worldMap.worldMap {
            nativeConfig.initialWorldMap = map
        }
        session.run(nativeConfig)
    }

    @Callable
    func raycast(query: ARRaycastQuery) -> VariantArray {
        guard let nativeQuery = query.query else { return VariantArray() }
        let results = session.raycast(nativeQuery)
        let array = VariantArray()
        for result in results {
            array.append(Variant(ARRaycastResult(result: result)))
        }
        return array
    }

    @Callable
    func tracked_raycast(query: ARRaycastQuery, callback: Callable) -> ARTrackedRaycast? {
        guard let nativeQuery = query.query else { return nil }
        let wrapper = ARTrackedRaycast()
        wrapper.trackedRaycast = session.trackedRaycast(nativeQuery) { results in
            let wrappedResults = results.map { ARRaycastResult(result: $0) }
            DispatchQueue.main.async {
                let array = VariantArray()
                for result in wrappedResults {
                    array.append(Variant(result))
                }
                _ = callback.call(Variant(array))
            }
        }
        return wrapper
    }

    private static func makeRunOptions(_ options: Int) -> ARKit.ARSession.RunOptions {
        var result: ARKit.ARSession.RunOptions = []
        if options & RunOption.RESET_TRACKING.rawValue != 0 {
            result.insert(.resetTracking)
        }
        if options & RunOption.REMOVE_EXISTING_ANCHORS.rawValue != 0 {
            result.insert(.removeExistingAnchors)
        }
        if #available(iOS 13.4, *), options & RunOption.STOP_TRACKED_RAYCASTS.rawValue != 0 {
            result.insert(.stopTrackedRaycasts)
        }
        if #available(iOS 13.4, *), options & RunOption.RESET_SCENE_RECONSTRUCTION.rawValue != 0 {
            result.insert(.resetSceneReconstruction)
        }
        return result
    }
}

#elseif canImport(ARKit) && os(visionOS)
import ARKit
import QuartzCore

@Godot
class ARSession: RefCounted, @unchecked Sendable {
    enum RunOption: Int, CaseIterable {
        case RESET_TRACKING = 1
        case REMOVE_EXISTING_ANCHORS = 2
        case STOP_TRACKED_RAYCASTS = 4
        case RESET_SCENE_RECONSTRUCTION = 8
    }

    @Signal("frame") var frame_updated: SignalWithArguments<ARFrame>
    @Signal("anchors") var anchors_added: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_updated: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_removed: SignalWithArguments<VariantArray>
    @Signal("camera") var camera_tracking_changed: SignalWithArguments<ARCamera>
    @Signal("message") var session_failed: SignalWithArguments<String>
    @Signal var session_interrupted: SimpleSignal
    @Signal var interruption_ended: SimpleSignal
    @Signal("hand") var hand_anchor_updated: SignalWithArguments<ARHandAnchor>
    @Signal("mesh") var mesh_anchor_added: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_updated: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_removed: SignalWithArguments<ARMeshAnchor>
    @Signal("collaborationData") var collaboration_data_received: SignalWithArguments<ARCollaborationData>

    private var session = ARKitSession()
    private var sessionUUID = UUID().uuidString
    private var updateTasks: [Task<Void, Never>] = []
    private var worldTrackingProvider: WorldTrackingProvider?
    private var planeDetectionProvider: PlaneDetectionProvider?
    private var sceneReconstructionProvider: SceneReconstructionProvider?
    private var handTrackingProvider: HandTrackingProvider?
    private var imageTrackingProvider: ImageTrackingProvider?
    private var worldAnchorsByIdentifier: [UUID: WorldAnchor] = [:]
    private var planeAnchorsByIdentifier: [UUID: PlaneAnchor] = [:]
    private var meshAnchorsByIdentifier: [UUID: MeshAnchor] = [:]
    private var imageAnchorsByIdentifier: [UUID: ARImageAnchor] = [:]
    private var latestCamera: ARCamera?
    private var latestTimestamp: Double = 0
    private var lastConfiguration: ARWorldTrackingConfiguration?

    @Export var identifier: String {
        sessionUUID
    }

    @Export var currentFrame: ARFrame? {
        guard let camera = latestCamera else { return nil }
        return ARFrame(
            timestamp: latestTimestamp,
            camera: camera,
            anchors: currentAnchors(),
            worldMappingStatus: camera.trackingState == .NORMAL ? .MAPPED : .LIMITED
        )
    }

    @Export var anchors: VariantArray {
        variantArray(currentAnchors())
    }

    @Export var configuration: ARWorldTrackingConfiguration? {
        lastConfiguration
    }

    @Callable
    static func is_supported() -> Bool {
        WorldTrackingProvider.isSupported
    }

    @Callable
    func run(configuration: ARWorldTrackingConfiguration, options: Int = 0) {
        pause()

        guard WorldTrackingProvider.isSupported else {
            session_failed.emit("World tracking is not supported on this visionOS device.")
            return
        }

        lastConfiguration = configuration
        session = ARKitSession()
        sessionUUID = UUID().uuidString

        let worldTrackingProvider = WorldTrackingProvider()
        let planeDetectionProvider = makePlaneDetectionProvider(configuration: configuration)
        let sceneReconstructionProvider = makeSceneReconstructionProvider(configuration: configuration)
        let handTrackingProvider = makeHandTrackingProvider(configuration: configuration)
        let imageTrackingProvider = makeImageTrackingProvider(configuration: configuration)
        let providers = makeProviders(
            worldTrackingProvider: worldTrackingProvider,
            planeDetectionProvider: planeDetectionProvider,
            sceneReconstructionProvider: sceneReconstructionProvider,
            handTrackingProvider: handTrackingProvider,
            imageTrackingProvider: imageTrackingProvider
        )

        self.worldTrackingProvider = worldTrackingProvider
        self.planeDetectionProvider = planeDetectionProvider
        self.sceneReconstructionProvider = sceneReconstructionProvider
        self.handTrackingProvider = handTrackingProvider
        self.imageTrackingProvider = imageTrackingProvider

        Task { [weak self] in
            guard let self else { return }

            var authorizationTypes: [ARKitSession.AuthorizationType] = [.worldSensing]
            if handTrackingProvider != nil {
                authorizationTypes.append(.handTracking)
            }
            _ = await self.session.requestAuthorization(for: authorizationTypes)

            do {
                try await self.session.run(providers)
                await MainActor.run {
                    self.worldAnchorsByIdentifier.removeAll()
                    self.planeAnchorsByIdentifier.removeAll()
                    self.meshAnchorsByIdentifier.removeAll()
                    self.imageAnchorsByIdentifier.removeAll()
                    self.latestCamera = nil
                    self.latestTimestamp = 0
                }
                await self.startTasks(
                    worldTrackingProvider: worldTrackingProvider,
                    planeDetectionProvider: planeDetectionProvider,
                    sceneReconstructionProvider: sceneReconstructionProvider,
                    handTrackingProvider: handTrackingProvider,
                    imageTrackingProvider: imageTrackingProvider
                )

                if #available(visionOS 26.0, *),
                   options & RunOption.REMOVE_EXISTING_ANCHORS.rawValue != 0
                {
                    try? await worldTrackingProvider.removeAllAnchors()
                }
            } catch {
                await self.emitSessionFailedOnMainActor(error.localizedDescription)
            }
        }
    }

    @Callable
    func run_body_tracking(configuration: ARBodyTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Body tracking is not available on visionOS.")
    }

    @Callable
    func run_geo_tracking(configuration: ARGeoTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Geo tracking is not available on visionOS.")
    }

    @Callable
    func run_face_tracking(configuration: ARFaceTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Face tracking configuration is not available on visionOS.")
    }

    @Callable
    func run_image_tracking(configuration: ARImageTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Standalone image tracking configuration is not available on visionOS. Use ARWorldTrackingConfiguration with detectionImageGroupName instead.")
    }

    @Callable
    func pause() {
        for task in updateTasks {
            task.cancel()
        }
        updateTasks.removeAll()
        session.stop()
        worldTrackingProvider = nil
        planeDetectionProvider = nil
        sceneReconstructionProvider = nil
        handTrackingProvider = nil
        imageTrackingProvider = nil
        worldAnchorsByIdentifier.removeAll()
        planeAnchorsByIdentifier.removeAll()
        meshAnchorsByIdentifier.removeAll()
        imageAnchorsByIdentifier.removeAll()
        latestCamera = nil
        latestTimestamp = 0
    }

    @Callable
    func add_anchor(anchor: ARAnchor) {
        guard let provider = worldTrackingProvider else { return }
        let nativeAnchor = anchor.anchor ?? WorldAnchor(originFromAnchorTransform: simdTransform(anchor.transform))

        Task { [weak self] in
            do {
                try await provider.addAnchor(nativeAnchor)
            } catch {
                if let self {
                    await self.emitSessionFailedOnMainActor(error.localizedDescription)
                }
            }
        }
    }

    @Callable
    func remove_anchor(anchor: ARAnchor) {
        guard let provider = worldTrackingProvider else { return }

        if let nativeAnchor = anchor.anchor {
            Task { [weak self] in
                do {
                    try await provider.removeAnchor(nativeAnchor)
                } catch {
                    if let self {
                        await self.emitSessionFailedOnMainActor(error.localizedDescription)
                    }
                }
            }
            return
        }

        guard let identifier = UUID(uuidString: anchor.identifier) else { return }
        Task { [weak self] in
            do {
                try await provider.removeAnchor(forID: identifier)
            } catch {
                if let self {
                    await self.emitSessionFailedOnMainActor(error.localizedDescription)
                }
            }
        }
    }

    @Callable
    func remove_all_anchors() {
        guard let provider = worldTrackingProvider else { return }

        Task { [weak self] in
            if #available(visionOS 26.0, *) {
                do {
                    try await provider.removeAllAnchors()
                } catch {
                    if let self {
                        await self.emitSessionFailedOnMainActor(error.localizedDescription)
                    }
                }
                return
            }

            let identifiers = Array(self?.worldAnchorsByIdentifier.keys ?? [])
            for identifier in identifiers {
                try? await provider.removeAnchor(forID: identifier)
            }
        }
    }

    @Callable
    func set_world_origin(relativeTransform: Transform3D) {}

    @Callable
    func capture_high_resolution_frame(callback: Callable) {
        _ = callback.call(nil, Variant("High resolution frame capture is unavailable on visionOS."))
    }

    // Not available on visionOS: ARKit on visionOS does not expose raycast APIs.
    // Spatial hit-testing is done through RealityKit (CollisionComponent, scene
    // queries) rather than through the ARKit session.
    @Callable
    func raycast(query: ARRaycastQuery) -> VariantArray {
        VariantArray()
    }

    @Callable
    func tracked_raycast(query: ARRaycastQuery, callback: Callable) -> ARTrackedRaycast? {
        nil
    }

    // Not available on visionOS: ARWorldMap and collaboration data are iOS-only
    // APIs. visionOS persists anchors automatically via WorldTrackingProvider and
    // does not expose a collaboration data stream. These are Apple platform
    // limitations, not missing bindings.
    @Callable
    func get_current_world_map(callback: Callable) {
        _ = callback.call(nil, Variant("World map persistence is unavailable on visionOS."))
    }

    @Callable
    func run_with_world_map(configuration: ARWorldTrackingConfiguration, worldMap: ARWorldMap) {
        session_failed.emit("World map persistence is unavailable on visionOS.")
    }

    @Callable
    func update_with_collaboration_data(data: ARCollaborationData) {}

    private func makeProviders(
        worldTrackingProvider: WorldTrackingProvider,
        planeDetectionProvider: PlaneDetectionProvider?,
        sceneReconstructionProvider: SceneReconstructionProvider?,
        handTrackingProvider: HandTrackingProvider?,
        imageTrackingProvider: ImageTrackingProvider?
    ) -> [any DataProvider] {
        var providers: [any DataProvider] = [worldTrackingProvider]
        if let planeDetectionProvider {
            providers.append(planeDetectionProvider)
        }
        if let sceneReconstructionProvider {
            providers.append(sceneReconstructionProvider)
        }
        if let handTrackingProvider {
            providers.append(handTrackingProvider)
        }
        if let imageTrackingProvider {
            providers.append(imageTrackingProvider)
        }
        return providers
    }

    private func makePlaneDetectionProvider(configuration: ARWorldTrackingConfiguration) -> PlaneDetectionProvider? {
        let alignments = configuration.makePlaneAlignments()
        guard !alignments.isEmpty, PlaneDetectionProvider.isSupported else { return nil }
        return PlaneDetectionProvider(alignments: alignments)
    }

    private func makeSceneReconstructionProvider(configuration: ARWorldTrackingConfiguration) -> SceneReconstructionProvider? {
        guard configuration.sceneReconstruction != .NONE, SceneReconstructionProvider.isSupported else { return nil }
        return SceneReconstructionProvider()
    }

    private func makeHandTrackingProvider(configuration: ARWorldTrackingConfiguration) -> HandTrackingProvider? {
        guard configuration.handTrackingEnabled, HandTrackingProvider.isSupported else { return nil }
        return HandTrackingProvider()
    }

    private func makeImageTrackingProvider(configuration: ARWorldTrackingConfiguration) -> ImageTrackingProvider? {
        guard configuration.maximumNumberOfTrackedImages > 0,
              !configuration.detectionImageGroupName.isEmpty,
              ImageTrackingProvider.isSupported
        else { return nil }

        guard let referenceImages = ReferenceImage.loadReferenceImages(inGroupNamed: configuration.detectionImageGroupName) else {
            return nil
        }
        return ImageTrackingProvider(referenceImages: Array(referenceImages))
    }

    private func startTasks(
        worldTrackingProvider: WorldTrackingProvider,
        planeDetectionProvider: PlaneDetectionProvider?,
        sceneReconstructionProvider: SceneReconstructionProvider?,
        handTrackingProvider: HandTrackingProvider?,
        imageTrackingProvider: ImageTrackingProvider?
    ) async {
        updateTasks.append(Task { [weak self] in
            guard let self else { return }
            for await update in worldTrackingProvider.anchorUpdates {
                await self.handleWorldAnchorUpdate(update)
            }
        })

        if let planeDetectionProvider {
            updateTasks.append(Task { [weak self] in
                guard let self else { return }
                for await update in planeDetectionProvider.anchorUpdates {
                    await self.handlePlaneAnchorUpdate(update)
                }
            })
        }

        if let sceneReconstructionProvider {
            updateTasks.append(Task { [weak self] in
                guard let self else { return }
                for await update in sceneReconstructionProvider.anchorUpdates {
                    await self.handleMeshAnchorUpdate(update)
                }
            })
        }

        if let handTrackingProvider {
            updateTasks.append(Task { [weak self] in
                guard let self else { return }
                for await update in handTrackingProvider.anchorUpdates {
                    await self.handleHandAnchorUpdate(update)
                }
            })
        }

        if let imageTrackingProvider {
            updateTasks.append(Task { [weak self] in
                guard let self else { return }
                for await update in imageTrackingProvider.anchorUpdates {
                    await self.handleImageAnchorUpdate(update)
                }
            })
        }

        updateTasks.append(Task { [weak self] in
            guard let self else { return }
            while !Task.isCancelled {
                if let deviceAnchor = worldTrackingProvider.queryDeviceAnchor(atTimestamp: CACurrentMediaTime()) {
                    await self.publishFrame(deviceAnchor: deviceAnchor)
                }
                try? await Task.sleep(nanoseconds: 33_000_000)
            }
        })
    }

    private func handleWorldAnchorUpdate(_ update: AnchorUpdate<WorldAnchor>) async {
        let wrappedAnchor = ARAnchor(anchor: update.anchor)

        await MainActor.run {
            switch update.event {
            case .added:
                worldAnchorsByIdentifier[update.anchor.id] = update.anchor
                anchors_added.emit(variantArray([wrappedAnchor]))
            case .updated:
                worldAnchorsByIdentifier[update.anchor.id] = update.anchor
                anchors_updated.emit(variantArray([wrappedAnchor]))
            case .removed:
                worldAnchorsByIdentifier.removeValue(forKey: update.anchor.id)
                anchors_removed.emit(variantArray([wrappedAnchor]))
            @unknown default:
                break
            }
        }
    }

    private func handlePlaneAnchorUpdate(_ update: AnchorUpdate<PlaneAnchor>) async {
        let wrappedAnchor = ARPlaneAnchor(anchor: update.anchor)

        await MainActor.run {
            switch update.event {
            case .added:
                planeAnchorsByIdentifier[update.anchor.id] = update.anchor
                anchors_added.emit(variantArray([wrappedAnchor]))
            case .updated:
                planeAnchorsByIdentifier[update.anchor.id] = update.anchor
                anchors_updated.emit(variantArray([wrappedAnchor]))
            case .removed:
                planeAnchorsByIdentifier.removeValue(forKey: update.anchor.id)
                anchors_removed.emit(variantArray([wrappedAnchor]))
            @unknown default:
                break
            }
        }
    }

    private func handleMeshAnchorUpdate(_ update: AnchorUpdate<MeshAnchor>) async {
        let wrappedAnchor = ARMeshAnchor(anchor: update.anchor)

        await MainActor.run {
            switch update.event {
            case .added:
                meshAnchorsByIdentifier[update.anchor.id] = update.anchor
                mesh_anchor_added.emit(wrappedAnchor)
                anchors_added.emit(variantArray([wrappedAnchor]))
            case .updated:
                meshAnchorsByIdentifier[update.anchor.id] = update.anchor
                mesh_anchor_updated.emit(wrappedAnchor)
                anchors_updated.emit(variantArray([wrappedAnchor]))
            case .removed:
                meshAnchorsByIdentifier.removeValue(forKey: update.anchor.id)
                mesh_anchor_removed.emit(wrappedAnchor)
                anchors_removed.emit(variantArray([wrappedAnchor]))
            @unknown default:
                break
            }
        }
    }

    private func handleHandAnchorUpdate(_ update: AnchorUpdate<HandAnchor>) async {
        let wrappedAnchor = ARHandAnchor(anchor: update.anchor)

        await MainActor.run {
            hand_anchor_updated.emit(wrappedAnchor)
        }
    }

    private func handleImageAnchorUpdate(_ update: AnchorUpdate<ImageAnchor>) async {
        let wrappedAnchor = ARImageAnchor()
        wrappedAnchor.identifier = update.anchor.id.uuidString
        wrappedAnchor.transform = godotTransform3D(update.anchor.originFromAnchorTransform)
        wrappedAnchor.referenceImageName = update.anchor.referenceImage.name ?? ""
        wrappedAnchor.referenceImagePhysicalSize = Vector2(
            x: Float(update.anchor.referenceImage.physicalSize.width),
            y: Float(update.anchor.referenceImage.physicalSize.height)
        )
        wrappedAnchor.isTracked = update.anchor.isTracked
        wrappedAnchor.estimatedScaleFactor = Double(update.anchor.estimatedScaleFactor)

        await MainActor.run {
            switch update.event {
            case .added:
                imageAnchorsByIdentifier[update.anchor.id] = wrappedAnchor
                anchors_added.emit(variantArray([wrappedAnchor]))
            case .updated:
                imageAnchorsByIdentifier[update.anchor.id] = wrappedAnchor
                anchors_updated.emit(variantArray([wrappedAnchor]))
            case .removed:
                imageAnchorsByIdentifier.removeValue(forKey: update.anchor.id)
                anchors_removed.emit(variantArray([wrappedAnchor]))
            @unknown default:
                break
            }
        }
    }

    private func publishFrame(deviceAnchor: DeviceAnchor) async {
        let camera = ARCamera(deviceAnchor: deviceAnchor)
        let timestamp: Double
        if #available(visionOS 2.0, *) {
            timestamp = deviceAnchor.timestamp
        } else {
            timestamp = CACurrentMediaTime()
        }

        await MainActor.run {
            latestCamera = camera
            latestTimestamp = timestamp

            let frame = ARFrame(
                timestamp: timestamp,
                camera: camera,
                anchors: currentAnchors(),
                worldMappingStatus: camera.trackingState == .NORMAL ? .MAPPED : .LIMITED
            )
            camera_tracking_changed.emit(camera)
            frame_updated.emit(frame)
        }
    }

    @MainActor
    private func emitSessionFailedOnMainActor(_ message: String) {
        session_failed.emit(message)
    }

    private func currentAnchors() -> [RefCounted] {
        let worldAnchors = worldAnchorsByIdentifier.keys.sorted(by: { $0.uuidString < $1.uuidString }).compactMap {
            worldAnchorsByIdentifier[$0].map { ARAnchor(anchor: $0) as RefCounted }
        }
        let planeAnchors = planeAnchorsByIdentifier.keys.sorted(by: { $0.uuidString < $1.uuidString }).compactMap {
            planeAnchorsByIdentifier[$0].map { ARPlaneAnchor(anchor: $0) as RefCounted }
        }
        let meshAnchors = meshAnchorsByIdentifier.keys.sorted(by: { $0.uuidString < $1.uuidString }).compactMap {
            meshAnchorsByIdentifier[$0].map { ARMeshAnchor(anchor: $0) as RefCounted }
        }
        let imageAnchors = imageAnchorsByIdentifier.keys.sorted(by: { $0.uuidString < $1.uuidString }).compactMap {
            imageAnchorsByIdentifier[$0] as RefCounted?
        }
        return worldAnchors + planeAnchors + meshAnchors + imageAnchors
    }
}

#elseif canImport(ARKit) && os(macOS)
import AppKit
import ARKit
import QuartzCore
import SwiftUI
import _ARKit_SwiftUI

@available(macOS 26.0, *)
private struct ARKitRemoteDeviceIdentifierReader: View {
    @Environment(\.remoteDeviceIdentifier) private var remoteDeviceIdentifier
    let onRead: (RemoteDeviceIdentifier?) -> Void

    var body: some View {
        Color.clear
            .frame(width: 1, height: 1)
            .onAppear {
                onRead(remoteDeviceIdentifier)
            }
    }
}

// macOS ARKit backend: remote-session bridge only (macOS 26.0+).
//
// This backend connects to a remote iOS/visionOS device via ARKitSession(device:)
// and relays DeviceAnchor pose data for camera tracking. It is intentionally a
// narrow bridge that mirrors Apple's remote-device model, which has significant
// limitations compared to local ARKit on iOS or visionOS:
//
//   - Anchor CRUD (add/remove) is not supported by the remote API
//   - World-origin reset is a no-op (no local coordinate system to relocate)
//   - High-res capture, raycasting, world maps, and collaboration are unavailable
//   - AnchorUpdate streams are not available on macOS, so world/plane/mesh/image
//     anchors cannot be tracked; only DeviceAnchor (camera pose) is queryable
//
// These are Apple platform limitations, not missing bindings.

@Godot
class ARSession: RefCounted, @unchecked Sendable {
    enum RunOption: Int, CaseIterable {
        case RESET_TRACKING = 1
        case REMOVE_EXISTING_ANCHORS = 2
        case STOP_TRACKED_RAYCASTS = 4
        case RESET_SCENE_RECONSTRUCTION = 8
    }

    @Signal("frame") var frame_updated: SignalWithArguments<ARFrame>
    @Signal("anchors") var anchors_added: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_updated: SignalWithArguments<VariantArray>
    @Signal("anchors") var anchors_removed: SignalWithArguments<VariantArray>
    @Signal("camera") var camera_tracking_changed: SignalWithArguments<ARCamera>
    @Signal("message") var session_failed: SignalWithArguments<String>
    @Signal var session_interrupted: SimpleSignal
    @Signal var interruption_ended: SimpleSignal
    @Signal("hand") var hand_anchor_updated: SignalWithArguments<ARHandAnchor>
    @Signal("mesh") var mesh_anchor_added: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_updated: SignalWithArguments<ARMeshAnchor>
    @Signal("mesh") var mesh_anchor_removed: SignalWithArguments<ARMeshAnchor>
    @Signal("collaborationData") var collaboration_data_received: SignalWithArguments<ARCollaborationData>

    private var macSession: AnyObject?
    private var macWorldTrackingProvider: AnyObject?

    private var sessionUUID = UUID().uuidString
    private var updateTasks: [Task<Void, Never>] = []
    private var worldAnchorsByIdentifier: [UUID: ARAnchor] = [:]
    private var latestCamera: ARCamera?
    private var latestTimestamp: Double = 0
    private var lastConfiguration: ARWorldTrackingConfiguration?

    @Export var identifier: String {
        sessionUUID
    }

    @Export var currentFrame: ARFrame? {
        guard let camera = latestCamera else { return nil }
        return ARFrame(
            timestamp: latestTimestamp,
            camera: camera,
            anchors: currentAnchors(),
            worldMappingStatus: camera.trackingState == .NORMAL ? .MAPPED : .LIMITED
        )
    }

    @Export var anchors: VariantArray {
        variantArray(currentAnchors())
    }

    @Export var configuration: ARWorldTrackingConfiguration? {
        lastConfiguration
    }

    @Callable
    static func is_supported() -> Bool {
        if #available(macOS 26.0, *) {
            return WorldTrackingProvider.isSupported
        }
        return false
    }

    @Callable
    func run(configuration: ARWorldTrackingConfiguration, options: Int = 0) {
        pause()
        lastConfiguration = configuration

        guard #available(macOS 26.0, *) else {
            session_failed.emit("ARKit remote sessions require macOS 26.0 or newer.")
            return
        }
        guard WorldTrackingProvider.isSupported else {
            session_failed.emit("World tracking is not supported on this Mac.")
            return
        }
        guard let remoteDeviceIdentifier = resolveRemoteDeviceIdentifier() else {
            session_failed.emit(
                "No SwiftUI remote device identifier was available. Launch this app in a remote device session before starting ARKit."
            )
            return
        }

        let session = ARKitSession(device: remoteDeviceIdentifier)
        let worldTrackingProvider = WorldTrackingProvider()
        self.macSession = session
        self.macWorldTrackingProvider = worldTrackingProvider
        sessionUUID = UUID().uuidString

        Task { [weak self] in
            do {
                try await session.run([worldTrackingProvider])
                await self?.startMacTasks(session: session, worldTrackingProvider: worldTrackingProvider)
            } catch {
                if let self {
                    await self.emitSessionFailedOnMainActor(error.localizedDescription)
                }
            }
        }
    }

    @Callable
    func run_body_tracking(configuration: ARBodyTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Body tracking is not available on macOS.")
    }

    @Callable
    func run_geo_tracking(configuration: ARGeoTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Geo tracking is not available on macOS.")
    }

    @Callable
    func run_face_tracking(configuration: ARFaceTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Face tracking is not available on macOS.")
    }

    @Callable
    func run_image_tracking(configuration: ARImageTrackingConfiguration, options: Int = 0) {
        session_failed.emit("Image tracking is not available on macOS.")
    }

    @Callable
    func pause() {
        for task in updateTasks {
            task.cancel()
        }
        updateTasks.removeAll()
        if #available(macOS 26.0, *) {
            (macSession as? ARKitSession)?.stop()
            macSession = nil
            macWorldTrackingProvider = nil
        }
        worldAnchorsByIdentifier.removeAll()
        latestCamera = nil
        latestTimestamp = 0
    }

    @Callable
    func add_anchor(anchor: ARAnchor) {
        session_failed.emit("macOS remote ARKit sessions do not expose world anchor creation in this binding.")
    }

    @Callable
    func remove_anchor(anchor: ARAnchor) {
        session_failed.emit("macOS remote ARKit sessions do not expose world anchor removal in this binding.")
    }

    @Callable
    func remove_all_anchors() {
        session_failed.emit("macOS remote ARKit sessions do not expose world anchor removal in this binding.")
    }

    @Callable
    func set_world_origin(relativeTransform: Transform3D) {}

    @Callable
    func capture_high_resolution_frame(callback: Callable) {
        _ = callback.call(nil, Variant("High resolution frame capture is unavailable on macOS ARKit remote sessions."))
    }

    // Not available on macOS: remote ARKit sessions do not support raycasting.
    @Callable
    func raycast(query: ARRaycastQuery) -> VariantArray {
        VariantArray()
    }

    @Callable
    func tracked_raycast(query: ARRaycastQuery, callback: Callable) -> ARTrackedRaycast? {
        nil
    }

    // Not available on macOS: ARWorldMap and collaboration data are iOS-only APIs.
    // Remote ARKit sessions do not support world map persistence or collaboration.
    @Callable
    func get_current_world_map(callback: Callable) {
        _ = callback.call(nil, Variant("World map persistence is unavailable on macOS."))
    }

    @Callable
    func run_with_world_map(configuration: ARWorldTrackingConfiguration, worldMap: ARWorldMap) {
        session_failed.emit("World map persistence is unavailable on macOS.")
    }

    @Callable
    func update_with_collaboration_data(data: ARCollaborationData) {}

    @available(macOS 26.0, *)
    private func startMacTasks(session: ARKitSession, worldTrackingProvider: WorldTrackingProvider) async {
        updateTasks.append(Task { [weak self] in
            guard let self else { return }
            for await event in session.events {
                await self.handleMacSessionEvent(event)
            }
        })

        updateTasks.append(Task { [weak self] in
            guard let self else { return }
            while !Task.isCancelled {
                if let deviceAnchor = worldTrackingProvider.queryDeviceAnchor(atTimestamp: CACurrentMediaTime()) {
                    await self.publishMacFrame(deviceAnchor: deviceAnchor)
                }
                try? await Task.sleep(nanoseconds: 33_000_000)
            }
        })
    }

    @available(macOS 26.0, *)
    private func handleMacSessionEvent(_ event: ARKitSession.Event) async {
        switch event {
        case let .dataProviderStateChanged(_, _, error):
            if let error {
                await emitSessionFailedOnMainActor(error.localizedDescription)
            }
        @unknown default:
            break
        }
    }

    @available(macOS 26.0, *)
    private func publishMacFrame(deviceAnchor: DeviceAnchor) async {
        let camera = ARCamera(deviceAnchor: deviceAnchor)
        let timestamp = deviceAnchor.timestamp

        await MainActor.run {
            latestCamera = camera
            latestTimestamp = timestamp

            let frame = ARFrame(
                timestamp: timestamp,
                camera: camera,
                anchors: currentAnchors(),
                worldMappingStatus: camera.trackingState == .NORMAL ? .MAPPED : .LIMITED
            )
            camera_tracking_changed.emit(camera)
            frame_updated.emit(frame)
        }
    }

    @MainActor
    private func emitSessionFailedOnMainActor(_ message: String) {
        session_failed.emit(message)
    }

    private func currentAnchors() -> [RefCounted] {
        worldAnchorsByIdentifier.keys.sorted(by: { $0.uuidString < $1.uuidString }).compactMap {
            worldAnchorsByIdentifier[$0] as RefCounted?
        }
    }

    @available(macOS 26.0, *)
    private func resolveRemoteDeviceIdentifier() -> RemoteDeviceIdentifier? {
        if Thread.isMainThread {
            return MainActor.assumeIsolated {
                captureRemoteDeviceIdentifierOnMainThread()
            }
        }

        var result: RemoteDeviceIdentifier?
        DispatchQueue.main.sync {
            result = MainActor.assumeIsolated {
                captureRemoteDeviceIdentifierOnMainThread()
            }
        }
        return result
    }

    @available(macOS 26.0, *)
    @MainActor
    private func captureRemoteDeviceIdentifierOnMainThread() -> RemoteDeviceIdentifier? {
        var capturedIdentifier: RemoteDeviceIdentifier?
        let hostingView = NSHostingView(
            rootView: ARKitRemoteDeviceIdentifierReader {
                capturedIdentifier = $0
            }
        )

        hostingView.frame = CGRect(x: 0, y: 0, width: 1, height: 1)
        hostingView.isHidden = true

        guard let contentView = NSApp.keyWindow?.contentView ?? NSApp.mainWindow?.contentView else {
            return nil
        }

        contentView.addSubview(hostingView)
        contentView.layoutSubtreeIfNeeded()
        RunLoop.current.run(until: Date().addingTimeInterval(0.05))
        hostingView.removeFromSuperview()

        return capturedIdentifier
    }
}

#endif
