// swift-tools-version: 6.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let swiftSettings: [SwiftSetting] = [
    .unsafeFlags([
        "-Xfrontend", "-internalize-at-link",
        "-Xfrontend", "-lto=llvm-full",
        "-Xfrontend", "-conditional-runtime-records"
    ])
]

let linkerSettings: [LinkerSetting] = [
    .unsafeFlags(["-Xlinker", "-dead_strip"])
]

let runtimeDependency: Target.Dependency = .product(
    name: "SwiftGodotRuntime",
    package: "SwiftGodot"
)

func pluginTarget(name: String, path: String, exclude: [String] = []) -> Target {
    .target(
        name: name,
        dependencies: [runtimeDependency],
        path: path,
        exclude: exclude,
        swiftSettings: swiftSettings,
        linkerSettings: linkerSettings
    )
}

let package = Package(
    name: "GodotApplePlugins",
    platforms: [
        .iOS(.v17),
        .macOS("14.0"),
        .visionOS(.v1),
    ],
    products: [
        .library(
            name: "GodotApplePlugins",
            type: .dynamic,
            targets: ["GodotApplePlugins"]
        ),
        .library(
            name: "GodotApplePluginsAVFoundation",
            type: .dynamic,
            targets: ["GodotApplePluginsAVFoundation"]
        ),
        .library(
            name: "GodotApplePluginsFoundation",
            type: .dynamic,
            targets: ["GodotApplePluginsFoundation"]
        ),
        .library(
            name: "GodotApplePluginsGameCenter",
            type: .dynamic,
            targets: ["GodotApplePluginsGameCenter"]
        ),
        .library(
            name: "GodotApplePluginsStoreKit",
            type: .dynamic,
            targets: ["GodotApplePluginsStoreKit"]
        ),
        .library(
            name: "GodotApplePluginsAuthenticationServices",
            type: .dynamic,
            targets: ["GodotApplePluginsAuthenticationServices"]
        ),
        .library(
            name: "GodotApplePluginsARKit",
            type: .dynamic,
            targets: ["GodotApplePluginsARKit"]
        ),
        .library(
            name: "GodotApplePluginsCoreMotion",
            type: .dynamic,
            targets: ["GodotApplePluginsCoreMotion"]
        ),
        .executable(
            name: "GodotApplePluginsStubGenerator",
            targets: ["GodotApplePluginsStubGenerator"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/migueldeicaza/SwiftGodot", revision: "5cf54f881f158e04108946472a1b3be6acd21e7b")
        // For local development:
        //.package(path: "../SwiftGodot")
    ],
    targets: [
        .target(
            name: "GodotApplePlugins",
            dependencies: [
                runtimeDependency,
                "GodotApplePluginsAVFoundation",
                "GodotApplePluginsFoundation",
                "GodotApplePluginsGameCenter",
                "GodotApplePluginsStoreKit",
                "GodotApplePluginsAuthenticationServices",
                "GodotApplePluginsARKit",
                "GodotApplePluginsCoreMotion",
            ],
            path: "Sources/GodotApplePlugins",
            swiftSettings: swiftSettings,
            linkerSettings: linkerSettings
        ),
        pluginTarget(
            name: "GodotApplePluginsAVFoundation",
            path: "Sources/GodotAVFoundation"
        ),
        pluginTarget(
            name: "GodotApplePluginsFoundation",
            path: "Sources/GodotFoundation"
        ),
        pluginTarget(
            name: "GodotApplePluginsGameCenter",
            path: "Sources/GodotGameCenter",
            exclude: [
                "Entitlements.md",
                "GameCenterGuide.md",
            ]
        ),
        pluginTarget(
            name: "GodotApplePluginsStoreKit",
            path: "Sources/GodotStoreKit"
        ),
        pluginTarget(
            name: "GodotApplePluginsAuthenticationServices",
            path: "Sources/GodotAuthenticationServices",
            exclude: ["AuthenticationServicesGuide.md"]
        ),
        pluginTarget(
            name: "GodotApplePluginsARKit",
            path: "Sources/GodotARKit",
            exclude: ["ARKitGuide.md"]
        ),
        pluginTarget(
            name: "GodotApplePluginsCoreMotion",
            path: "Sources/GodotCoreMotion",
            exclude: ["CoreMotionGuide.md"]
        ),
        .executableTarget(
            name: "GodotApplePluginsStubGenerator"
        ),
    ]
)
