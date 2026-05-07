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

func pluginTarget(name: String, sources: [String], exclude: [String] = []) -> Target {
    .target(
        name: name,
        dependencies: [runtimeDependency],
        path: "Sources",
        exclude: exclude,
        sources: sources,
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
        .executable(
            name: "GodotApplePluginsStubGenerator",
            targets: ["GodotApplePluginsStubGenerator"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/migueldeicaza/SwiftGodot", revision: "ead7bffc9546c1740678a36096282e1a811b7da6")
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
            ],
            path: "Sources/GodotApplePluginsRegistration",
            sources: ["GodotApplePlugins.swift"],
            swiftSettings: swiftSettings,
            linkerSettings: linkerSettings
        ),
        pluginTarget(
            name: "GodotApplePluginsAVFoundation",
            sources: [
                "GodotApplePlugins/AVFoundation",
                "GodotApplePluginsRegistration/GodotApplePluginsAVFoundation.swift",
            ]
        ),
        pluginTarget(
            name: "GodotApplePluginsFoundation",
            sources: [
                "GodotApplePlugins/Foundation",
                "GodotApplePlugins/Shared",
                "GodotApplePlugins/UI",
                "GodotApplePluginsRegistration/GodotApplePluginsFoundation.swift",
            ]
        ),
        pluginTarget(
            name: "GodotApplePluginsGameCenter",
            sources: [
                "GodotApplePlugins/GameCenter",
                "GodotApplePluginsGameCenterSupport",
                "GodotApplePluginsRegistration/GodotApplePluginsGameCenter.swift",
            ],
            exclude: [
                "GodotApplePlugins/GameCenter/Entitlements.md",
                "GodotApplePlugins/GameCenter/GameCenterGuide.md",
            ]
        ),
        pluginTarget(
            name: "GodotApplePluginsStoreKit",
            sources: [
                "GodotApplePlugins/StoreKit",
                "GodotApplePluginsStoreKitSupport",
                "GodotApplePluginsRegistration/GodotApplePluginsStoreKit.swift",
            ]
        ),
        pluginTarget(
            name: "GodotApplePluginsAuthenticationServices",
            sources: [
                "GodotApplePlugins/AuthenticationServices",
                "GodotApplePluginsRegistration/GodotApplePluginsAuthenticationServices.swift",
            ],
            exclude: ["GodotApplePlugins/AuthenticationServices/AuthenticationServicesGuide.md"]
        ),
        pluginTarget(
            name: "GodotApplePluginsARKit",
            sources: [
                "GodotApplePlugins/ARKit",
                "GodotApplePluginsARKitSupport",
                "GodotApplePluginsRegistration/GodotApplePluginsARKit.swift",
            ],
            exclude: ["GodotApplePlugins/ARKit/ARKitGuide.md"]
        ),
        .executableTarget(
            name: "GodotApplePluginsStubGenerator"
        ),
    ]
)
