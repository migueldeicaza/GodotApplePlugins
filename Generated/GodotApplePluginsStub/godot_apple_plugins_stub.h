#ifndef GODOT_APPLE_PLUGINS_STUB_H
#define GODOT_APPLE_PLUGINS_STUB_H

#include "gdextension_interface.h"

#if defined(_WIN32)
#define GAP_STUB_EXPORT __declspec(dllexport)
#else
#define GAP_STUB_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

GAP_STUB_EXPORT GDExtensionBool godot_apple_plugins_stub_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization);

#ifdef __cplusplus
}
#endif

#endif