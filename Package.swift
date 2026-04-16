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
        .executable(
            name: "GodotApplePluginsStubGenerator",
            targets: ["GodotApplePluginsStubGenerator"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/migueldeicaza/SwiftGodot", revision: "e18d5d938deb716f128cb6686197dbf436dfd9bf")
        // For local development:
        //.package(path: "../SwiftGodot")
    ],
    targets: [
        .target(
            name: "GodotApplePlugins",
            dependencies: [
                .product(name: "SwiftGodotRuntimeStatic", package: "SwiftGodot")
            ],
            swiftSettings: swiftSettings,
            linkerSettings: linkerSettings
        ),
        .executableTarget(
            name: "GodotApplePluginsStubGenerator"
        ),
    ]
)
