# Using Apple's ARKit APIs with Godot

This is a guide on using the ARKit APIs in this Godot addon. For an
overview of what you can do with ARKit, check [Apple's ARKit
Documentation](https://developer.apple.com/documentation/arkit/).

As with the rest of GodotApplePlugins, the binding surfaces the same
class names that Apple uses for their own data types to simplify
looking things up and finding resources online. Method names follow
the Godot naming scheme (snake_case instead of camelCase).

# Table of Contents

* [Available Types](#available-types)
* [Platform Notes](#platform-notes)

# Available Types

The ARKit binding is documented in `doc_classes/` and in the published API reference. The surface includes:

## Core Session Types

* `ARSession` — manages the AR experience lifecycle
* `ARWorldTrackingConfiguration` — configures six-degrees-of-freedom world tracking
* `ARFrame` — a single frame of AR data including camera transforms and anchors
* `ARCamera` — camera position, orientation, and tracking state
* `ARLightEstimate` — ambient lighting information for the scene
* `ARPointCloud` — a collection of 3D points detected in the scene

## Anchors

* `ARAnchor` — base anchor representing a tracked position in 3D space
* `ARPlaneAnchor` — a detected horizontal or vertical surface
* `ARImageAnchor` — a detected known image in the environment
* `ARMeshAnchor` — a mesh geometry representing real-world surfaces
* `ARFaceAnchor` — a detected face with blend shapes and geometry
* `ARBodyAnchor` — a detected human body with skeletal data
* `ARHandAnchor` — a detected hand with joint positions
* `AREnvironmentProbeAnchor` — environment lighting information at a point
* `ARGeoAnchor` — an anchor placed at a geographic coordinate

## Session Helpers

* `ARRaycastQuery` — a query for raycasting against real-world geometry
* `ARRaycastResult` — the result of a raycast
* `ARTrackedRaycast` — a continuously updated raycast
* `ARWorldMap` — a serializable snapshot of the AR world state
* `ARCoachingOverlay` — Apple's built-in onboarding UI for AR
* `ARCollaborationData` — data for multi-user AR sessions

# Platform Notes

* **iOS** provides the broadest ARKit coverage.
* **visionOS** uses the provider-based `ARKitSession` backend.
* **macOS** uses the remote-device ARKit session workflow exposed by Apple.
