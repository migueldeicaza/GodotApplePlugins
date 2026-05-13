`SwiftGodotRuntime` payload for the split `GodotApplePlugins*` addons.

The split addon frameworks share the runtime from
`res://addons/GodotApplePluginsRuntime/bin`.

Keep this addon installed when using any split `GodotApplePlugins*` addon. The
`.gdextension` manifests declare it as a native dependency so Godot embeds it in
iOS exports and preloads it on macOS.
