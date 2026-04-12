#include "godot_apple_plugins_stub.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GAP_PROPERTY_HINT_NONE 0u
#define GAP_PROPERTY_USAGE_DEFAULT 6u
#define GAP_PROPERTY_USAGE_NIL_IS_VARIANT 131072u

typedef struct {
    uintptr_t opaque[3];
} GAPStubStringStorage;

typedef struct {
    uintptr_t opaque[3];
} GAPStubStringNameStorage;

typedef struct {
    GDExtensionVariantType variant_type;
    const char *class_name;
    uint32_t hint;
    const char *hint_string;
    uint32_t usage;
} GAPStubTypeInfo;

typedef struct {
    const char *name;
    GAPStubTypeInfo type;
} GAPStubNamedTypeInfo;

typedef struct {
    const char *name;
    const char *display_name;
    uint32_t flags;
    GDExtensionBool has_return_value;
    GAPStubTypeInfo return_type;
    const GAPStubNamedTypeInfo *arguments;
    uint32_t argument_count;
} GAPStubMethodDescriptor;

typedef struct {
    const char *name;
    GAPStubTypeInfo type;
    const char *setter;
    const char *getter;
} GAPStubPropertyDescriptor;

typedef struct {
    const char *name;
    const GAPStubNamedTypeInfo *arguments;
    uint32_t argument_count;
} GAPStubSignalDescriptor;

typedef struct {
    const char *enum_name;
    const char *name;
    int64_t value;
    GDExtensionBool is_bitfield;
} GAPStubConstantDescriptor;

typedef struct {
    const char *name;
    const char *parent_name;
    const GAPStubMethodDescriptor *methods;
    uint32_t method_count;
    const GAPStubPropertyDescriptor *properties;
    uint32_t property_count;
    const GAPStubSignalDescriptor *signals;
    uint32_t signal_count;
    const GAPStubConstantDescriptor *constants;
    uint32_t constant_count;
} GAPStubClassDescriptor;

typedef struct {
    GAPStubStringNameStorage name;
    GAPStubStringNameStorage class_name;
    GAPStubStringStorage hint_string;
} GAPStubPropertyInfoTemp;

typedef void (*GAPStubPersistentCleanup)(void *ptr, size_t count);

typedef struct GAPStubPersistentAllocation {
    void *ptr;
    size_t count;
    GAPStubPersistentCleanup cleanup;
    struct GAPStubPersistentAllocation *next;
} GAPStubPersistentAllocation;

typedef struct {
    GDExtensionInterfaceMemAlloc mem_alloc;
    GDExtensionInterfaceMemFree mem_free;
    GDExtensionInterfacePrintErrorWithMessage print_error_with_message;
    GDExtensionInterfaceStringNewWithUtf8Chars string_new_with_utf8_chars;
    GDExtensionInterfaceStringNameNewWithLatin1Chars string_name_new_with_latin1_chars;
    GDExtensionInterfaceVariantGetPtrDestructor variant_get_ptr_destructor;
    GDExtensionInterfaceVariantNewNil variant_new_nil;
    GDExtensionInterfaceClassdbRegisterExtensionClass2 classdb_register_extension_class2;
    GDExtensionInterfaceClassdbRegisterExtensionClassMethod classdb_register_extension_class_method;
    GDExtensionInterfaceClassdbRegisterExtensionClassProperty classdb_register_extension_class_property;
    GDExtensionInterfaceClassdbRegisterExtensionClassSignal classdb_register_extension_class_signal;
    GDExtensionInterfaceClassdbRegisterExtensionClassIntegerConstant classdb_register_extension_class_integer_constant;
    GDExtensionInterfaceClassdbUnregisterExtensionClass classdb_unregister_extension_class;
    GDExtensionPtrDestructor string_destructor;
    GDExtensionPtrDestructor string_name_destructor;
} GAPStubAPI;

static GAPStubAPI gap_api;
static GDExtensionClassLibraryPtr gap_library;
static GAPStubPersistentAllocation *gap_persistent_allocations;

static void gap_report_unimplemented(const char *callable_name) {
    char message[512];
    snprintf(
        message,
        sizeof(message),
        "GodotApplePlugins stub: %s is only available on Apple platforms. This generated library only preserves the documented API surface.",
        callable_name
    );
    gap_api.print_error_with_message("GodotApplePlugins stub", message, __func__, __FILE__, __LINE__, 1);
}

static void gap_string_init(GAPStubStringStorage *storage, const char *value) {
    gap_api.string_new_with_utf8_chars((GDExtensionUninitializedStringPtr)storage, value != NULL ? value : "");
}

static void gap_string_destroy(GAPStubStringStorage *storage) {
    if (gap_api.string_destructor != NULL) {
        gap_api.string_destructor((GDExtensionTypePtr)storage);
    }
}

static void gap_string_name_init(GAPStubStringNameStorage *storage, const char *value) {
    gap_api.string_name_new_with_latin1_chars((GDExtensionUninitializedStringNamePtr)storage, value != NULL ? value : "", 1);
}

static void gap_string_name_destroy(GAPStubStringNameStorage *storage) {
    if (gap_api.string_name_destructor != NULL) {
        gap_api.string_name_destructor((GDExtensionTypePtr)storage);
    }
}

static void gap_property_info_init(
    GAPStubPropertyInfoTemp *temp,
    GDExtensionPropertyInfo *property_info,
    const char *name,
    const GAPStubTypeInfo *type_info
) {
    gap_string_name_init(&temp->name, name);
    gap_string_name_init(&temp->class_name, type_info->class_name);
    gap_string_init(&temp->hint_string, type_info->hint_string);

    property_info->type = type_info->variant_type;
    property_info->name = (GDExtensionStringNamePtr)&temp->name;
    property_info->class_name = (GDExtensionStringNamePtr)&temp->class_name;
    property_info->hint = type_info->hint;
    property_info->hint_string = (GDExtensionStringPtr)&temp->hint_string;
    property_info->usage = type_info->usage;
}

static void gap_property_info_destroy(GAPStubPropertyInfoTemp *temp) {
    gap_string_destroy(&temp->hint_string);
    gap_string_name_destroy(&temp->class_name);
    gap_string_name_destroy(&temp->name);
}

static void gap_cleanup_string_name_storage(void *ptr, size_t count) {
    GAPStubStringNameStorage *items = (GAPStubStringNameStorage *)ptr;
    size_t index;

    for (index = 0; index < count; ++index) {
        gap_string_name_destroy(&items[index]);
    }
}

static void gap_cleanup_property_info_temp(void *ptr, size_t count) {
    GAPStubPropertyInfoTemp *items = (GAPStubPropertyInfoTemp *)ptr;
    size_t index;

    for (index = 0; index < count; ++index) {
        gap_property_info_destroy(&items[index]);
    }
}

static void *gap_persistent_alloc(size_t item_size, size_t count, GAPStubPersistentCleanup cleanup) {
    GAPStubPersistentAllocation *allocation;
    size_t total_size;

    if (count == 0 || item_size == 0) {
        return NULL;
    }

    allocation = (GAPStubPersistentAllocation *)gap_api.mem_alloc(sizeof(GAPStubPersistentAllocation));
    if (allocation == NULL) {
        return NULL;
    }

    total_size = item_size * count;
    allocation->ptr = gap_api.mem_alloc(total_size);
    if (allocation->ptr == NULL) {
        gap_api.mem_free(allocation);
        return NULL;
    }

    memset(allocation->ptr, 0, total_size);
    allocation->count = count;
    allocation->cleanup = cleanup;
    allocation->next = gap_persistent_allocations;
    gap_persistent_allocations = allocation;

    return allocation->ptr;
}

static void gap_release_persistent_allocations(void) {
    GAPStubPersistentAllocation *allocation = gap_persistent_allocations;

    while (allocation != NULL) {
        GAPStubPersistentAllocation *next = allocation->next;

        if (allocation->cleanup != NULL) {
            allocation->cleanup(allocation->ptr, allocation->count);
        }
        gap_api.mem_free(allocation->ptr);
        gap_api.mem_free(allocation);
        allocation = next;
    }

    gap_persistent_allocations = NULL;
}

static void gap_stub_method_call(
    void *method_userdata,
    GDExtensionClassInstancePtr p_instance,
    const GDExtensionConstVariantPtr *p_args,
    GDExtensionInt p_argument_count,
    GDExtensionVariantPtr r_return,
    GDExtensionCallError *r_error
) {
    const GAPStubMethodDescriptor *method = (const GAPStubMethodDescriptor *)method_userdata;
    (void)p_instance;
    (void)p_args;
    (void)p_argument_count;

    gap_report_unimplemented(method->display_name);

    if (r_return != NULL) {
        gap_api.variant_new_nil(r_return);
    }

    if (r_error != NULL) {
        r_error->error = GDEXTENSION_CALL_ERROR_INVALID_METHOD;
        r_error->argument = 0;
        r_error->expected = 0;
    }
}

static GDExtensionObjectPtr gap_stub_create_instance(void *class_userdata) {
    const GAPStubClassDescriptor *descriptor = (const GAPStubClassDescriptor *)class_userdata;
    gap_report_unimplemented(descriptor->name);
    return NULL;
}

static void gap_stub_free_instance(void *class_userdata, GDExtensionClassInstancePtr p_instance) {
    (void)class_userdata;
    (void)p_instance;
}

static const GAPStubNamedTypeInfo gap_class_0_method_1_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_0_method_2_args[] = {
    { "name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_0_methods[] = {
{
    "copy_anchor",
    "ARAnchor.copy_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "create",
    "ARAnchor.create",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_0_method_1_args,
    1
},
{
    "create_named",
    "ARAnchor.create_named",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_0_method_2_args,
    2
},
{
    "get_identifier",
    "ARAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_name",
    "ARAnchor.get_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_session_identifier",
    "ARAnchor.get_session_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_transform",
    "ARAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_0_properties[] = {
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_name"
},
{
    "session_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_session_identifier"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_transform"
},
};
static const GAPStubNamedTypeInfo gap_class_1_method_1_args[] = {
    { "estimated_scale_factor", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_method_3_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_method_5_args[] = {
    { "is_tracked", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_method_7_args[] = {
    { "skeleton", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARBodySkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_method_9_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_1_methods[] = {
{
    "get_estimated_scale_factor",
    "ARBodyAnchor.get_estimated_scale_factor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_estimated_scale_factor",
    "ARBodyAnchor.set_estimated_scale_factor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_1_method_1_args,
    1
},
{
    "get_identifier",
    "ARBodyAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARBodyAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_1_method_3_args,
    1
},
{
    "get_is_tracked",
    "ARBodyAnchor.get_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_tracked",
    "ARBodyAnchor.set_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_1_method_5_args,
    1
},
{
    "get_skeleton",
    "ARBodyAnchor.get_skeleton",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARBodySkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_skeleton",
    "ARBodyAnchor.set_skeleton",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_1_method_7_args,
    1
},
{
    "get_transform",
    "ARBodyAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARBodyAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_1_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_1_properties[] = {
{
    "estimated_scale_factor",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_estimated_scale_factor",
    "get_estimated_scale_factor"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "is_tracked",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_tracked",
    "get_is_tracked"
},
{
    "skeleton",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARBodySkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_skeleton",
    "get_skeleton"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
};
static const GAPStubNamedTypeInfo gap_class_2_method_0_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_2_method_1_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_2_method_2_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_2_method_4_args[] = {
    { "joint_count", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_2_methods[] = {
{
    "get_joint_is_tracked",
    "ARBodySkeleton.get_joint_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_2_method_0_args,
    1
},
{
    "get_joint_name",
    "ARBodySkeleton.get_joint_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_2_method_1_args,
    1
},
{
    "get_joint_transform",
    "ARBodySkeleton.get_joint_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_2_method_2_args,
    1
},
{
    "get_joint_count",
    "ARBodySkeleton.get_joint_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_joint_count",
    "ARBodySkeleton.set_joint_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_2_method_4_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_2_properties[] = {
{
    "joint_count",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_joint_count",
    "get_joint_count"
},
};
static const GAPStubNamedTypeInfo gap_class_3_method_2_args[] = {
    { "automatic_skeleton_scale_estimation_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_3_method_4_args[] = {
    { "is_auto_focus_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_3_method_6_args[] = {
    { "is_light_estimation_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_3_methods[] = {
{
    "is_supported",
    "ARBodyTrackingConfiguration.is_supported",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_automatic_skeleton_scale_estimation_enabled",
    "ARBodyTrackingConfiguration.get_automatic_skeleton_scale_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_automatic_skeleton_scale_estimation_enabled",
    "ARBodyTrackingConfiguration.set_automatic_skeleton_scale_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_3_method_2_args,
    1
},
{
    "get_is_auto_focus_enabled",
    "ARBodyTrackingConfiguration.get_is_auto_focus_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_auto_focus_enabled",
    "ARBodyTrackingConfiguration.set_is_auto_focus_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_3_method_4_args,
    1
},
{
    "get_is_light_estimation_enabled",
    "ARBodyTrackingConfiguration.get_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_light_estimation_enabled",
    "ARBodyTrackingConfiguration.set_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_3_method_6_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_3_properties[] = {
{
    "automatic_skeleton_scale_estimation_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_automatic_skeleton_scale_estimation_enabled",
    "get_automatic_skeleton_scale_estimation_enabled"
},
{
    "is_auto_focus_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_auto_focus_enabled",
    "get_is_auto_focus_enabled"
},
{
    "is_light_estimation_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_light_estimation_enabled",
    "get_is_light_estimation_enabled"
},
};
static const GAPStubNamedTypeInfo gap_class_4_method_0_args[] = {
    { "point", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "orientation", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "viewportSize", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_4_method_1_args[] = {
    { "orientation", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "viewportSize", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "zNear", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "zFar", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_4_method_2_args[] = {
    { "point", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "ontoPlaneWithTransform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "orientation", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "viewportSize", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_4_method_3_args[] = {
    { "orientation", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_4_methods[] = {
{
    "project_point",
    "ARCamera.project_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_4_method_0_args,
    3
},
{
    "projection_matrix_for_orientation",
    "ARCamera.projection_matrix_for_orientation",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_4_method_1_args,
    4
},
{
    "unproject_point",
    "ARCamera.unproject_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_4_method_2_args,
    4
},
{
    "view_matrix_for_orientation",
    "ARCamera.view_matrix_for_orientation",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_4_method_3_args,
    1
},
{
    "get_euler_angles",
    "ARCamera.get_euler_angles",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exposure_duration",
    "ARCamera.get_exposure_duration",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exposure_offset",
    "ARCamera.get_exposure_offset",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_image_resolution",
    "ARCamera.get_image_resolution",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_intrinsics",
    "ARCamera.get_intrinsics",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_projection_matrix",
    "ARCamera.get_projection_matrix",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_timestamp",
    "ARCamera.get_timestamp",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_tracking_state",
    "ARCamera.get_tracking_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_tracking_state_reason",
    "ARCamera.get_tracking_state_reason",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_transform",
    "ARCamera.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_4_properties[] = {
{
    "euler_angles",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_euler_angles"
},
{
    "exposure_duration",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exposure_duration"
},
{
    "exposure_offset",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exposure_offset"
},
{
    "image_resolution",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_image_resolution"
},
{
    "intrinsics",
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_intrinsics"
},
{
    "projection_matrix",
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_projection_matrix"
},
{
    "timestamp",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_timestamp"
},
{
    "tracking_state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_tracking_state"
},
{
    "tracking_state_reason",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_tracking_state_reason"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_transform"
},
};
static const GAPStubConstantDescriptor gap_class_4_constants[] = {
{ "TrackingState", "NOT_AVAILABLE", 0, 0 },
{ "TrackingState", "LIMITED", 1, 0 },
{ "TrackingState", "NORMAL", 2, 0 },
{ "TrackingStateReason", "NONE", 0, 0 },
{ "TrackingStateReason", "INITIALIZING", 1, 0 },
{ "TrackingStateReason", "EXCESSIVE_MOTION", 2, 0 },
{ "TrackingStateReason", "INSUFFICIENT_FEATURES", 3, 0 },
{ "TrackingStateReason", "RELOCALIZING", 4, 0 },
};
static const GAPStubNamedTypeInfo gap_class_5_method_0_args[] = {
    { "session", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARSession", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_5_method_1_args[] = {
    { "active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "animated", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_5_method_3_args[] = {
    { "activates_automatically", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_5_method_5_args[] = {
    { "goal", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_5_methods[] = {
{
    "attach_to_session",
    "ARCoachingOverlay.attach_to_session",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_5_method_0_args,
    1
},
{
    "set_active",
    "ARCoachingOverlay.set_active",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_5_method_1_args,
    2
},
{
    "get_activates_automatically",
    "ARCoachingOverlay.get_activates_automatically",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_activates_automatically",
    "ARCoachingOverlay.set_activates_automatically",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_5_method_3_args,
    1
},
{
    "get_goal",
    "ARCoachingOverlay.get_goal",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_goal",
    "ARCoachingOverlay.set_goal",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_5_method_5_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_5_properties[] = {
{
    "activates_automatically",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_activates_automatically",
    "get_activates_automatically"
},
{
    "goal",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_goal",
    "get_goal"
},
};
static const GAPStubSignalDescriptor gap_class_5_signals[] = {
{
    "coaching_did_deactivate",
    NULL,
    0
},
};
static const GAPStubConstantDescriptor gap_class_5_constants[] = {
{ "Goal", "TRACKING", 0, 0 },
{ "Goal", "HORIZONTAL_PLANE", 1, 0 },
{ "Goal", "VERTICAL_PLANE", 2, 0 },
{ "Goal", "ANY_PLANE", 3, 0 },
{ "Goal", "GEO_TRACKING", 4, 0 },
};
static const GAPStubNamedTypeInfo gap_class_6_method_0_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_6_method_3_args[] = {
    { "priority", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_6_methods[] = {
{
    "deserialize",
    "ARCollaborationData.deserialize",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCollaborationData", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_6_method_0_args,
    1
},
{
    "serialize",
    "ARCollaborationData.serialize",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_priority",
    "ARCollaborationData.get_priority",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_priority",
    "ARCollaborationData.set_priority",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_6_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_6_properties[] = {
{
    "priority",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_priority",
    "get_priority"
},
};
static const GAPStubConstantDescriptor gap_class_6_constants[] = {
{ "Priority", "CRITICAL", 0, 0 },
{ "Priority", "OPTIONAL", 1, 0 },
};
static const GAPStubNamedTypeInfo gap_class_7_method_1_args[] = {
    { "extent", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_7_method_3_args[] = {
    { "has_environment_texture", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_7_method_5_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_7_method_7_args[] = {
    { "texture_height", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_7_method_9_args[] = {
    { "texture_width", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_7_method_11_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_7_methods[] = {
{
    "get_extent",
    "AREnvironmentProbeAnchor.get_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_extent",
    "AREnvironmentProbeAnchor.set_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_1_args,
    1
},
{
    "get_has_environment_texture",
    "AREnvironmentProbeAnchor.get_has_environment_texture",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_has_environment_texture",
    "AREnvironmentProbeAnchor.set_has_environment_texture",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_3_args,
    1
},
{
    "get_identifier",
    "AREnvironmentProbeAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "AREnvironmentProbeAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_5_args,
    1
},
{
    "get_texture_height",
    "AREnvironmentProbeAnchor.get_texture_height",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_texture_height",
    "AREnvironmentProbeAnchor.set_texture_height",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_7_args,
    1
},
{
    "get_texture_width",
    "AREnvironmentProbeAnchor.get_texture_width",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_texture_width",
    "AREnvironmentProbeAnchor.set_texture_width",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_9_args,
    1
},
{
    "get_transform",
    "AREnvironmentProbeAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "AREnvironmentProbeAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_7_method_11_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_7_properties[] = {
{
    "extent",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_extent",
    "get_extent"
},
{
    "has_environment_texture",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_has_environment_texture",
    "get_has_environment_texture"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "texture_height",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_texture_height",
    "get_texture_height"
},
{
    "texture_width",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_texture_width",
    "get_texture_width"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
};
static const GAPStubNamedTypeInfo gap_class_8_method_0_args[] = {
    { "location", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_2_args[] = {
    { "geometry_texture_coordinates", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_4_args[] = {
    { "geometry_vertices", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_6_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_8_args[] = {
    { "is_tracked", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_10_args[] = {
    { "left_eye_transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_12_args[] = {
    { "look_at_point", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_14_args[] = {
    { "right_eye_transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_16_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_18_args[] = {
    { "triangle_indices", { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_8_methods[] = {
{
    "get_blend_shape_value",
    "ARFaceAnchor.get_blend_shape_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_0_args,
    1
},
{
    "get_geometry_texture_coordinates",
    "ARFaceAnchor.get_geometry_texture_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_geometry_texture_coordinates",
    "ARFaceAnchor.set_geometry_texture_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_2_args,
    1
},
{
    "get_geometry_vertices",
    "ARFaceAnchor.get_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_geometry_vertices",
    "ARFaceAnchor.set_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_4_args,
    1
},
{
    "get_identifier",
    "ARFaceAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARFaceAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_6_args,
    1
},
{
    "get_is_tracked",
    "ARFaceAnchor.get_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_tracked",
    "ARFaceAnchor.set_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_8_args,
    1
},
{
    "get_left_eye_transform",
    "ARFaceAnchor.get_left_eye_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_left_eye_transform",
    "ARFaceAnchor.set_left_eye_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_10_args,
    1
},
{
    "get_look_at_point",
    "ARFaceAnchor.get_look_at_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_look_at_point",
    "ARFaceAnchor.set_look_at_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_12_args,
    1
},
{
    "get_right_eye_transform",
    "ARFaceAnchor.get_right_eye_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_right_eye_transform",
    "ARFaceAnchor.set_right_eye_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_14_args,
    1
},
{
    "get_transform",
    "ARFaceAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARFaceAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_16_args,
    1
},
{
    "get_triangle_indices",
    "ARFaceAnchor.get_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_triangle_indices",
    "ARFaceAnchor.set_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_18_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_8_properties[] = {
{
    "geometry_texture_coordinates",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_geometry_texture_coordinates",
    "get_geometry_texture_coordinates"
},
{
    "geometry_vertices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_geometry_vertices",
    "get_geometry_vertices"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "is_tracked",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_tracked",
    "get_is_tracked"
},
{
    "left_eye_transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_left_eye_transform",
    "get_left_eye_transform"
},
{
    "look_at_point",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_look_at_point",
    "get_look_at_point"
},
{
    "right_eye_transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_right_eye_transform",
    "get_right_eye_transform"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
{
    "triangle_indices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_triangle_indices",
    "get_triangle_indices"
},
};
static const GAPStubConstantDescriptor gap_class_8_constants[] = {
{ "BlendShapeLocation", "EYE_BLINK_LEFT", 0, 0 },
{ "BlendShapeLocation", "EYE_LOOK_DOWN_LEFT", 1, 0 },
{ "BlendShapeLocation", "EYE_LOOK_IN_LEFT", 2, 0 },
{ "BlendShapeLocation", "EYE_LOOK_OUT_LEFT", 3, 0 },
{ "BlendShapeLocation", "EYE_LOOK_UP_LEFT", 4, 0 },
{ "BlendShapeLocation", "EYE_SQUINT_LEFT", 5, 0 },
{ "BlendShapeLocation", "EYE_WIDE_LEFT", 6, 0 },
{ "BlendShapeLocation", "EYE_BLINK_RIGHT", 7, 0 },
{ "BlendShapeLocation", "EYE_LOOK_DOWN_RIGHT", 8, 0 },
{ "BlendShapeLocation", "EYE_LOOK_IN_RIGHT", 9, 0 },
{ "BlendShapeLocation", "EYE_LOOK_OUT_RIGHT", 10, 0 },
{ "BlendShapeLocation", "EYE_LOOK_UP_RIGHT", 11, 0 },
{ "BlendShapeLocation", "EYE_SQUINT_RIGHT", 12, 0 },
{ "BlendShapeLocation", "EYE_WIDE_RIGHT", 13, 0 },
{ "BlendShapeLocation", "JAW_FORWARD", 14, 0 },
{ "BlendShapeLocation", "JAW_LEFT", 15, 0 },
{ "BlendShapeLocation", "JAW_RIGHT", 16, 0 },
{ "BlendShapeLocation", "JAW_OPEN", 17, 0 },
{ "BlendShapeLocation", "MOUTH_CLOSE", 18, 0 },
{ "BlendShapeLocation", "MOUTH_FUNNEL", 19, 0 },
{ "BlendShapeLocation", "MOUTH_PUCKER", 20, 0 },
{ "BlendShapeLocation", "MOUTH_LEFT", 21, 0 },
{ "BlendShapeLocation", "MOUTH_RIGHT", 22, 0 },
{ "BlendShapeLocation", "MOUTH_SMILE_LEFT", 23, 0 },
{ "BlendShapeLocation", "MOUTH_SMILE_RIGHT", 24, 0 },
{ "BlendShapeLocation", "MOUTH_FROWN_LEFT", 25, 0 },
{ "BlendShapeLocation", "MOUTH_FROWN_RIGHT", 26, 0 },
{ "BlendShapeLocation", "MOUTH_DIMPLE_LEFT", 27, 0 },
{ "BlendShapeLocation", "MOUTH_DIMPLE_RIGHT", 28, 0 },
{ "BlendShapeLocation", "MOUTH_STRETCH_LEFT", 29, 0 },
{ "BlendShapeLocation", "MOUTH_STRETCH_RIGHT", 30, 0 },
{ "BlendShapeLocation", "MOUTH_ROLL_LOWER", 31, 0 },
{ "BlendShapeLocation", "MOUTH_ROLL_UPPER", 32, 0 },
{ "BlendShapeLocation", "MOUTH_SHRUG_LOWER", 33, 0 },
{ "BlendShapeLocation", "MOUTH_SHRUG_UPPER", 34, 0 },
{ "BlendShapeLocation", "MOUTH_PRESS_LEFT", 35, 0 },
{ "BlendShapeLocation", "MOUTH_PRESS_RIGHT", 36, 0 },
{ "BlendShapeLocation", "MOUTH_LOWER_DOWN_LEFT", 37, 0 },
{ "BlendShapeLocation", "MOUTH_LOWER_DOWN_RIGHT", 38, 0 },
{ "BlendShapeLocation", "MOUTH_UPPER_UP_LEFT", 39, 0 },
{ "BlendShapeLocation", "MOUTH_UPPER_UP_RIGHT", 40, 0 },
{ "BlendShapeLocation", "BROW_DOWN_LEFT", 41, 0 },
{ "BlendShapeLocation", "BROW_DOWN_RIGHT", 42, 0 },
{ "BlendShapeLocation", "BROW_INNER_UP", 43, 0 },
{ "BlendShapeLocation", "BROW_OUTER_UP_LEFT", 44, 0 },
{ "BlendShapeLocation", "BROW_OUTER_UP_RIGHT", 45, 0 },
{ "BlendShapeLocation", "CHEEK_PUFF", 46, 0 },
{ "BlendShapeLocation", "CHEEK_SQUINT_LEFT", 47, 0 },
{ "BlendShapeLocation", "CHEEK_SQUINT_RIGHT", 48, 0 },
{ "BlendShapeLocation", "NOSE_SNEER_LEFT", 49, 0 },
{ "BlendShapeLocation", "NOSE_SNEER_RIGHT", 50, 0 },
{ "BlendShapeLocation", "TONGUE_OUT", 51, 0 },
};
static const GAPStubNamedTypeInfo gap_class_9_method_0_args[] = {
    { "orientation", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "viewportSize", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_1_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_3_args[] = {
    { "plane", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_4_args[] = {
    { "plane", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_5_args[] = {
    { "plane", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_9_methods[] = {
{
    "display_transform",
    "ARFrame.display_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_0_args,
    2
},
{
    "get_anchor",
    "ARFrame.get_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "RefCounted", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_1_args,
    1
},
{
    "get_captured_image_plane_count",
    "ARFrame.get_captured_image_plane_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_captured_image_plane_data",
    "ARFrame.get_captured_image_plane_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_3_args,
    1
},
{
    "get_captured_image_plane_size",
    "ARFrame.get_captured_image_plane_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_4_args,
    1
},
{
    "get_captured_image_plane_stride",
    "ARFrame.get_captured_image_plane_stride",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_5_args,
    1
},
{
    "get_estimated_depth_data",
    "ARFrame.get_estimated_depth_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_segmentation_buffer",
    "ARFrame.get_segmentation_buffer",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_anchors",
    "ARFrame.get_anchors",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_camera",
    "ARFrame.get_camera",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCamera", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_captured_image_size",
    "ARFrame.get_captured_image_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_estimated_depth_data_size",
    "ARFrame.get_estimated_depth_data_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_light_estimate",
    "ARFrame.get_light_estimate",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARLightEstimate", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_raw_feature_points",
    "ARFrame.get_raw_feature_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARPointCloud", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_segmentation_buffer_size",
    "ARFrame.get_segmentation_buffer_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_timestamp",
    "ARFrame.get_timestamp",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_world_mapping_status",
    "ARFrame.get_world_mapping_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_9_properties[] = {
{
    "anchors",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_anchors"
},
{
    "camera",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCamera", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_camera"
},
{
    "captured_image_size",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_captured_image_size"
},
{
    "estimated_depth_data_size",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_estimated_depth_data_size"
},
{
    "light_estimate",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARLightEstimate", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_light_estimate"
},
{
    "raw_feature_points",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARPointCloud", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_raw_feature_points"
},
{
    "segmentation_buffer_size",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_segmentation_buffer_size"
},
{
    "timestamp",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_timestamp"
},
{
    "world_mapping_status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_world_mapping_status"
},
};
static const GAPStubConstantDescriptor gap_class_9_constants[] = {
{ "WorldMappingStatus", "NOT_AVAILABLE", 0, 0 },
{ "WorldMappingStatus", "LIMITED", 1, 0 },
{ "WorldMappingStatus", "EXTENDING", 2, 0 },
{ "WorldMappingStatus", "MAPPED", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_10_method_0_args[] = {
    { "latitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "longitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "altitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_1_args[] = {
    { "latitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "longitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_3_args[] = {
    { "altitude", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_5_args[] = {
    { "altitude_source", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_7_args[] = {
    { "coordinate", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_9_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_10_method_11_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_10_methods[] = {
{
    "create",
    "ARGeoAnchor.create",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARGeoAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_0_args,
    3
},
{
    "create_at_coordinate",
    "ARGeoAnchor.create_at_coordinate",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARGeoAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_1_args,
    2
},
{
    "get_altitude",
    "ARGeoAnchor.get_altitude",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_altitude",
    "ARGeoAnchor.set_altitude",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_3_args,
    1
},
{
    "get_altitude_source",
    "ARGeoAnchor.get_altitude_source",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_altitude_source",
    "ARGeoAnchor.set_altitude_source",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_5_args,
    1
},
{
    "get_coordinate",
    "ARGeoAnchor.get_coordinate",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_coordinate",
    "ARGeoAnchor.set_coordinate",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_7_args,
    1
},
{
    "get_identifier",
    "ARGeoAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARGeoAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_9_args,
    1
},
{
    "get_transform",
    "ARGeoAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARGeoAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_10_method_11_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_10_properties[] = {
{
    "altitude",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_altitude",
    "get_altitude"
},
{
    "altitude_source",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_altitude_source",
    "get_altitude_source"
},
{
    "coordinate",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_coordinate",
    "get_coordinate"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
};
static const GAPStubConstantDescriptor gap_class_10_constants[] = {
{ "AltitudeSource", "UNKNOWN", 0, 0 },
{ "AltitudeSource", "COARSE", 1, 0 },
{ "AltitudeSource", "PRECISE", 2, 0 },
{ "AltitudeSource", "USER_DEFINED", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_11_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_2_args[] = {
    { "groupName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_4_args[] = {
    { "detection_image_group_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_6_args[] = {
    { "environment_texturing", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_8_args[] = {
    { "is_light_estimation_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_10_args[] = {
    { "plane_detection_mask", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_11_method_12_args[] = {
    { "wants_hdr_environment_textures", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_11_methods[] = {
{
    "check_availability",
    "ARGeoTrackingConfiguration.check_availability",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_0_args,
    1
},
{
    "is_supported",
    "ARGeoTrackingConfiguration.is_supported",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_detection_image_group",
    "ARGeoTrackingConfiguration.set_detection_image_group",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_2_args,
    1
},
{
    "get_detection_image_group_name",
    "ARGeoTrackingConfiguration.get_detection_image_group_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_detection_image_group_name",
    "ARGeoTrackingConfiguration.set_detection_image_group_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_4_args,
    1
},
{
    "get_environment_texturing",
    "ARGeoTrackingConfiguration.get_environment_texturing",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_environment_texturing",
    "ARGeoTrackingConfiguration.set_environment_texturing",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_6_args,
    1
},
{
    "get_is_light_estimation_enabled",
    "ARGeoTrackingConfiguration.get_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_light_estimation_enabled",
    "ARGeoTrackingConfiguration.set_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_8_args,
    1
},
{
    "get_plane_detection_mask",
    "ARGeoTrackingConfiguration.get_plane_detection_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_plane_detection_mask",
    "ARGeoTrackingConfiguration.set_plane_detection_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_10_args,
    1
},
{
    "get_wants_hdr_environment_textures",
    "ARGeoTrackingConfiguration.get_wants_hdr_environment_textures",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_wants_hdr_environment_textures",
    "ARGeoTrackingConfiguration.set_wants_hdr_environment_textures",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_11_method_12_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_11_properties[] = {
{
    "detection_image_group_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_detection_image_group_name",
    "get_detection_image_group_name"
},
{
    "environment_texturing",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_environment_texturing",
    "get_environment_texturing"
},
{
    "is_light_estimation_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_light_estimation_enabled",
    "get_is_light_estimation_enabled"
},
{
    "plane_detection_mask",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_plane_detection_mask",
    "get_plane_detection_mask"
},
{
    "wants_hdr_environment_textures",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_wants_hdr_environment_textures",
    "get_wants_hdr_environment_textures"
},
};
static const GAPStubNamedTypeInfo gap_class_12_method_1_args[] = {
    { "chirality", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_12_method_3_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_12_method_5_args[] = {
    { "is_tracked", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_12_method_7_args[] = {
    { "skeleton", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARHandSkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_12_method_9_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_12_methods[] = {
{
    "get_chirality",
    "ARHandAnchor.get_chirality",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_chirality",
    "ARHandAnchor.set_chirality",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_1_args,
    1
},
{
    "get_identifier",
    "ARHandAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARHandAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_3_args,
    1
},
{
    "get_is_tracked",
    "ARHandAnchor.get_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_tracked",
    "ARHandAnchor.set_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_5_args,
    1
},
{
    "get_skeleton",
    "ARHandAnchor.get_skeleton",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARHandSkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_skeleton",
    "ARHandAnchor.set_skeleton",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_7_args,
    1
},
{
    "get_transform",
    "ARHandAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARHandAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_12_properties[] = {
{
    "chirality",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_chirality",
    "get_chirality"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "is_tracked",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_tracked",
    "get_is_tracked"
},
{
    "skeleton",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARHandSkeleton", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_skeleton",
    "get_skeleton"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
};
static const GAPStubConstantDescriptor gap_class_12_constants[] = {
{ "Chirality", "LEFT", 0, 0 },
{ "Chirality", "RIGHT", 1, 0 },
};
static const GAPStubNamedTypeInfo gap_class_13_method_1_args[] = {
    { "joint", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_13_method_2_args[] = {
    { "joint", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_13_method_4_args[] = {
    { "joint_count", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_13_methods[] = {
{
    "get_all_joint_transforms",
    "ARHandSkeleton.get_all_joint_transforms",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_joint_is_tracked",
    "ARHandSkeleton.get_joint_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_1_args,
    1
},
{
    "get_joint_transform",
    "ARHandSkeleton.get_joint_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_2_args,
    1
},
{
    "get_joint_count",
    "ARHandSkeleton.get_joint_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_joint_count",
    "ARHandSkeleton.set_joint_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_4_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_13_properties[] = {
{
    "joint_count",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_joint_count",
    "get_joint_count"
},
};
static const GAPStubConstantDescriptor gap_class_13_constants[] = {
{ "JointName", "WRIST", 0, 0 },
{ "JointName", "THUMB_KNUCKLE", 1, 0 },
{ "JointName", "THUMB_INTERMEDIATE_BASE", 2, 0 },
{ "JointName", "THUMB_INTERMEDIATE_TIP", 3, 0 },
{ "JointName", "THUMB_TIP", 4, 0 },
{ "JointName", "INDEX_FINGER_METACARPAL", 5, 0 },
{ "JointName", "INDEX_FINGER_KNUCKLE", 6, 0 },
{ "JointName", "INDEX_FINGER_INTERMEDIATE_BASE", 7, 0 },
{ "JointName", "INDEX_FINGER_INTERMEDIATE_TIP", 8, 0 },
{ "JointName", "INDEX_FINGER_TIP", 9, 0 },
{ "JointName", "MIDDLE_FINGER_METACARPAL", 10, 0 },
{ "JointName", "MIDDLE_FINGER_KNUCKLE", 11, 0 },
{ "JointName", "MIDDLE_FINGER_INTERMEDIATE_BASE", 12, 0 },
{ "JointName", "MIDDLE_FINGER_INTERMEDIATE_TIP", 13, 0 },
{ "JointName", "MIDDLE_FINGER_TIP", 14, 0 },
{ "JointName", "RING_FINGER_METACARPAL", 15, 0 },
{ "JointName", "RING_FINGER_KNUCKLE", 16, 0 },
{ "JointName", "RING_FINGER_INTERMEDIATE_BASE", 17, 0 },
{ "JointName", "RING_FINGER_INTERMEDIATE_TIP", 18, 0 },
{ "JointName", "RING_FINGER_TIP", 19, 0 },
{ "JointName", "LITTLE_FINGER_METACARPAL", 20, 0 },
{ "JointName", "LITTLE_FINGER_KNUCKLE", 21, 0 },
{ "JointName", "LITTLE_FINGER_INTERMEDIATE_BASE", 22, 0 },
{ "JointName", "LITTLE_FINGER_INTERMEDIATE_TIP", 23, 0 },
{ "JointName", "LITTLE_FINGER_TIP", 24, 0 },
{ "JointName", "FOREARM_WRIST", 25, 0 },
{ "JointName", "FOREARM_ARM", 26, 0 },
};
static const GAPStubNamedTypeInfo gap_class_14_method_1_args[] = {
    { "estimated_scale_factor", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_3_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_5_args[] = {
    { "is_tracked", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_7_args[] = {
    { "reference_image_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_9_args[] = {
    { "reference_image_physical_size", { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_11_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_14_methods[] = {
{
    "get_estimated_scale_factor",
    "ARImageAnchor.get_estimated_scale_factor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_estimated_scale_factor",
    "ARImageAnchor.set_estimated_scale_factor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_1_args,
    1
},
{
    "get_identifier",
    "ARImageAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARImageAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_3_args,
    1
},
{
    "get_is_tracked",
    "ARImageAnchor.get_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_tracked",
    "ARImageAnchor.set_is_tracked",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_5_args,
    1
},
{
    "get_reference_image_name",
    "ARImageAnchor.get_reference_image_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_reference_image_name",
    "ARImageAnchor.set_reference_image_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_7_args,
    1
},
{
    "get_reference_image_physical_size",
    "ARImageAnchor.get_reference_image_physical_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_reference_image_physical_size",
    "ARImageAnchor.set_reference_image_physical_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_9_args,
    1
},
{
    "get_transform",
    "ARImageAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARImageAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_14_method_11_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_14_properties[] = {
{
    "estimated_scale_factor",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_estimated_scale_factor",
    "get_estimated_scale_factor"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "is_tracked",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_tracked",
    "get_is_tracked"
},
{
    "reference_image_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_reference_image_name",
    "get_reference_image_name"
},
{
    "reference_image_physical_size",
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_reference_image_physical_size",
    "get_reference_image_physical_size"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
};
static const GAPStubNamedTypeInfo gap_class_15_method_2_args[] = {
    { "ambient_color_temperature", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_4_args[] = {
    { "ambient_intensity", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_6_args[] = {
    { "primary_light_direction", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_8_args[] = {
    { "primary_light_intensity", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_10_args[] = {
    { "spherical_harmonics_coefficients", { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_15_methods[] = {
{
    "is_directional",
    "ARLightEstimate.is_directional",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_ambient_color_temperature",
    "ARLightEstimate.get_ambient_color_temperature",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_ambient_color_temperature",
    "ARLightEstimate.set_ambient_color_temperature",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_2_args,
    1
},
{
    "get_ambient_intensity",
    "ARLightEstimate.get_ambient_intensity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_ambient_intensity",
    "ARLightEstimate.set_ambient_intensity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_4_args,
    1
},
{
    "get_primary_light_direction",
    "ARLightEstimate.get_primary_light_direction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_primary_light_direction",
    "ARLightEstimate.set_primary_light_direction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_6_args,
    1
},
{
    "get_primary_light_intensity",
    "ARLightEstimate.get_primary_light_intensity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_primary_light_intensity",
    "ARLightEstimate.set_primary_light_intensity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_8_args,
    1
},
{
    "get_spherical_harmonics_coefficients",
    "ARLightEstimate.get_spherical_harmonics_coefficients",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_spherical_harmonics_coefficients",
    "ARLightEstimate.set_spherical_harmonics_coefficients",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_10_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_15_properties[] = {
{
    "ambient_color_temperature",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_ambient_color_temperature",
    "get_ambient_color_temperature"
},
{
    "ambient_intensity",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_ambient_intensity",
    "get_ambient_intensity"
},
{
    "primary_light_direction",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_primary_light_direction",
    "get_primary_light_direction"
},
{
    "primary_light_intensity",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_primary_light_intensity",
    "get_primary_light_intensity"
},
{
    "spherical_harmonics_coefficients",
    { GDEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_spherical_harmonics_coefficients",
    "get_spherical_harmonics_coefficients"
},
};
static const GAPStubNamedTypeInfo gap_class_16_method_0_args[] = {
    { "faceIndex", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_1_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_2_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_4_args[] = {
    { "classification_per_face", { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_6_args[] = {
    { "geometry_normals", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_8_args[] = {
    { "geometry_vertices", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_10_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_12_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_14_args[] = {
    { "triangle_indices", { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_16_methods[] = {
{
    "get_face_classification",
    "ARMeshAnchor.get_face_classification",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_0_args,
    1
},
{
    "get_normal",
    "ARMeshAnchor.get_normal",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_1_args,
    1
},
{
    "get_vertex",
    "ARMeshAnchor.get_vertex",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_2_args,
    1
},
{
    "get_classification_per_face",
    "ARMeshAnchor.get_classification_per_face",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_classification_per_face",
    "ARMeshAnchor.set_classification_per_face",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_4_args,
    1
},
{
    "get_geometry_normals",
    "ARMeshAnchor.get_geometry_normals",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_geometry_normals",
    "ARMeshAnchor.set_geometry_normals",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_6_args,
    1
},
{
    "get_geometry_vertices",
    "ARMeshAnchor.get_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_geometry_vertices",
    "ARMeshAnchor.set_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_8_args,
    1
},
{
    "get_identifier",
    "ARMeshAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARMeshAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_10_args,
    1
},
{
    "get_transform",
    "ARMeshAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARMeshAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_12_args,
    1
},
{
    "get_triangle_indices",
    "ARMeshAnchor.get_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_triangle_indices",
    "ARMeshAnchor.set_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_14_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_16_properties[] = {
{
    "classification_per_face",
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_classification_per_face",
    "get_classification_per_face"
},
{
    "geometry_normals",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_geometry_normals",
    "get_geometry_normals"
},
{
    "geometry_vertices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_geometry_vertices",
    "get_geometry_vertices"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
{
    "triangle_indices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_triangle_indices",
    "get_triangle_indices"
},
};
static const GAPStubConstantDescriptor gap_class_16_constants[] = {
{ "MeshClassification", "NONE", 0, 0 },
{ "MeshClassification", "WALL", 1, 0 },
{ "MeshClassification", "FLOOR", 2, 0 },
{ "MeshClassification", "CEILING", 3, 0 },
{ "MeshClassification", "TABLE", 4, 0 },
{ "MeshClassification", "SEAT", 5, 0 },
{ "MeshClassification", "WINDOW", 6, 0 },
{ "MeshClassification", "DOOR", 7, 0 },
};
static const GAPStubNamedTypeInfo gap_class_17_method_0_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_1_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_2_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_3_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_6_args[] = {
    { "alignment", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_8_args[] = {
    { "boundary_vertices", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_10_args[] = {
    { "center", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_12_args[] = {
    { "classification", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_14_args[] = {
    { "classification_status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_16_args[] = {
    { "extent", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_18_args[] = {
    { "geometry_vertices", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_20_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_22_args[] = {
    { "plane_extent_height", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_24_args[] = {
    { "plane_extent_rotation_on_y_axis", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_26_args[] = {
    { "plane_extent_width", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_28_args[] = {
    { "texture_coordinates", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_30_args[] = {
    { "transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_32_args[] = {
    { "triangle_indices", { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_17_methods[] = {
{
    "get_boundary_vertex",
    "ARPlaneAnchor.get_boundary_vertex",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_0_args,
    1
},
{
    "get_geometry_vertex",
    "ARPlaneAnchor.get_geometry_vertex",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_1_args,
    1
},
{
    "get_texture_coordinate",
    "ARPlaneAnchor.get_texture_coordinate",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_2_args,
    1
},
{
    "get_triangle_index",
    "ARPlaneAnchor.get_triangle_index",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_3_args,
    1
},
{
    "is_classification_supported",
    "ARPlaneAnchor.is_classification_supported",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_alignment",
    "ARPlaneAnchor.get_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_alignment",
    "ARPlaneAnchor.set_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_6_args,
    1
},
{
    "get_boundary_vertices",
    "ARPlaneAnchor.get_boundary_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_boundary_vertices",
    "ARPlaneAnchor.set_boundary_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_8_args,
    1
},
{
    "get_center",
    "ARPlaneAnchor.get_center",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_center",
    "ARPlaneAnchor.set_center",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_10_args,
    1
},
{
    "get_classification",
    "ARPlaneAnchor.get_classification",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_classification",
    "ARPlaneAnchor.set_classification",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_12_args,
    1
},
{
    "get_classification_status",
    "ARPlaneAnchor.get_classification_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_classification_status",
    "ARPlaneAnchor.set_classification_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_14_args,
    1
},
{
    "get_extent",
    "ARPlaneAnchor.get_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_extent",
    "ARPlaneAnchor.set_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_16_args,
    1
},
{
    "get_geometry_vertices",
    "ARPlaneAnchor.get_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_geometry_vertices",
    "ARPlaneAnchor.set_geometry_vertices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_18_args,
    1
},
{
    "get_identifier",
    "ARPlaneAnchor.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "ARPlaneAnchor.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_20_args,
    1
},
{
    "get_plane_extent_height",
    "ARPlaneAnchor.get_plane_extent_height",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_plane_extent_height",
    "ARPlaneAnchor.set_plane_extent_height",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_22_args,
    1
},
{
    "get_plane_extent_rotation_on_y_axis",
    "ARPlaneAnchor.get_plane_extent_rotation_on_y_axis",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_plane_extent_rotation_on_y_axis",
    "ARPlaneAnchor.set_plane_extent_rotation_on_y_axis",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_24_args,
    1
},
{
    "get_plane_extent_width",
    "ARPlaneAnchor.get_plane_extent_width",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_plane_extent_width",
    "ARPlaneAnchor.set_plane_extent_width",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_26_args,
    1
},
{
    "get_texture_coordinates",
    "ARPlaneAnchor.get_texture_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_texture_coordinates",
    "ARPlaneAnchor.set_texture_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_28_args,
    1
},
{
    "get_transform",
    "ARPlaneAnchor.get_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_transform",
    "ARPlaneAnchor.set_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_30_args,
    1
},
{
    "get_triangle_indices",
    "ARPlaneAnchor.get_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_triangle_indices",
    "ARPlaneAnchor.set_triangle_indices",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_32_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_17_properties[] = {
{
    "alignment",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_alignment",
    "get_alignment"
},
{
    "boundary_vertices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_boundary_vertices",
    "get_boundary_vertices"
},
{
    "center",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_center",
    "get_center"
},
{
    "classification",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_classification",
    "get_classification"
},
{
    "classification_status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_classification_status",
    "get_classification_status"
},
{
    "extent",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_extent",
    "get_extent"
},
{
    "geometry_vertices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_geometry_vertices",
    "get_geometry_vertices"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "plane_extent_height",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_plane_extent_height",
    "get_plane_extent_height"
},
{
    "plane_extent_rotation_on_y_axis",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_plane_extent_rotation_on_y_axis",
    "get_plane_extent_rotation_on_y_axis"
},
{
    "plane_extent_width",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_plane_extent_width",
    "get_plane_extent_width"
},
{
    "texture_coordinates",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_texture_coordinates",
    "get_texture_coordinates"
},
{
    "transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_transform",
    "get_transform"
},
{
    "triangle_indices",
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_triangle_indices",
    "get_triangle_indices"
},
};
static const GAPStubConstantDescriptor gap_class_17_constants[] = {
{ "Alignment", "HORIZONTAL", 0, 0 },
{ "Alignment", "VERTICAL", 1, 0 },
{ "Alignment", "SLANTED", 2, 0 },
{ "ClassificationStatus", "NOT_AVAILABLE", 0, 0 },
{ "ClassificationStatus", "UNDETERMINED", 1, 0 },
{ "ClassificationStatus", "UNKNOWN", 2, 0 },
{ "ClassificationStatus", "KNOWN", 3, 0 },
{ "Classification", "NONE", 0, 0 },
{ "Classification", "WALL", 1, 0 },
{ "Classification", "FLOOR", 2, 0 },
{ "Classification", "CEILING", 3, 0 },
{ "Classification", "TABLE", 4, 0 },
{ "Classification", "SEAT", 5, 0 },
{ "Classification", "WINDOW", 6, 0 },
{ "Classification", "DOOR", 7, 0 },
{ "Classification", "STAIRS", 8, 0 },
{ "Classification", "BED", 9, 0 },
{ "Classification", "CABINET", 10, 0 },
{ "Classification", "HOME_APPLIANCE", 11, 0 },
{ "Classification", "TV", 12, 0 },
{ "Classification", "PLANT", 13, 0 },
};
static const GAPStubNamedTypeInfo gap_class_18_method_0_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_18_method_1_args[] = {
    { "index", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_18_method_3_args[] = {
    { "count", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_18_method_5_args[] = {
    { "identifiers", { GDEXTENSION_VARIANT_TYPE_PACKED_INT64_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_18_method_7_args[] = {
    { "points", { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_18_methods[] = {
{
    "get_identifier",
    "ARPointCloud.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_18_method_0_args,
    1
},
{
    "get_point",
    "ARPointCloud.get_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_18_method_1_args,
    1
},
{
    "get_count",
    "ARPointCloud.get_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_count",
    "ARPointCloud.set_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_18_method_3_args,
    1
},
{
    "get_identifiers",
    "ARPointCloud.get_identifiers",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT64_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifiers",
    "ARPointCloud.set_identifiers",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_18_method_5_args,
    1
},
{
    "get_points",
    "ARPointCloud.get_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_points",
    "ARPointCloud.set_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_18_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_18_properties[] = {
{
    "count",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_count",
    "get_count"
},
{
    "identifiers",
    { GDEXTENSION_VARIANT_TYPE_PACKED_INT64_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifiers",
    "get_identifiers"
},
{
    "points",
    { GDEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_points",
    "get_points"
},
};
static const GAPStubNamedTypeInfo gap_class_19_method_0_args[] = {
    { "origin", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "direction", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "target", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "targetAlignment", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_2_args[] = {
    { "direction", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_4_args[] = {
    { "origin", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_6_args[] = {
    { "target", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_8_args[] = {
    { "target_alignment", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_19_methods[] = {
{
    "create",
    "ARRaycastQuery.create",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARRaycastQuery", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_0_args,
    4
},
{
    "get_direction",
    "ARRaycastQuery.get_direction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_direction",
    "ARRaycastQuery.set_direction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_2_args,
    1
},
{
    "get_origin",
    "ARRaycastQuery.get_origin",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_origin",
    "ARRaycastQuery.set_origin",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_4_args,
    1
},
{
    "get_target",
    "ARRaycastQuery.get_target",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_target",
    "ARRaycastQuery.set_target",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_6_args,
    1
},
{
    "get_target_alignment",
    "ARRaycastQuery.get_target_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_target_alignment",
    "ARRaycastQuery.set_target_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_8_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_19_properties[] = {
{
    "direction",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_direction",
    "get_direction"
},
{
    "origin",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_origin",
    "get_origin"
},
{
    "target",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_target",
    "get_target"
},
{
    "target_alignment",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_target_alignment",
    "get_target_alignment"
},
};
static const GAPStubConstantDescriptor gap_class_19_constants[] = {
{ "Target", "EXISTING_PLANE_GEOMETRY", 1, 0 },
{ "Target", "EXISTING_PLANE_INFINITE", 2, 0 },
{ "Target", "ESTIMATED_PLANE", 3, 0 },
{ "TargetAlignment", "HORIZONTAL", 0, 0 },
{ "TargetAlignment", "VERTICAL", 1, 0 },
{ "TargetAlignment", "ANY", 2, 0 },
};
static const GAPStubNamedTypeInfo gap_class_20_method_1_args[] = {
    { "anchor", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_20_method_3_args[] = {
    { "target", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_20_method_5_args[] = {
    { "target_alignment", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_20_method_7_args[] = {
    { "world_transform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_20_methods[] = {
{
    "get_anchor",
    "ARRaycastResult.get_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_anchor",
    "ARRaycastResult.set_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_20_method_1_args,
    1
},
{
    "get_target",
    "ARRaycastResult.get_target",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_target",
    "ARRaycastResult.set_target",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_20_method_3_args,
    1
},
{
    "get_target_alignment",
    "ARRaycastResult.get_target_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_target_alignment",
    "ARRaycastResult.set_target_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_20_method_5_args,
    1
},
{
    "get_world_transform",
    "ARRaycastResult.get_world_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_world_transform",
    "ARRaycastResult.set_world_transform",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_20_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_20_properties[] = {
{
    "anchor",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_anchor",
    "get_anchor"
},
{
    "target",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_target",
    "get_target"
},
{
    "target_alignment",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_target_alignment",
    "get_target_alignment"
},
{
    "world_transform",
    { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_world_transform",
    "get_world_transform"
},
};
static const GAPStubConstantDescriptor gap_class_20_constants[] = {
{ "Target", "EXISTING_PLANE_GEOMETRY", 1, 0 },
{ "Target", "EXISTING_PLANE_INFINITE", 2, 0 },
{ "Target", "ESTIMATED_PLANE", 3, 0 },
{ "TargetAlignment", "HORIZONTAL", 0, 0 },
{ "TargetAlignment", "VERTICAL", 1, 0 },
{ "TargetAlignment", "ANY", 2, 0 },
};
static const GAPStubNamedTypeInfo gap_class_21_method_0_args[] = {
    { "anchor", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_5_args[] = {
    { "query", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARRaycastQuery", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_7_args[] = {
    { "anchor", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_8_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_9_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARBodyTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_10_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARGeoTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_11_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARFaceTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_12_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARImageTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_13_args[] = {
    { "configuration", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "worldMap", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldMap", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_14_args[] = {
    { "relativeTransform", { GDEXTENSION_VARIANT_TYPE_TRANSFORM3D, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_15_args[] = {
    { "query", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARRaycastQuery", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_16_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCollaborationData", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_0_args[] = {
    { "anchors", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_1_args[] = {
    { "anchors", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_2_args[] = {
    { "anchors", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_3_args[] = {
    { "camera", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCamera", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_4_args[] = {
    { "collaborationData", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARCollaborationData", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_5_args[] = {
    { "frame", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARFrame", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_6_args[] = {
    { "hand", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARHandAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_8_args[] = {
    { "mesh", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARMeshAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_9_args[] = {
    { "mesh", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARMeshAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_10_args[] = {
    { "mesh", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARMeshAnchor", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_signal_11_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_21_methods[] = {
{
    "add_anchor",
    "ARSession.add_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_0_args,
    1
},
{
    "capture_high_resolution_frame",
    "ARSession.capture_high_resolution_frame",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_1_args,
    1
},
{
    "get_current_world_map",
    "ARSession.get_current_world_map",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_2_args,
    1
},
{
    "is_supported",
    "ARSession.is_supported",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "pause",
    "ARSession.pause",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "raycast",
    "ARSession.raycast",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_5_args,
    1
},
{
    "remove_all_anchors",
    "ARSession.remove_all_anchors",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "remove_anchor",
    "ARSession.remove_anchor",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_7_args,
    1
},
{
    "run",
    "ARSession.run",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_8_args,
    2
},
{
    "run_body_tracking",
    "ARSession.run_body_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_9_args,
    2
},
{
    "run_geo_tracking",
    "ARSession.run_geo_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_10_args,
    2
},
{
    "run_face_tracking",
    "ARSession.run_face_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_11_args,
    2
},
{
    "run_image_tracking",
    "ARSession.run_image_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_12_args,
    2
},
{
    "run_with_world_map",
    "ARSession.run_with_world_map",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_13_args,
    2
},
{
    "set_world_origin",
    "ARSession.set_world_origin",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_14_args,
    1
},
{
    "tracked_raycast",
    "ARSession.tracked_raycast",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARTrackedRaycast", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_15_args,
    2
},
{
    "update_with_collaboration_data",
    "ARSession.update_with_collaboration_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_21_method_16_args,
    1
},
{
    "get_anchors",
    "ARSession.get_anchors",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_configuration",
    "ARSession.get_configuration",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_current_frame",
    "ARSession.get_current_frame",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARFrame", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "ARSession.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_21_properties[] = {
{
    "anchors",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_anchors"
},
{
    "configuration",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldTrackingConfiguration", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_configuration"
},
{
    "current_frame",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARFrame", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_current_frame"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
};
static const GAPStubSignalDescriptor gap_class_21_signals[] = {
{
    "anchors_added",
    gap_class_21_signal_0_args,
    1
},
{
    "anchors_removed",
    gap_class_21_signal_1_args,
    1
},
{
    "anchors_updated",
    gap_class_21_signal_2_args,
    1
},
{
    "camera_tracking_changed",
    gap_class_21_signal_3_args,
    1
},
{
    "collaboration_data_received",
    gap_class_21_signal_4_args,
    1
},
{
    "frame_updated",
    gap_class_21_signal_5_args,
    1
},
{
    "hand_anchor_updated",
    gap_class_21_signal_6_args,
    1
},
{
    "interruption_ended",
    NULL,
    0
},
{
    "mesh_anchor_added",
    gap_class_21_signal_8_args,
    1
},
{
    "mesh_anchor_removed",
    gap_class_21_signal_9_args,
    1
},
{
    "mesh_anchor_updated",
    gap_class_21_signal_10_args,
    1
},
{
    "session_failed",
    gap_class_21_signal_11_args,
    1
},
{
    "session_interrupted",
    NULL,
    0
},
};
static const GAPStubConstantDescriptor gap_class_21_constants[] = {
{ "RunOption", "RESET_TRACKING", 1, 0 },
{ "RunOption", "REMOVE_EXISTING_ANCHORS", 2, 0 },
{ "RunOption", "STOP_TRACKED_RAYCASTS", 4, 0 },
{ "RunOption", "RESET_SCENE_RECONSTRUCTION", 8, 0 },
};
static const GAPStubNamedTypeInfo gap_class_22_method_2_args[] = {
    { "is_tracking", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_22_methods[] = {
{
    "stop_tracking",
    "ARTrackedRaycast.stop_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_tracking",
    "ARTrackedRaycast.get_is_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_tracking",
    "ARTrackedRaycast.set_is_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_22_method_2_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_22_properties[] = {
{
    "is_tracking",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_tracking",
    "get_is_tracking"
},
};
static const GAPStubNamedTypeInfo gap_class_23_method_0_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_23_method_3_args[] = {
    { "anchors", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_23_method_5_args[] = {
    { "center", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_23_method_7_args[] = {
    { "extent", { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_23_method_9_args[] = {
    { "raw_feature_points", { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARPointCloud", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_23_methods[] = {
{
    "deserialize",
    "ARWorldMap.deserialize",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARWorldMap", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_0_args,
    1
},
{
    "serialize",
    "ARWorldMap.serialize",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_anchors",
    "ARWorldMap.get_anchors",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_anchors",
    "ARWorldMap.set_anchors",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_3_args,
    1
},
{
    "get_center",
    "ARWorldMap.get_center",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_center",
    "ARWorldMap.set_center",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_5_args,
    1
},
{
    "get_extent",
    "ARWorldMap.get_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_extent",
    "ARWorldMap.set_extent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_7_args,
    1
},
{
    "get_raw_feature_points",
    "ARWorldMap.get_raw_feature_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARPointCloud", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_raw_feature_points",
    "ARWorldMap.set_raw_feature_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_23_properties[] = {
{
    "anchors",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_anchors",
    "get_anchors"
},
{
    "center",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_center",
    "get_center"
},
{
    "extent",
    { GDEXTENSION_VARIANT_TYPE_VECTOR3, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_extent",
    "get_extent"
},
{
    "raw_feature_points",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "ARPointCloud", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_raw_feature_points",
    "get_raw_feature_points"
},
};
static const GAPStubNamedTypeInfo gap_class_24_method_1_args[] = {
    { "groupName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_3_args[] = {
    { "scene_reconstruction", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_6_args[] = {
    { "app_clip_code_tracking_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_8_args[] = {
    { "automatic_image_scale_estimation_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_10_args[] = {
    { "detection_image_group_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_12_args[] = {
    { "environment_texturing", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_14_args[] = {
    { "frame_semantics_mask", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_16_args[] = {
    { "hand_tracking_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_18_args[] = {
    { "is_auto_focus_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_20_args[] = {
    { "is_collaboration_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_22_args[] = {
    { "is_light_estimation_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_24_args[] = {
    { "maximum_number_of_tracked_images", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_26_args[] = {
    { "plane_detection_mask", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_28_args[] = {
    { "provides_audio_data", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_30_args[] = {
    { "scene_reconstruction", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_32_args[] = {
    { "user_face_tracking_enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_34_args[] = {
    { "wants_hdr_environment_textures", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_36_args[] = {
    { "world_alignment", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_24_methods[] = {
{
    "is_supported",
    "ARWorldTrackingConfiguration.is_supported",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_detection_image_group",
    "ARWorldTrackingConfiguration.set_detection_image_group",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_1_args,
    1
},
{
    "supports_app_clip_code_tracking",
    "ARWorldTrackingConfiguration.supports_app_clip_code_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "supports_scene_reconstruction",
    "ARWorldTrackingConfiguration.supports_scene_reconstruction",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_3_args,
    1
},
{
    "supports_user_face_tracking",
    "ARWorldTrackingConfiguration.supports_user_face_tracking",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_app_clip_code_tracking_enabled",
    "ARWorldTrackingConfiguration.get_app_clip_code_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_app_clip_code_tracking_enabled",
    "ARWorldTrackingConfiguration.set_app_clip_code_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_6_args,
    1
},
{
    "get_automatic_image_scale_estimation_enabled",
    "ARWorldTrackingConfiguration.get_automatic_image_scale_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_automatic_image_scale_estimation_enabled",
    "ARWorldTrackingConfiguration.set_automatic_image_scale_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_8_args,
    1
},
{
    "get_detection_image_group_name",
    "ARWorldTrackingConfiguration.get_detection_image_group_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_detection_image_group_name",
    "ARWorldTrackingConfiguration.set_detection_image_group_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_10_args,
    1
},
{
    "get_environment_texturing",
    "ARWorldTrackingConfiguration.get_environment_texturing",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_environment_texturing",
    "ARWorldTrackingConfiguration.set_environment_texturing",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_12_args,
    1
},
{
    "get_frame_semantics_mask",
    "ARWorldTrackingConfiguration.get_frame_semantics_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_frame_semantics_mask",
    "ARWorldTrackingConfiguration.set_frame_semantics_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_14_args,
    1
},
{
    "get_hand_tracking_enabled",
    "ARWorldTrackingConfiguration.get_hand_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_hand_tracking_enabled",
    "ARWorldTrackingConfiguration.set_hand_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_16_args,
    1
},
{
    "get_is_auto_focus_enabled",
    "ARWorldTrackingConfiguration.get_is_auto_focus_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_auto_focus_enabled",
    "ARWorldTrackingConfiguration.set_is_auto_focus_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_18_args,
    1
},
{
    "get_is_collaboration_enabled",
    "ARWorldTrackingConfiguration.get_is_collaboration_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_collaboration_enabled",
    "ARWorldTrackingConfiguration.set_is_collaboration_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_20_args,
    1
},
{
    "get_is_light_estimation_enabled",
    "ARWorldTrackingConfiguration.get_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_light_estimation_enabled",
    "ARWorldTrackingConfiguration.set_is_light_estimation_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_22_args,
    1
},
{
    "get_maximum_number_of_tracked_images",
    "ARWorldTrackingConfiguration.get_maximum_number_of_tracked_images",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_maximum_number_of_tracked_images",
    "ARWorldTrackingConfiguration.set_maximum_number_of_tracked_images",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_24_args,
    1
},
{
    "get_plane_detection_mask",
    "ARWorldTrackingConfiguration.get_plane_detection_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_plane_detection_mask",
    "ARWorldTrackingConfiguration.set_plane_detection_mask",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_26_args,
    1
},
{
    "get_provides_audio_data",
    "ARWorldTrackingConfiguration.get_provides_audio_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_provides_audio_data",
    "ARWorldTrackingConfiguration.set_provides_audio_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_28_args,
    1
},
{
    "get_scene_reconstruction",
    "ARWorldTrackingConfiguration.get_scene_reconstruction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_scene_reconstruction",
    "ARWorldTrackingConfiguration.set_scene_reconstruction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_30_args,
    1
},
{
    "get_user_face_tracking_enabled",
    "ARWorldTrackingConfiguration.get_user_face_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_user_face_tracking_enabled",
    "ARWorldTrackingConfiguration.set_user_face_tracking_enabled",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_32_args,
    1
},
{
    "get_wants_hdr_environment_textures",
    "ARWorldTrackingConfiguration.get_wants_hdr_environment_textures",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_wants_hdr_environment_textures",
    "ARWorldTrackingConfiguration.set_wants_hdr_environment_textures",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_34_args,
    1
},
{
    "get_world_alignment",
    "ARWorldTrackingConfiguration.get_world_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_world_alignment",
    "ARWorldTrackingConfiguration.set_world_alignment",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_36_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_24_properties[] = {
{
    "app_clip_code_tracking_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_app_clip_code_tracking_enabled",
    "get_app_clip_code_tracking_enabled"
},
{
    "automatic_image_scale_estimation_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_automatic_image_scale_estimation_enabled",
    "get_automatic_image_scale_estimation_enabled"
},
{
    "detection_image_group_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_detection_image_group_name",
    "get_detection_image_group_name"
},
{
    "environment_texturing",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_environment_texturing",
    "get_environment_texturing"
},
{
    "frame_semantics_mask",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_frame_semantics_mask",
    "get_frame_semantics_mask"
},
{
    "hand_tracking_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_hand_tracking_enabled",
    "get_hand_tracking_enabled"
},
{
    "is_auto_focus_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_auto_focus_enabled",
    "get_is_auto_focus_enabled"
},
{
    "is_collaboration_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_collaboration_enabled",
    "get_is_collaboration_enabled"
},
{
    "is_light_estimation_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_light_estimation_enabled",
    "get_is_light_estimation_enabled"
},
{
    "maximum_number_of_tracked_images",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_maximum_number_of_tracked_images",
    "get_maximum_number_of_tracked_images"
},
{
    "plane_detection_mask",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_plane_detection_mask",
    "get_plane_detection_mask"
},
{
    "provides_audio_data",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_provides_audio_data",
    "get_provides_audio_data"
},
{
    "scene_reconstruction",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_scene_reconstruction",
    "get_scene_reconstruction"
},
{
    "user_face_tracking_enabled",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_user_face_tracking_enabled",
    "get_user_face_tracking_enabled"
},
{
    "wants_hdr_environment_textures",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_wants_hdr_environment_textures",
    "get_wants_hdr_environment_textures"
},
{
    "world_alignment",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_world_alignment",
    "get_world_alignment"
},
};
static const GAPStubConstantDescriptor gap_class_24_constants[] = {
{ "WorldAlignment", "GRAVITY", 0, 0 },
{ "WorldAlignment", "GRAVITY_AND_HEADING", 1, 0 },
{ "WorldAlignment", "CAMERA", 2, 0 },
{ "EnvironmentTexturing", "NONE", 0, 0 },
{ "EnvironmentTexturing", "MANUAL", 1, 0 },
{ "EnvironmentTexturing", "AUTOMATIC", 2, 0 },
{ "PlaneDetection", "HORIZONTAL", 1, 0 },
{ "PlaneDetection", "VERTICAL", 2, 0 },
{ "PlaneDetection", "SLANTED", 4, 0 },
{ "SceneReconstruction", "MESH", 1, 0 },
{ "SceneReconstruction", "MESH_WITH_CLASSIFICATION", 3, 0 },
{ "FrameSemantics", "PERSON_SEGMENTATION", 1, 0 },
{ "FrameSemantics", "PERSON_SEGMENTATION_WITH_DEPTH", 2, 0 },
{ "FrameSemantics", "BODY_DETECTION", 4, 0 },
};
static const GAPStubMethodDescriptor gap_class_25_methods[] = {
{
    "get_authorization_code",
    "ASAuthorizationAppleIDCredential.get_authorization_code",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_authorized_scopes",
    "ASAuthorizationAppleIDCredential.get_authorized_scopes",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_email",
    "ASAuthorizationAppleIDCredential.get_email",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_full_name",
    "ASAuthorizationAppleIDCredential.get_full_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identity_token",
    "ASAuthorizationAppleIDCredential.get_identity_token",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_real_user_status",
    "ASAuthorizationAppleIDCredential.get_real_user_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_state",
    "ASAuthorizationAppleIDCredential.get_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_user",
    "ASAuthorizationAppleIDCredential.get_user",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_user_age_range",
    "ASAuthorizationAppleIDCredential.get_user_age_range",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_25_properties[] = {
{
    "authorization_code",
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_authorization_code"
},
{
    "authorized_scopes",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_authorized_scopes"
},
{
    "email",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_email"
},
{
    "full_name",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_full_name"
},
{
    "identity_token",
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identity_token"
},
{
    "real_user_status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_real_user_status"
},
{
    "state",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_state"
},
{
    "user",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_user"
},
{
    "user_age_range",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_user_age_range"
},
};
static const GAPStubConstantDescriptor gap_class_25_constants[] = {
{ "UserDetectionStatus", "UNSUPPORTED", 0, 0 },
{ "UserDetectionStatus", "UNKNOWN", 1, 0 },
{ "UserDetectionStatus", "LIKELY_REAL", 2, 0 },
{ "UserAgeRange", "NOT_KNOWN", 0, 0 },
{ "UserAgeRange", "CHILD", 1, 0 },
{ "UserAgeRange", "NOT_CHILD", 2, 0 },
};
static const GAPStubNamedTypeInfo gap_class_26_method_1_args[] = {
    { "scopeStrings", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_signal_0_args[] = {
    { "credential", { GDEXTENSION_VARIANT_TYPE_OBJECT, "RefCounted", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_signal_1_args[] = {
    { "error", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_26_methods[] = {
{
    "signin",
    "ASAuthorizationController.signin",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "signin_with_scopes",
    "ASAuthorizationController.signin_with_scopes",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_26_method_1_args,
    1
},
};
static const GAPStubSignalDescriptor gap_class_26_signals[] = {
{
    "authorization_completed",
    gap_class_26_signal_0_args,
    1
},
{
    "authorization_failed",
    gap_class_26_signal_1_args,
    1
},
};
static const GAPStubMethodDescriptor gap_class_27_methods[] = {
{
    "get_password",
    "ASPasswordCredential.get_password",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_user",
    "ASPasswordCredential.get_user",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_27_properties[] = {
{
    "password",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_password"
},
{
    "user",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_user"
},
};
static const GAPStubNamedTypeInfo gap_class_28_method_1_args[] = {
    { "auth_url", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback_scheme", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "prefers_ephemeral", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_1_args[] = {
    { "callback_url", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_28_methods[] = {
{
    "cancel",
    "ASWebAuthenticationSession.cancel",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "start",
    "ASWebAuthenticationSession.start",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_28_method_1_args,
    3
},
};
static const GAPStubSignalDescriptor gap_class_28_signals[] = {
{
    "canceled",
    NULL,
    0
},
{
    "completed",
    gap_class_28_signal_1_args,
    1
},
{
    "failed",
    gap_class_28_signal_2_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_29_method_0_args[] = {
    { "category", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "policy", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_29_method_2_args[] = {
    { "current_category", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_29_methods[] = {
{
    "set_category",
    "AVAudioSession.set_category",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_29_method_0_args,
    4
},
{
    "get_current_category",
    "AVAudioSession.get_current_category",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_current_category",
    "AVAudioSession.set_current_category",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_29_method_2_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_29_properties[] = {
{
    "current_category",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_current_category",
    "get_current_category"
},
};
static const GAPStubConstantDescriptor gap_class_29_constants[] = {
{ "CategoryOptions", "MIX_WITH_OTHERS", 1, 0 },
{ "CategoryOptions", "DUCK_OTHERS", 2, 0 },
{ "CategoryOptions", "ALLOW_BLUETOOTH", 4, 0 },
{ "CategoryOptions", "DEFAULT_TO_SPEAKER", 8, 0 },
{ "CategoryOptions", "INTERRUPT_SPOKEN_AUDIO_AND_MIX_WITH_OTHERS", 17, 0 },
{ "CategoryOptions", "ALLOW_BLUETOOTH_A2DP", 32, 0 },
{ "CategoryOptions", "ALLOW_AIRPLAY", 64, 0 },
{ "CategoryOptions", "OVERRIDE_MUTED_MICROPHONE_INTERRUPTION", 128, 0 },
{ "RouteSharingPolicy", "ROUTE_SHARING_DEFAULT", 0, 0 },
{ "RouteSharingPolicy", "LONG_FORM_AUDIO", 1, 0 },
{ "RouteSharingPolicy", "INDEPENDENT", 2, 0 },
{ "RouteSharingPolicy", "LONG_FORM", 3, 0 },
{ "SessionCategory", "AMBIENT", 0, 0 },
{ "SessionCategory", "MULTI_ROUTE", 1, 0 },
{ "SessionCategory", "PLAY_AND_RECORD", 2, 0 },
{ "SessionCategory", "PLAYBACK", 3, 0 },
{ "SessionCategory", "RECORD", 4, 0 },
{ "SessionCategory", "SOLO_AMBIENT", 5, 0 },
{ "SessionCategory", "UNKNOWN", 6, 0 },
{ "SessionMode", "DEFAULT", 0, 0 },
{ "SessionMode", "GAME_CHAT", 1, 0 },
{ "SessionMode", "MEASUREMENT", 2, 0 },
{ "SessionMode", "MOVIE_PLAYBACK", 3, 0 },
{ "SessionMode", "SPOKEN_AUDIO", 4, 0 },
{ "SessionMode", "VIDEO_CHAT", 5, 0 },
{ "SessionMode", "VOICE_CHAT", 6, 0 },
{ "SessionMode", "VOICE_PROMPT", 7, 0 },
};
static const GAPStubNamedTypeInfo gap_class_30_method_0_args[] = {
    { "allowed_types", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "allow_multiple", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_30_signal_1_args[] = {
    { "url", { GDEXTENSION_VARIANT_TYPE_OBJECT, "AppleURL", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "path", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_30_signal_2_args[] = {
    { "urls", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "paths", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_30_methods[] = {
{
    "pick_document",
    "AppleFilePicker.pick_document",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_30_method_0_args,
    2
},
};
static const GAPStubSignalDescriptor gap_class_30_signals[] = {
{
    "canceled",
    NULL,
    0
},
{
    "file_selected",
    gap_class_30_signal_1_args,
    2
},
{
    "files_selected",
    gap_class_30_signal_2_args,
    2
},
};
static const GAPStubNamedTypeInfo gap_class_31_method_5_args[] = {
    { "path", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_31_method_6_args[] = {
    { "str", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_31_methods[] = {
{
    "get_absolute_string",
    "AppleURL.get_absolute_string",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_data",
    "AppleURL.get_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_path",
    "AppleURL.get_path",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_path_encoded",
    "AppleURL.get_path_encoded",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_string",
    "AppleURL.get_string",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_from_filepath",
    "AppleURL.set_from_filepath",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_31_method_5_args,
    1
},
{
    "set_value",
    "AppleURL.set_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_31_method_6_args,
    1
},
{
    "start_accessing_security_scoped_resource",
    "AppleURL.start_accessing_security_scoped_resource",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "stop_accessing_security_scoped_resource",
    "AppleURL.stop_accessing_security_scoped_resource",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubMethodDescriptor gap_class_32_methods[] = {
{
    "uuid",
    "Foundation.uuid",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubNamedTypeInfo gap_class_33_method_0_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_1_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_2_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_3_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_4_args[] = {
    { "achievementID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_5_args[] = {
    { "challengeDefinitionID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_6_args[] = {
    { "gameActivity", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivity", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_7_args[] = {
    { "gameActivityDefinitionID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_8_args[] = {
    { "leaderboardID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "playerScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_9_args[] = {
    { "leaderboardSetID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_10_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_11_args[] = {
    { "state", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_13_args[] = {
    { "active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_18_args[] = {
    { "location", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_33_method_20_args[] = {
    { "show_highlights", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_33_methods[] = {
{
    "trigger",
    "GKAccessPoint.trigger",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_0_args,
    1
},
{
    "trigger_for_challenges",
    "GKAccessPoint.trigger_for_challenges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_1_args,
    1
},
{
    "trigger_for_friending",
    "GKAccessPoint.trigger_for_friending",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_2_args,
    1
},
{
    "trigger_for_play_together",
    "GKAccessPoint.trigger_for_play_together",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_3_args,
    1
},
{
    "trigger_with_achievement",
    "GKAccessPoint.trigger_with_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_4_args,
    2
},
{
    "trigger_with_challenge_definition_id",
    "GKAccessPoint.trigger_with_challenge_definition_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_5_args,
    2
},
{
    "trigger_with_game_activity",
    "GKAccessPoint.trigger_with_game_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_6_args,
    2
},
{
    "trigger_with_game_activity_definition_id",
    "GKAccessPoint.trigger_with_game_activity_definition_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_7_args,
    2
},
{
    "trigger_with_leaderboard",
    "GKAccessPoint.trigger_with_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_8_args,
    4
},
{
    "trigger_with_leaderboard_set",
    "GKAccessPoint.trigger_with_leaderboard_set",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_9_args,
    2
},
{
    "trigger_with_player",
    "GKAccessPoint.trigger_with_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_10_args,
    2
},
{
    "trigger_with_state",
    "GKAccessPoint.trigger_with_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_11_args,
    2
},
{
    "get_active",
    "GKAccessPoint.get_active",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_active",
    "GKAccessPoint.set_active",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_13_args,
    1
},
{
    "get_frame_in_screen_coordinates",
    "GKAccessPoint.get_frame_in_screen_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_RECT2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_frame_in_unit_coordinates",
    "GKAccessPoint.get_frame_in_unit_coordinates",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_RECT2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_presenting_game_center",
    "GKAccessPoint.get_is_presenting_game_center",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_location",
    "GKAccessPoint.get_location",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_location",
    "GKAccessPoint.set_location",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_18_args,
    1
},
{
    "get_show_highlights",
    "GKAccessPoint.get_show_highlights",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_show_highlights",
    "GKAccessPoint.set_show_highlights",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_33_method_20_args,
    1
},
{
    "get_visible",
    "GKAccessPoint.get_visible",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_33_properties[] = {
{
    "active",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_active",
    "get_active"
},
{
    "frame_in_screen_coordinates",
    { GDEXTENSION_VARIANT_TYPE_RECT2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_frame_in_screen_coordinates"
},
{
    "frame_in_unit_coordinates",
    { GDEXTENSION_VARIANT_TYPE_RECT2, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_frame_in_unit_coordinates"
},
{
    "is_presenting_game_center",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_presenting_game_center"
},
{
    "location",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_location",
    "get_location"
},
{
    "show_highlights",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_show_highlights",
    "get_show_highlights"
},
{
    "visible",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_visible"
},
};
static const GAPStubConstantDescriptor gap_class_33_constants[] = {
{ "Location", "TOP_LEADING", 0, 0 },
{ "Location", "TOP_TRAILING", 1, 0 },
{ "Location", "BOTTOM_LEADING", 2, 0 },
{ "Location", "BOTTOM_TRAILING", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_34_method_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_2_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_3_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_4_args[] = {
    { "achievements", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_5_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_6_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_8_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_12_args[] = {
    { "percent_complete", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_15_args[] = {
    { "shows_completion_banner", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_34_methods[] = {
{
    "challenge_compose_controller",
    "GKAchievement.challenge_compose_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_0_args,
    2
},
{
    "load_achievements",
    "GKAchievement.load_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_1_args,
    1
},
{
    "make",
    "GKAchievement.make",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_2_args,
    1
},
{
    "make_for_player",
    "GKAchievement.make_for_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_3_args,
    2
},
{
    "report_achievement",
    "GKAchievement.report_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_4_args,
    2
},
{
    "reset_achievements",
    "GKAchievement.reset_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_5_args,
    1
},
{
    "select_challengeable_players",
    "GKAchievement.select_challengeable_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_6_args,
    2
},
{
    "get_identifier",
    "GKAchievement.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_identifier",
    "GKAchievement.set_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_8_args,
    1
},
{
    "get_is_completed",
    "GKAchievement.get_is_completed",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_last_reported_date",
    "GKAchievement.get_last_reported_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_percent_complete",
    "GKAchievement.get_percent_complete",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_percent_complete",
    "GKAchievement.set_percent_complete",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_12_args,
    1
},
{
    "get_player",
    "GKAchievement.get_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_shows_completion_banner",
    "GKAchievement.get_shows_completion_banner",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_shows_completion_banner",
    "GKAchievement.set_shows_completion_banner",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_15_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_34_properties[] = {
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_identifier",
    "get_identifier"
},
{
    "is_completed",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_completed"
},
{
    "last_reported_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_last_reported_date"
},
{
    "percent_complete",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_percent_complete",
    "get_percent_complete"
},
{
    "player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player"
},
{
    "shows_completion_banner",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_shows_completion_banner",
    "get_shows_completion_banner"
},
};
static const GAPStubNamedTypeInfo gap_class_35_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_35_methods[] = {
{
    "decline",
    "GKChallenge.decline",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "load_received_challenges",
    "GKChallenge.load_received_challenges",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_35_method_1_args,
    1
},
{
    "get_challenge_type",
    "GKChallenge.get_challenge_type",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_completion_date",
    "GKChallenge.get_completion_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_issue_date",
    "GKChallenge.get_issue_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_issuing_player",
    "GKChallenge.get_issuing_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_message",
    "GKChallenge.get_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_receiving_player",
    "GKChallenge.get_receiving_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_state",
    "GKChallenge.get_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_35_properties[] = {
{
    "challenge_type",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_challenge_type"
},
{
    "completion_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_completion_date"
},
{
    "issue_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_issue_date"
},
{
    "issuing_player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_issuing_player"
},
{
    "message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_message"
},
{
    "receiving_player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_receiving_player"
},
{
    "state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_state"
},
};
static const GAPStubConstantDescriptor gap_class_35_constants[] = {
{ "ChallengeState", "INVALID", 0, 0 },
{ "ChallengeState", "PENDING", 1, 0 },
{ "ChallengeState", "COMPLETED", 2, 0 },
{ "ChallengeState", "DECLINED", 3, 0 },
{ "ChallengeType", "SCORE", 0, 0 },
{ "ChallengeType", "ACHIEVEMENT", 1, 0 },
{ "ChallengeType", "UNKNOWN", 2, 0 },
};
static const GAPStubMethodDescriptor gap_class_36_methods[] = {
{
    "get_achievement",
    "GKAchievementChallenge.get_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_36_properties[] = {
{
    "achievement",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_achievement"
},
};
static const GAPStubNamedTypeInfo gap_class_37_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_37_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_37_methods[] = {
{
    "incomplete_achievement_image",
    "GKAchievementDescription.incomplete_achievement_image",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "load_achievement_descriptions",
    "GKAchievementDescription.load_achievement_descriptions",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_37_method_1_args,
    1
},
{
    "load_image",
    "GKAchievementDescription.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_37_method_2_args,
    1
},
{
    "placeholder_completed_achievement_image",
    "GKAchievementDescription.placeholder_completed_achievement_image",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "get_achieved_description",
    "GKAchievementDescription.get_achieved_description",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_activity_identifier",
    "GKAchievementDescription.get_activity_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_activity_properties",
    "GKAchievementDescription.get_activity_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_group_identifier",
    "GKAchievementDescription.get_group_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "GKAchievementDescription.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_hidden",
    "GKAchievementDescription.get_is_hidden",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_replayable",
    "GKAchievementDescription.get_is_replayable",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_maximum_points",
    "GKAchievementDescription.get_maximum_points",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_rarity_percent",
    "GKAchievementDescription.get_rarity_percent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "get_release_state",
    "GKAchievementDescription.get_release_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_title",
    "GKAchievementDescription.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_unachieved_description",
    "GKAchievementDescription.get_unachieved_description",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_37_properties[] = {
{
    "achieved_description",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_achieved_description"
},
{
    "activity_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_activity_identifier"
},
{
    "activity_properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_activity_properties"
},
{
    "group_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_identifier"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "is_hidden",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_hidden"
},
{
    "is_replayable",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_replayable"
},
{
    "maximum_points",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_maximum_points"
},
{
    "rarity_percent",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "",
    "get_rarity_percent"
},
{
    "release_state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_release_state"
},
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_title"
},
{
    "unachieved_description",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_unachieved_description"
},
};
static const GAPStubNamedTypeInfo gap_class_38_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_38_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_38_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_38_methods[] = {
{
    "has_active_challenges",
    "GKChallengeDefinition.has_active_challenges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_38_method_0_args,
    1
},
{
    "load_challenge_definitions",
    "GKChallengeDefinition.load_challenge_definitions",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_38_method_1_args,
    1
},
{
    "load_image",
    "GKChallengeDefinition.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_38_method_2_args,
    1
},
{
    "get_details",
    "GKChallengeDefinition.get_details",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_duration_options",
    "GKChallengeDefinition.get_duration_options",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_group_identifier",
    "GKChallengeDefinition.get_group_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "GKChallengeDefinition.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_repeatable",
    "GKChallengeDefinition.get_is_repeatable",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_leaderboard",
    "GKChallengeDefinition.get_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_release_state",
    "GKChallengeDefinition.get_release_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_title",
    "GKChallengeDefinition.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_38_properties[] = {
{
    "details",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_details"
},
{
    "duration_options",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_duration_options"
},
{
    "group_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_identifier"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "is_repeatable",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_repeatable"
},
{
    "leaderboard",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_leaderboard"
},
{
    "release_state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_release_state"
},
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_title"
},
};
static const GAPStubNamedTypeInfo gap_class_39_method_1_args[] = {
    { "code", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_39_method_3_args[] = {
    { "domain", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_39_method_5_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_39_methods[] = {
{
    "get_code",
    "GKError.get_code",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_code",
    "GKError.set_code",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_39_method_1_args,
    1
},
{
    "get_domain",
    "GKError.get_domain",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_domain",
    "GKError.set_domain",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_39_method_3_args,
    1
},
{
    "get_message",
    "GKError.get_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_message",
    "GKError.set_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_39_method_5_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_39_properties[] = {
{
    "code",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_code",
    "get_code"
},
{
    "domain",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_domain",
    "get_domain"
},
{
    "message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_message",
    "get_message"
},
};
static const GAPStubConstantDescriptor gap_class_39_constants[] = {
{ "Code", "GAME_UNRECOGNIZED", 0, 0 },
{ "Code", "NOT_SUPPORTED", 1, 0 },
{ "Code", "APP_UNLISTED", 2, 0 },
{ "Code", "UNKNOWN", 3, 0 },
{ "Code", "CANCELLED", 4, 0 },
{ "Code", "COMMUNICATIONS_FAILURE", 5, 0 },
{ "Code", "INVALID_PLAYER", 6, 0 },
{ "Code", "INVALID_PARAMETER", 7, 0 },
{ "Code", "GAME_SESSION_REQUEST_INVALID", 8, 0 },
{ "Code", "API_NOT_AVAILABLE", 9, 0 },
{ "Code", "CONNECTION_TIMEOUT", 10, 0 },
{ "Code", "API_OBSOLETE", 11, 0 },
{ "Code", "USER_DENIED", 12, 0 },
{ "Code", "INVALID_CREDENTIALS", 13, 0 },
{ "Code", "NOT_AUTHENTICATED", 14, 0 },
{ "Code", "AUTHENTICATION_IN_PROGRESS", 15, 0 },
{ "Code", "PARENTAL_CONTROLS_BLOCKED", 16, 0 },
{ "Code", "PLAYER_STATUS_EXCEEDS_MAXIMUM_LENGTH", 17, 0 },
{ "Code", "PLAYER_STATUS_INVALID", 18, 0 },
{ "Code", "UNDERAGE", 19, 0 },
{ "Code", "PLAYER_PHOTO_FAILURE", 20, 0 },
{ "Code", "UBIQUITY_CONTAINER_UNAVAILABLE", 21, 0 },
{ "Code", "NOT_AUTHORIZED", 22, 0 },
{ "Code", "ICLOUD_UNAVAILABLE", 23, 0 },
{ "Code", "LOCKDOWN_MODE", 24, 0 },
{ "Code", "FRIEND_LIST_DESCRIPTION_MISSING", 25, 0 },
{ "Code", "FRIEND_LIST_RESTRICTED", 26, 0 },
{ "Code", "FRIEND_LIST_DENIED", 27, 0 },
{ "Code", "FRIEND_REQUEST_NOT_AVAILABLE", 28, 0 },
{ "Code", "MATCH_REQUEST_INVALID", 29, 0 },
{ "Code", "UNEXPECTED_CONNECTION", 30, 0 },
{ "Code", "INVITATIONS_DISABLED", 31, 0 },
{ "Code", "MATCH_NOT_CONNECTED", 32, 0 },
{ "Code", "RESTRICTED_TO_AUTOMATCH", 33, 0 },
{ "Code", "TURN_BASED_MATCH_DATA_TOO_LARGE", 34, 0 },
{ "Code", "TURN_BASED_TOO_MANY_SESSIONS", 35, 0 },
{ "Code", "TURN_BASED_INVALID_PARTICIPANT", 36, 0 },
{ "Code", "TURN_BASED_INVALID_TURN", 37, 0 },
{ "Code", "TURN_BASED_INVALID_STATE", 38, 0 },
{ "Code", "SCORE_NOT_SET", 39, 0 },
{ "Code", "CHALLENGE_INVALID", 40, 0 },
{ "Code", "DEBUG_MODE", 41, 0 },
};
static const GAPStubNamedTypeInfo gap_class_40_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_1_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_5_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_6_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_7_args[] = {
    { "partyCode", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_10_args[] = {
    { "achievements", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_11_args[] = {
    { "leaderboards", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_13_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_14_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "percentComplete", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_15_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_16_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_18_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_19_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "partyCode", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_40_method_32_args[] = {
    { "properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_40_methods[] = {
{
    "check_pending_game_activity_existence",
    "GKGameActivity.check_pending_game_activity_existence",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_0_args,
    1
},
{
    "create_with_definition",
    "GKGameActivity.create_with_definition",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivity", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_1_args,
    1
},
{
    "end",
    "GKGameActivity.end",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "find_match",
    "GKGameActivity.find_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_3_args,
    1
},
{
    "find_players_for_hosted_match",
    "GKGameActivity.find_players_for_hosted_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_4_args,
    1
},
{
    "get_progress_on_achievement",
    "GKGameActivity.get_progress_on_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_5_args,
    1
},
{
    "get_score_on_leaderboard",
    "GKGameActivity.get_score_on_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboardScore", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_6_args,
    1
},
{
    "is_valid_party_code",
    "GKGameActivity.is_valid_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_7_args,
    1
},
{
    "make_match_request",
    "GKGameActivity.make_match_request",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "pause",
    "GKGameActivity.pause",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "remove_achievements",
    "GKGameActivity.remove_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_10_args,
    1
},
{
    "remove_scores_from_leaderboards",
    "GKGameActivity.remove_scores_from_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_11_args,
    1
},
{
    "resume",
    "GKGameActivity.resume",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_achievement_completed",
    "GKGameActivity.set_achievement_completed",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_13_args,
    1
},
{
    "set_progress_on_achievement",
    "GKGameActivity.set_progress_on_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_14_args,
    2
},
{
    "set_score_on_leaderboard",
    "GKGameActivity.set_score_on_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_15_args,
    2
},
{
    "set_score_on_leaderboard_with_context",
    "GKGameActivity.set_score_on_leaderboard_with_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_16_args,
    3
},
{
    "start",
    "GKGameActivity.start",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "start_with_definition",
    "GKGameActivity.start_with_definition",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_18_args,
    2
},
{
    "start_with_definition_and_party_code",
    "GKGameActivity.start_with_definition_and_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_19_args,
    3
},
{
    "valid_party_code_alphabet",
    "GKGameActivity.valid_party_code_alphabet",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_achievements",
    "GKGameActivity.get_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_activity_definition",
    "GKGameActivity.get_activity_definition",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_creation_date",
    "GKGameActivity.get_creation_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_duration",
    "GKGameActivity.get_duration",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_end_date",
    "GKGameActivity.get_end_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "GKGameActivity.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_last_resume_date",
    "GKGameActivity.get_last_resume_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_leaderboard_scores",
    "GKGameActivity.get_leaderboard_scores",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_party_code",
    "GKGameActivity.get_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_party_url",
    "GKGameActivity.get_party_url",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_properties",
    "GKGameActivity.get_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_properties",
    "GKGameActivity.set_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_40_method_32_args,
    1
},
{
    "get_start_date",
    "GKGameActivity.get_start_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_state",
    "GKGameActivity.get_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_40_properties[] = {
{
    "achievements",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_achievements"
},
{
    "activity_definition",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_activity_definition"
},
{
    "creation_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_creation_date"
},
{
    "duration",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_duration"
},
{
    "end_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_end_date"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "last_resume_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_last_resume_date"
},
{
    "leaderboard_scores",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_leaderboard_scores"
},
{
    "party_code",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_party_code"
},
{
    "party_url",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_party_url"
},
{
    "properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_properties",
    "get_properties"
},
{
    "start_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_start_date"
},
{
    "state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_state"
},
};
static const GAPStubNamedTypeInfo gap_class_41_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_2_args[] = {
    { "ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_41_methods[] = {
{
    "load_achievement_descriptions",
    "GKGameActivityDefinition.load_achievement_descriptions",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_0_args,
    1
},
{
    "load_game_activity_definitions",
    "GKGameActivityDefinition.load_game_activity_definitions",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_1_args,
    1
},
{
    "load_game_activity_definitions_with_ids",
    "GKGameActivityDefinition.load_game_activity_definitions_with_ids",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_2_args,
    2
},
{
    "load_image",
    "GKGameActivityDefinition.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_3_args,
    1
},
{
    "load_leaderboards",
    "GKGameActivityDefinition.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_4_args,
    1
},
{
    "get_default_properties",
    "GKGameActivityDefinition.get_default_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_details",
    "GKGameActivityDefinition.get_details",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_fallback_url",
    "GKGameActivityDefinition.get_fallback_url",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_group_identifier",
    "GKGameActivityDefinition.get_group_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "GKGameActivityDefinition.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_max_players",
    "GKGameActivityDefinition.get_max_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "get_min_players",
    "GKGameActivityDefinition.get_min_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "get_play_style",
    "GKGameActivityDefinition.get_play_style",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_release_state",
    "GKGameActivityDefinition.get_release_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_supports_party_code",
    "GKGameActivityDefinition.get_supports_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_supports_unlimited_players",
    "GKGameActivityDefinition.get_supports_unlimited_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_title",
    "GKGameActivityDefinition.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_41_properties[] = {
{
    "default_properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_default_properties"
},
{
    "details",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_details"
},
{
    "fallback_url",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_fallback_url"
},
{
    "group_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_identifier"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "max_players",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "",
    "get_max_players"
},
{
    "min_players",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "",
    "get_min_players"
},
{
    "play_style",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_play_style"
},
{
    "release_state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_release_state"
},
{
    "supports_party_code",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_supports_party_code"
},
{
    "supports_unlimited_players",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_supports_unlimited_players"
},
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_title"
},
};
static const GAPStubNamedTypeInfo gap_class_42_method_0_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_42_method_1_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "scope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_42_method_2_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "scope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_42_method_3_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_42_method_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_42_method_5_args[] = {
    { "type", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_42_methods[] = {
{
    "show_achievement",
    "GKGameCenterViewController.show_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_0_args,
    1
},
{
    "show_leaderboard",
    "GKGameCenterViewController.show_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_1_args,
    2
},
{
    "show_leaderboard_time_period",
    "GKGameCenterViewController.show_leaderboard_time_period",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_2_args,
    3
},
{
    "show_leaderboardset",
    "GKGameCenterViewController.show_leaderboardset",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_3_args,
    1
},
{
    "show_player",
    "GKGameCenterViewController.show_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_4_args,
    1
},
{
    "show_type",
    "GKGameCenterViewController.show_type",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_42_method_5_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_42_constants[] = {
{ "State", "DEFAULT_SCREEN", 0, 0 },
{ "State", "LEADERBOARDS", 1, 0 },
{ "State", "ACHIEVEMENTS", 2, 0 },
{ "State", "LOCAL_PLAYER_PROFILE", 3, 0 },
{ "State", "DASHBOARD", 4, 0 },
{ "State", "LOCAL_PLAYER_FRIENDS_LIST", 5, 0 },
};
static const GAPStubMethodDescriptor gap_class_43_methods[] = {
{
    "get_is_hosted",
    "GKInvite.get_is_hosted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_player_attributes",
    "GKInvite.get_player_attributes",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_player_group",
    "GKInvite.get_player_group",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_sender",
    "GKInvite.get_sender",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_43_properties[] = {
{
    "is_hosted",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_hosted"
},
{
    "player_attributes",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player_attributes"
},
{
    "player_group",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player_group"
},
{
    "sender",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_sender"
},
};
static const GAPStubNamedTypeInfo gap_class_44_method_0_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_2_args[] = {
    { "ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_3_args[] = {
    { "playerScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "rangeStart", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "rangeLength", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_5_args[] = {
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_44_methods[] = {
{
    "load_entries",
    "GKLeaderboard.load_entries",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_0_args,
    3
},
{
    "load_image",
    "GKLeaderboard.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_1_args,
    1
},
{
    "load_leaderboards",
    "GKLeaderboard.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_2_args,
    2
},
{
    "load_local_player_entries",
    "GKLeaderboard.load_local_player_entries",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_3_args,
    5
},
{
    "load_previous_occurrence",
    "GKLeaderboard.load_previous_occurrence",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_4_args,
    1
},
{
    "submit_score",
    "GKLeaderboard.submit_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_5_args,
    4
},
{
    "get_activity_identifier",
    "GKLeaderboard.get_activity_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_activity_properties",
    "GKLeaderboard.get_activity_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_base_leaderboard_id",
    "GKLeaderboard.get_base_leaderboard_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_duration",
    "GKLeaderboard.get_duration",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_group_identifier",
    "GKLeaderboard.get_group_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_hidden",
    "GKLeaderboard.get_is_hidden",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_leaderboard_description",
    "GKLeaderboard.get_leaderboard_description",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_next_start_date",
    "GKLeaderboard.get_next_start_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_release_state",
    "GKLeaderboard.get_release_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_start_date",
    "GKLeaderboard.get_start_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_title",
    "GKLeaderboard.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_type",
    "GKLeaderboard.get_type",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_44_properties[] = {
{
    "activity_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_activity_identifier"
},
{
    "activity_properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_activity_properties"
},
{
    "base_leaderboard_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_base_leaderboard_id"
},
{
    "duration",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_duration"
},
{
    "group_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_identifier"
},
{
    "is_hidden",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_hidden"
},
{
    "leaderboard_description",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_leaderboard_description"
},
{
    "next_start_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_next_start_date"
},
{
    "release_state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_release_state"
},
{
    "start_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_start_date"
},
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_title"
},
{
    "type",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_type"
},
};
static const GAPStubConstantDescriptor gap_class_44_constants[] = {
{ "AppleLeaderboardType", "CLASSIC", 0, 0 },
{ "AppleLeaderboardType", "RECURRING", 1, 0 },
{ "AppleLeaderboardType", "UNKNOWN", 2, 0 },
{ "TimeScope", "TODAY", 0, 0 },
{ "TimeScope", "WEEK", 1, 0 },
{ "TimeScope", "ALL_TIME", 2, 0 },
{ "PlayerScope", "GLOBAL", 0, 0 },
{ "PlayerScope", "FRIENDS_ONLY", 1, 0 },
};
static const GAPStubMethodDescriptor gap_class_45_methods[] = {
{
    "get_context",
    "GKLeaderboardEntry.get_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_date",
    "GKLeaderboardEntry.get_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_formatted_score",
    "GKLeaderboardEntry.get_formatted_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_player",
    "GKLeaderboardEntry.get_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_rank",
    "GKLeaderboardEntry.get_rank",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_score",
    "GKLeaderboardEntry.get_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_45_properties[] = {
{
    "context",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_context"
},
{
    "date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_date"
},
{
    "formatted_score",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_formatted_score"
},
{
    "player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player"
},
{
    "rank",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_rank"
},
{
    "score",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_score"
},
};
static const GAPStubNamedTypeInfo gap_class_46_method_1_args[] = {
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_46_method_3_args[] = {
    { "leaderboard_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_46_method_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_46_method_7_args[] = {
    { "value", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_46_methods[] = {
{
    "get_context",
    "GKLeaderboardScore.get_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_context",
    "GKLeaderboardScore.set_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_46_method_1_args,
    1
},
{
    "get_leaderboard_id",
    "GKLeaderboardScore.get_leaderboard_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_leaderboard_id",
    "GKLeaderboardScore.set_leaderboard_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_46_method_3_args,
    1
},
{
    "get_player",
    "GKLeaderboardScore.get_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_player",
    "GKLeaderboardScore.set_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_46_method_5_args,
    1
},
{
    "get_value",
    "GKLeaderboardScore.get_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_value",
    "GKLeaderboardScore.set_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_46_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_46_properties[] = {
{
    "context",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_context",
    "get_context"
},
{
    "leaderboard_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_leaderboard_id",
    "get_leaderboard_id"
},
{
    "player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_player",
    "get_player"
},
{
    "value",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_value",
    "get_value"
},
};
static const GAPStubNamedTypeInfo gap_class_47_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_47_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_47_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_47_methods[] = {
{
    "load_image",
    "GKLeaderboardSet.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_0_args,
    1
},
{
    "load_leaderboard_sets",
    "GKLeaderboardSet.load_leaderboard_sets",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_1_args,
    1
},
{
    "load_leaderboards",
    "GKLeaderboardSet.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_2_args,
    1
},
{
    "get_group_identifier",
    "GKLeaderboardSet.get_group_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_identifier",
    "GKLeaderboardSet.get_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_title",
    "GKLeaderboardSet.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_47_properties[] = {
{
    "group_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_identifier"
},
{
    "identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_identifier"
},
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_title"
},
};
static const GAPStubNamedTypeInfo gap_class_48_method_0_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_48_method_1_args[] = {
    { "small", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_48_methods[] = {
{
    "anonymous_guest_player",
    "GKPlayer.anonymous_guest_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_48_method_0_args,
    1
},
{
    "load_photo",
    "GKPlayer.load_photo",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_48_method_1_args,
    2
},
{
    "scopedIDsArePersistent",
    "GKPlayer.scopedIDsArePersistent",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_alias",
    "GKPlayer.get_alias",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_display_name",
    "GKPlayer.get_display_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_game_player_id",
    "GKPlayer.get_game_player_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_guest_identifier",
    "GKPlayer.get_guest_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_invitable",
    "GKPlayer.get_is_invitable",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_team_player_id",
    "GKPlayer.get_team_player_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_48_properties[] = {
{
    "alias",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_alias"
},
{
    "display_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_display_name"
},
{
    "game_player_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_game_player_id"
},
{
    "guest_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_guest_identifier"
},
{
    "is_invitable",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_invitable"
},
{
    "team_player_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_team_player_id"
},
};
static const GAPStubNamedTypeInfo gap_class_49_method_0_args[] = {
    { "named", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_5_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_6_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_7_args[] = {
    { "identifiers", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_10_args[] = {
    { "conflicts", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_11_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "withName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_method_12_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "friend_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_2_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "friend_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_3_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "conflicting_saved_games", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "exchange", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedExchange", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_6_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "replies", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_7_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "exchange", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedExchange", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_8_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_9_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "recipient_players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_10_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_11_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "saved_game", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKSavedGame", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_12_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_49_signal_13_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "did_become_active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_49_methods[] = {
{
    "delete_saved_games",
    "GKLocalPlayer.delete_saved_games",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_0_args,
    2
},
{
    "fetch_items_for_identity_verification_signature",
    "GKLocalPlayer.fetch_items_for_identity_verification_signature",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_1_args,
    1
},
{
    "fetch_saved_games",
    "GKLocalPlayer.fetch_saved_games",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_2_args,
    1
},
{
    "load_challengeable_friends",
    "GKLocalPlayer.load_challengeable_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_3_args,
    1
},
{
    "load_default_leaderboard_identifier",
    "GKLocalPlayer.load_default_leaderboard_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_4_args,
    1
},
{
    "load_friends",
    "GKLocalPlayer.load_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_5_args,
    1
},
{
    "load_friends_authorization_status",
    "GKLocalPlayer.load_friends_authorization_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_6_args,
    1
},
{
    "load_friends_with_identifiers",
    "GKLocalPlayer.load_friends_with_identifiers",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_7_args,
    2
},
{
    "load_recent_friends",
    "GKLocalPlayer.load_recent_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_8_args,
    1
},
{
    "register_listener",
    "GKLocalPlayer.register_listener",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "resolve_conflicting_saved_games",
    "GKLocalPlayer.resolve_conflicting_saved_games",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_10_args,
    3
},
{
    "save_game_data",
    "GKLocalPlayer.save_game_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_11_args,
    3
},
{
    "set_default_leaderboard_identifier",
    "GKLocalPlayer.set_default_leaderboard_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_49_method_12_args,
    2
},
{
    "unregister_listener",
    "GKLocalPlayer.unregister_listener",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_authenticated",
    "GKLocalPlayer.get_is_authenticated",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_multiplayer_gaming_restricted",
    "GKLocalPlayer.get_is_multiplayer_gaming_restricted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_personalized_communication_restricted",
    "GKLocalPlayer.get_is_personalized_communication_restricted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_underage",
    "GKLocalPlayer.get_is_underage",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_49_properties[] = {
{
    "is_authenticated",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_authenticated"
},
{
    "is_multiplayer_gaming_restricted",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_multiplayer_gaming_restricted"
},
{
    "is_personalized_communication_restricted",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_personalized_communication_restricted"
},
{
    "is_underage",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_underage"
},
};
static const GAPStubSignalDescriptor gap_class_49_signals[] = {
{
    "challenge_completed",
    gap_class_49_signal_0_args,
    3
},
{
    "challenge_other_player_accepted",
    gap_class_49_signal_1_args,
    2
},
{
    "challenge_other_player_completed",
    gap_class_49_signal_2_args,
    3
},
{
    "challenge_received",
    gap_class_49_signal_3_args,
    2
},
{
    "conflicting_saved_games",
    gap_class_49_signal_4_args,
    2
},
{
    "exchange_canceled",
    gap_class_49_signal_5_args,
    3
},
{
    "exchange_completed",
    gap_class_49_signal_6_args,
    3
},
{
    "exchange_received",
    gap_class_49_signal_7_args,
    3
},
{
    "invite_accepted",
    gap_class_49_signal_8_args,
    2
},
{
    "match_requested_with_other_players",
    gap_class_49_signal_9_args,
    2
},
{
    "player_wants_to_quit_match",
    gap_class_49_signal_10_args,
    2
},
{
    "saved_game_modified",
    gap_class_49_signal_11_args,
    2
},
{
    "turn_based_match_ended",
    gap_class_49_signal_12_args,
    2
},
{
    "turn_event_received",
    gap_class_49_signal_13_args,
    3
},
};
static const GAPStubNamedTypeInfo gap_class_50_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_method_3_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "toPlayers", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "dataMode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_method_4_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "dataMode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_method_5_args[] = {
    { "channel", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_method_11_args[] = {
    { "should_reinvite_disconnected_player", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_50_signal_0_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_signal_1_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "recipient", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "from_remote_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_50_signal_3_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "connected", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_50_methods[] = {
{
    "choose_best_hosting_player",
    "GKMatch.choose_best_hosting_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_0_args,
    1
},
{
    "disconnect",
    "GKMatch.disconnect",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "rematch",
    "GKMatch.rematch",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_2_args,
    1
},
{
    "send",
    "GKMatch.send",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_3_args,
    3
},
{
    "send_data_to_all_players",
    "GKMatch.send_data_to_all_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_4_args,
    2
},
{
    "voice_chat",
    "GKMatch.voice_chat",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKVoiceChat", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_5_args,
    1
},
{
    "get_expected_player_count",
    "GKMatch.get_expected_player_count",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_player_properties",
    "GKMatch.get_player_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_players",
    "GKMatch.get_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_properties",
    "GKMatch.get_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_should_reinvite_disconnected_player",
    "GKMatch.get_should_reinvite_disconnected_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "set_should_reinvite_disconnected_player",
    "GKMatch.set_should_reinvite_disconnected_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_50_method_11_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_50_properties[] = {
{
    "expected_player_count",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_expected_player_count"
},
{
    "player_properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player_properties"
},
{
    "players",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_players"
},
{
    "properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_properties"
},
{
    "should_reinvite_disconnected_player",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "set_should_reinvite_disconnected_player",
    "get_should_reinvite_disconnected_player"
},
};
static const GAPStubSignalDescriptor gap_class_50_signals[] = {
{
    "data_received",
    gap_class_50_signal_0_args,
    2
},
{
    "data_received_for_recipient_from_player",
    gap_class_50_signal_1_args,
    3
},
{
    "did_fail_with_error",
    gap_class_50_signal_2_args,
    1
},
{
    "player_changed",
    gap_class_50_signal_3_args,
    2
},
};
static const GAPStubConstantDescriptor gap_class_50_constants[] = {
{ "SendDataMode", "RELIABLE", 0, 0 },
{ "SendDataMode", "UNRELIABLE", 1, 0 },
};
static const GAPStubNamedTypeInfo gap_class_51_method_0_args[] = {
    { "forType", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_2_args[] = {
    { "default_number_of_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_4_args[] = {
    { "invite_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_6_args[] = {
    { "max_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_8_args[] = {
    { "min_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_10_args[] = {
    { "player_attributes", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_12_args[] = {
    { "player_group", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_14_args[] = {
    { "properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_16_args[] = {
    { "queue_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_18_args[] = {
    { "recipient_properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_20_args[] = {
    { "recipient_response", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_51_method_22_args[] = {
    { "recipients", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_51_methods[] = {
{
    "max_players_allowed_for_match",
    "GKMatchRequest.max_players_allowed_for_match",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_0_args,
    1
},
{
    "get_default_number_of_players",
    "GKMatchRequest.get_default_number_of_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_default_number_of_players",
    "GKMatchRequest.set_default_number_of_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_2_args,
    1
},
{
    "get_invite_message",
    "GKMatchRequest.get_invite_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_invite_message",
    "GKMatchRequest.set_invite_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_4_args,
    1
},
{
    "get_max_players",
    "GKMatchRequest.get_max_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_max_players",
    "GKMatchRequest.set_max_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_6_args,
    1
},
{
    "get_min_players",
    "GKMatchRequest.get_min_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_min_players",
    "GKMatchRequest.set_min_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_8_args,
    1
},
{
    "get_player_attributes",
    "GKMatchRequest.get_player_attributes",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_player_attributes",
    "GKMatchRequest.set_player_attributes",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_10_args,
    1
},
{
    "get_player_group",
    "GKMatchRequest.get_player_group",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_player_group",
    "GKMatchRequest.set_player_group",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_12_args,
    1
},
{
    "get_properties",
    "GKMatchRequest.get_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_properties",
    "GKMatchRequest.set_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_14_args,
    1
},
{
    "get_queue_name",
    "GKMatchRequest.get_queue_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_queue_name",
    "GKMatchRequest.set_queue_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_16_args,
    1
},
{
    "get_recipient_properties",
    "GKMatchRequest.get_recipient_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_recipient_properties",
    "GKMatchRequest.set_recipient_properties",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_18_args,
    1
},
{
    "get_recipient_response",
    "GKMatchRequest.get_recipient_response",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "set_recipient_response",
    "GKMatchRequest.set_recipient_response",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_20_args,
    1
},
{
    "get_recipients",
    "GKMatchRequest.get_recipients",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_recipients",
    "GKMatchRequest.set_recipients",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_51_method_22_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_51_properties[] = {
{
    "default_number_of_players",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_default_number_of_players",
    "get_default_number_of_players"
},
{
    "invite_message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_invite_message",
    "get_invite_message"
},
{
    "max_players",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_max_players",
    "get_max_players"
},
{
    "min_players",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_min_players",
    "get_min_players"
},
{
    "player_attributes",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_player_attributes",
    "get_player_attributes"
},
{
    "player_group",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_player_group",
    "get_player_group"
},
{
    "properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_properties",
    "get_properties"
},
{
    "queue_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_queue_name",
    "get_queue_name"
},
{
    "recipient_properties",
    { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_recipient_properties",
    "get_recipient_properties"
},
{
    "recipient_response",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "set_recipient_response",
    "get_recipient_response"
},
{
    "recipients",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_recipients",
    "get_recipients"
},
};
static const GAPStubConstantDescriptor gap_class_51_constants[] = {
{ "MatchType", "PEER_TO_PEER", 0, 0 },
{ "MatchType", "HOSTED", 1, 0 },
{ "MatchType", "TURN_BASED", 2, 0 },
{ "InviteRecipientResponse", "ACCEPTED", 0, 0 },
{ "InviteRecipientResponse", "DECLINED", 1, 0 },
{ "InviteRecipientResponse", "FAILED", 2, 0 },
{ "InviteRecipientResponse", "INCOMPATIBLE", 3, 0 },
{ "InviteRecipientResponse", "UNABLE_TO_CONNECT", 4, 0 },
{ "InviteRecipientResponse", "NO_ANSWER", 5, 0 },
{ "InviteRecipientResponse", "UNKNOWN", 6, 0 },
};
static const GAPStubNamedTypeInfo gap_class_52_method_0_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_2_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_3_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_4_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_5_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_6_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_7_args[] = {
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_9_args[] = {
    { "groupID", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_10_args[] = {
    { "queueName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "is_reachable", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_signal_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_52_methods[] = {
{
    "add_players",
    "GKMatchmaker.add_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_0_args,
    3
},
{
    "cancel",
    "GKMatchmaker.cancel",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "cancel_pending_invite",
    "GKMatchmaker.cancel_pending_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_2_args,
    1
},
{
    "find_match",
    "GKMatchmaker.find_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_3_args,
    2
},
{
    "find_matched_players",
    "GKMatchmaker.find_matched_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_4_args,
    2
},
{
    "find_players",
    "GKMatchmaker.find_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_5_args,
    2
},
{
    "finish_matchmaking",
    "GKMatchmaker.finish_matchmaking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_6_args,
    1
},
{
    "match_for_invite",
    "GKMatchmaker.match_for_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_7_args,
    2
},
{
    "query_activity",
    "GKMatchmaker.query_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_8_args,
    1
},
{
    "query_player_group_activity",
    "GKMatchmaker.query_player_group_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_9_args,
    2
},
{
    "query_queue_activity",
    "GKMatchmaker.query_queue_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_52_method_10_args,
    2
},
{
    "start_browsing_for_nearby_players",
    "GKMatchmaker.start_browsing_for_nearby_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "start_group_activity",
    "GKMatchmaker.start_group_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "stop_browsing_for_nearby_players",
    "GKMatchmaker.stop_browsing_for_nearby_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "stop_group_activity",
    "GKMatchmaker.stop_group_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubSignalDescriptor gap_class_52_signals[] = {
{
    "nearby_player_reachable",
    gap_class_52_signal_0_args,
    2
},
{
    "player_joining_group_activity",
    gap_class_52_signal_1_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_53_method_0_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_1_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_2_args[] = {
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_4_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "didConnect", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_7_args[] = {
    { "can_start_with_minimum_players", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_9_args[] = {
    { "get_match_properties_for_recipient", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_11_args[] = {
    { "is_hosted", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_14_args[] = {
    { "matchmaking_mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_signal_0_args[] = {
    { "detail", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_signal_1_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_signal_2_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_signal_3_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_signal_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_53_methods[] = {
{
    "add_players_to_match",
    "GKMatchmakerViewController.add_players_to_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_0_args,
    1
},
{
    "create_controller",
    "GKMatchmakerViewController.create_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_1_args,
    1
},
{
    "create_controller_from_invite",
    "GKMatchmakerViewController.create_controller_from_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_2_args,
    1
},
{
    "present",
    "GKMatchmakerViewController.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "request_match",
    "GKMatchmakerViewController.request_match",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_4_args,
    2
},
{
    "set_hosted_player_did_connect",
    "GKMatchmakerViewController.set_hosted_player_did_connect",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_5_args,
    2
},
{
    "get_can_start_with_minimum_players",
    "GKMatchmakerViewController.get_can_start_with_minimum_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_can_start_with_minimum_players",
    "GKMatchmakerViewController.set_can_start_with_minimum_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_7_args,
    1
},
{
    "get_get_match_properties_for_recipient",
    "GKMatchmakerViewController.get_get_match_properties_for_recipient",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    NULL,
    0
},
{
    "set_get_match_properties_for_recipient",
    "GKMatchmakerViewController.set_get_match_properties_for_recipient",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_9_args,
    1
},
{
    "get_is_hosted",
    "GKMatchmakerViewController.get_is_hosted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_hosted",
    "GKMatchmakerViewController.set_is_hosted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_11_args,
    1
},
{
    "get_match_request",
    "GKMatchmakerViewController.get_match_request",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_matchmaking_mode",
    "GKMatchmakerViewController.get_matchmaking_mode",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_matchmaking_mode",
    "GKMatchmakerViewController.set_matchmaking_mode",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_53_method_14_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_53_properties[] = {
{
    "can_start_with_minimum_players",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_can_start_with_minimum_players",
    "get_can_start_with_minimum_players"
},
{
    "get_match_properties_for_recipient",
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT },
    "set_get_match_properties_for_recipient",
    "get_get_match_properties_for_recipient"
},
{
    "is_hosted",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_hosted",
    "get_is_hosted"
},
{
    "match_request",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_match_request"
},
{
    "matchmaking_mode",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_matchmaking_mode",
    "get_matchmaking_mode"
},
};
static const GAPStubSignalDescriptor gap_class_53_signals[] = {
{
    "cancelled",
    gap_class_53_signal_0_args,
    1
},
{
    "did_find_hosted_players",
    gap_class_53_signal_1_args,
    1
},
{
    "did_find_match",
    gap_class_53_signal_2_args,
    1
},
{
    "failed_with_error",
    gap_class_53_signal_3_args,
    1
},
{
    "hosted_player_did_accept",
    gap_class_53_signal_4_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_54_method_0_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_54_method_1_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "duration", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_54_methods[] = {
{
    "show",
    "GKNotificationBanner.show",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_54_method_0_args,
    2
},
{
    "show_with_duration",
    "GKNotificationBanner.show_with_duration",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_54_method_1_args,
    3
},
};
static const GAPStubNamedTypeInfo gap_class_55_method_0_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_55_methods[] = {
{
    "load_data",
    "GKSavedGame.load_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_55_method_0_args,
    1
},
{
    "get_device_name",
    "GKSavedGame.get_device_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_modification_date",
    "GKSavedGame.get_modification_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_name",
    "GKSavedGame.get_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_55_properties[] = {
{
    "device_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_device_name"
},
{
    "modification_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_modification_date"
},
{
    "name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_name"
},
};
static const GAPStubMethodDescriptor gap_class_56_methods[] = {
{
    "get_context",
    "GKScoreChallenge.get_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_formatted_score",
    "GKScoreChallenge.get_formatted_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_leaderboard_entry",
    "GKScoreChallenge.get_leaderboard_entry",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboardEntry", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_leaderboard_identifier",
    "GKScoreChallenge.get_leaderboard_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_player",
    "GKScoreChallenge.get_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_rank",
    "GKScoreChallenge.get_rank",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_score",
    "GKScoreChallenge.get_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_56_properties[] = {
{
    "context",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_context"
},
{
    "formatted_score",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_formatted_score"
},
{
    "leaderboard_entry",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboardEntry", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_leaderboard_entry"
},
{
    "leaderboard_identifier",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_leaderboard_identifier"
},
{
    "player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player"
},
{
    "rank",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_rank"
},
{
    "score",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_score"
},
};
static const GAPStubNamedTypeInfo gap_class_57_method_0_args[] = {
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_57_method_1_args[] = {
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_57_methods[] = {
{
    "cancel",
    "GKTurnBasedExchange.cancel",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_57_method_0_args,
    3
},
{
    "reply",
    "GKTurnBasedExchange.reply",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_57_method_1_args,
    4
},
{
    "get_completion_date",
    "GKTurnBasedExchange.get_completion_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_data",
    "GKTurnBasedExchange.get_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exchange_id",
    "GKTurnBasedExchange.get_exchange_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_message",
    "GKTurnBasedExchange.get_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_recipients",
    "GKTurnBasedExchange.get_recipients",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_replies",
    "GKTurnBasedExchange.get_replies",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_send_date",
    "GKTurnBasedExchange.get_send_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_sender",
    "GKTurnBasedExchange.get_sender",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_status",
    "GKTurnBasedExchange.get_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_timeout_date",
    "GKTurnBasedExchange.get_timeout_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_57_properties[] = {
{
    "completion_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_completion_date"
},
{
    "data",
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_data"
},
{
    "exchange_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exchange_id"
},
{
    "message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_message"
},
{
    "recipients",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_recipients"
},
{
    "replies",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_replies"
},
{
    "send_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_send_date"
},
{
    "sender",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_sender"
},
{
    "status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_status"
},
{
    "timeout_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_timeout_date"
},
};
static const GAPStubMethodDescriptor gap_class_58_methods[] = {
{
    "get_data",
    "GKTurnBasedExchangeReply.get_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_message",
    "GKTurnBasedExchangeReply.get_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_recipient",
    "GKTurnBasedExchangeReply.get_recipient",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_reply_date",
    "GKTurnBasedExchangeReply.get_reply_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_58_properties[] = {
{
    "data",
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_data"
},
{
    "message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_message"
},
{
    "recipient",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_recipient"
},
{
    "reply_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_reply_date"
},
};
static const GAPStubNamedTypeInfo gap_class_59_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_2_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_3_args[] = {
    { "nextParticipants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_6_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_7_args[] = {
    { "matchID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_9_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_10_args[] = {
    { "outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "nextParticipants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_11_args[] = {
    { "outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_12_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_13_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_14_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_15_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "resolvedExchanges", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_16_args[] = {
    { "participants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_17_args[] = {
    { "participants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_59_method_18_args[] = {
    { "key", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_59_methods[] = {
{
    "accept_invite",
    "GKTurnBasedMatch.accept_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_0_args,
    1
},
{
    "decline_invite",
    "GKTurnBasedMatch.decline_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_1_args,
    1
},
{
    "end_match_in_turn",
    "GKTurnBasedMatch.end_match_in_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_2_args,
    2
},
{
    "end_turn",
    "GKTurnBasedMatch.end_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_3_args,
    4
},
{
    "exchange_timeout_default",
    "GKTurnBasedMatch.exchange_timeout_default",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "exchange_timeout_none",
    "GKTurnBasedMatch.exchange_timeout_none",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "find",
    "GKTurnBasedMatch.find",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_6_args,
    2
},
{
    "load",
    "GKTurnBasedMatch.load",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_7_args,
    2
},
{
    "load_match_data",
    "GKTurnBasedMatch.load_match_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_8_args,
    1
},
{
    "load_matches",
    "GKTurnBasedMatch.load_matches",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_9_args,
    1
},
{
    "participant_quit_in_turn",
    "GKTurnBasedMatch.participant_quit_in_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_10_args,
    5
},
{
    "participant_quit_out_of_turn",
    "GKTurnBasedMatch.participant_quit_out_of_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_11_args,
    2
},
{
    "rematch",
    "GKTurnBasedMatch.rematch",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_12_args,
    1
},
{
    "remove",
    "GKTurnBasedMatch.remove",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_13_args,
    1
},
{
    "save_current_turn",
    "GKTurnBasedMatch.save_current_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_14_args,
    2
},
{
    "save_merged_match",
    "GKTurnBasedMatch.save_merged_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_15_args,
    3
},
{
    "send_exchange",
    "GKTurnBasedMatch.send_exchange",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_16_args,
    6
},
{
    "send_reminder",
    "GKTurnBasedMatch.send_reminder",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_17_args,
    4
},
{
    "set_localizable_message_with_key",
    "GKTurnBasedMatch.set_localizable_message_with_key",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_59_method_18_args,
    2
},
{
    "turn_timeout_default",
    "GKTurnBasedMatch.turn_timeout_default",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "turn_timeout_none",
    "GKTurnBasedMatch.turn_timeout_none",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_active_exchanges",
    "GKTurnBasedMatch.get_active_exchanges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_completed_exchanges",
    "GKTurnBasedMatch.get_completed_exchanges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_creation_date",
    "GKTurnBasedMatch.get_creation_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_current_participant",
    "GKTurnBasedMatch.get_current_participant",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exchange_data_maximum_size",
    "GKTurnBasedMatch.get_exchange_data_maximum_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exchange_max_initiated_exchanges_per_player",
    "GKTurnBasedMatch.get_exchange_max_initiated_exchanges_per_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_exchanges",
    "GKTurnBasedMatch.get_exchanges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_active_player",
    "GKTurnBasedMatch.get_is_active_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_match_data",
    "GKTurnBasedMatch.get_match_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_match_data_maximum_size",
    "GKTurnBasedMatch.get_match_data_maximum_size",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_match_id",
    "GKTurnBasedMatch.get_match_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_message",
    "GKTurnBasedMatch.get_message",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_participants",
    "GKTurnBasedMatch.get_participants",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_status",
    "GKTurnBasedMatch.get_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_59_properties[] = {
{
    "active_exchanges",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_active_exchanges"
},
{
    "completed_exchanges",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_completed_exchanges"
},
{
    "creation_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_creation_date"
},
{
    "current_participant",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedParticipant", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_current_participant"
},
{
    "exchange_data_maximum_size",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exchange_data_maximum_size"
},
{
    "exchange_max_initiated_exchanges_per_player",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exchange_max_initiated_exchanges_per_player"
},
{
    "exchanges",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_exchanges"
},
{
    "is_active_player",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_active_player"
},
{
    "match_data",
    { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_match_data"
},
{
    "match_data_maximum_size",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_match_data_maximum_size"
},
{
    "match_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_match_id"
},
{
    "message",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_message"
},
{
    "participants",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_participants"
},
{
    "status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_status"
},
};
static const GAPStubNamedTypeInfo gap_class_60_method_0_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_method_2_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_method_4_args[] = {
    { "matchmaking_mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_method_6_args[] = {
    { "show_existing_matches", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_signal_0_args[] = {
    { "detail", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_signal_1_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_60_signal_3_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_60_methods[] = {
{
    "create_controller",
    "GKTurnBasedMatchmakerViewController.create_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_60_method_0_args,
    1
},
{
    "present",
    "GKTurnBasedMatchmakerViewController.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "request_match",
    "GKTurnBasedMatchmakerViewController.request_match",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_60_method_2_args,
    2
},
{
    "get_matchmaking_mode",
    "GKTurnBasedMatchmakerViewController.get_matchmaking_mode",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_matchmaking_mode",
    "GKTurnBasedMatchmakerViewController.set_matchmaking_mode",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_60_method_4_args,
    1
},
{
    "get_show_existing_matches",
    "GKTurnBasedMatchmakerViewController.get_show_existing_matches",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_show_existing_matches",
    "GKTurnBasedMatchmakerViewController.set_show_existing_matches",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_60_method_6_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_60_properties[] = {
{
    "matchmaking_mode",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_matchmaking_mode",
    "get_matchmaking_mode"
},
{
    "show_existing_matches",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_show_existing_matches",
    "get_show_existing_matches"
},
};
static const GAPStubSignalDescriptor gap_class_60_signals[] = {
{
    "cancelled",
    gap_class_60_signal_0_args,
    1
},
{
    "did_find_match",
    gap_class_60_signal_1_args,
    1
},
{
    "failed_with_error",
    gap_class_60_signal_2_args,
    1
},
{
    "player_quit_for_match",
    gap_class_60_signal_3_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_60_constants[] = {
{ "MatchmakingMode", "DEFAULT", 0, 0 },
{ "MatchmakingMode", "NEARBY_ONLY", 1, 0 },
{ "MatchmakingMode", "AUTOMATCH_ONLY", 2, 0 },
{ "MatchmakingMode", "INVITE_ONLY", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_61_method_2_args[] = {
    { "match_outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_61_methods[] = {
{
    "get_last_turn_date",
    "GKTurnBasedParticipant.get_last_turn_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_match_outcome",
    "GKTurnBasedParticipant.get_match_outcome",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_match_outcome",
    "GKTurnBasedParticipant.set_match_outcome",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_61_method_2_args,
    1
},
{
    "get_player",
    "GKTurnBasedParticipant.get_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_status",
    "GKTurnBasedParticipant.get_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_timeout_date",
    "GKTurnBasedParticipant.get_timeout_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_61_properties[] = {
{
    "last_turn_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_last_turn_date"
},
{
    "match_outcome",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_match_outcome",
    "get_match_outcome"
},
{
    "player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_player"
},
{
    "status",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_status"
},
{
    "timeout_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_timeout_date"
},
};
static const GAPStubNamedTypeInfo gap_class_62_method_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "muted", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_62_method_5_args[] = {
    { "is_active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_62_method_9_args[] = {
    { "volume", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_62_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "state", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_62_methods[] = {
{
    "is_voip_allowed",
    "GKVoiceChat.is_voip_allowed",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_player_muted",
    "GKVoiceChat.set_player_muted",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_62_method_1_args,
    2
},
{
    "start",
    "GKVoiceChat.start",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "stop",
    "GKVoiceChat.stop",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_active",
    "GKVoiceChat.get_is_active",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_is_active",
    "GKVoiceChat.set_is_active",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_62_method_5_args,
    1
},
{
    "get_name",
    "GKVoiceChat.get_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_players",
    "GKVoiceChat.get_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_volume",
    "GKVoiceChat.get_volume",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_volume",
    "GKVoiceChat.set_volume",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_62_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_62_properties[] = {
{
    "is_active",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_is_active",
    "get_is_active"
},
{
    "name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_name"
},
{
    "players",
    { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_players"
},
{
    "volume",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_volume",
    "get_volume"
},
};
static const GAPStubSignalDescriptor gap_class_62_signals[] = {
{
    "player_state_changed",
    gap_class_62_signal_0_args,
    2
},
};
static const GAPStubNamedTypeInfo gap_class_63_method_3_args[] = {
    { "local_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLocalPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_63_signal_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_63_signal_1_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_63_methods[] = {
{
    "authenticate",
    "GameCenterManager.authenticate",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_access_point",
    "GameCenterManager.get_access_point",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAccessPoint", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_local_player",
    "GameCenterManager.get_local_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLocalPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_local_player",
    "GameCenterManager.set_local_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_63_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_63_properties[] = {
{
    "access_point",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAccessPoint", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_access_point"
},
{
    "local_player",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLocalPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_local_player",
    "get_local_player"
},
};
static const GAPStubSignalDescriptor gap_class_63_signals[] = {
{
    "authentication_error",
    gap_class_63_signal_0_args,
    1
},
{
    "authentication_result",
    gap_class_63_signal_1_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_64_method_3_args[] = {
    { "prefers_promotional_icon", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_64_method_5_args[] = {
    { "product_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_64_method_7_args[] = {
    { "style", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_64_method_9_args[] = {
    { "system_icon_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_64_methods[] = {
{
    "dismiss",
    "ProductView.dismiss",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "present",
    "ProductView.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_prefers_promotional_icon",
    "ProductView.get_prefers_promotional_icon",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_prefers_promotional_icon",
    "ProductView.set_prefers_promotional_icon",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_64_method_3_args,
    1
},
{
    "get_product_id",
    "ProductView.get_product_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_product_id",
    "ProductView.set_product_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_64_method_5_args,
    1
},
{
    "get_style",
    "ProductView.get_style",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_style",
    "ProductView.set_style",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_64_method_7_args,
    1
},
{
    "get_system_icon_name",
    "ProductView.get_system_icon_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_system_icon_name",
    "ProductView.set_system_icon_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_64_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_64_properties[] = {
{
    "prefers_promotional_icon",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_prefers_promotional_icon",
    "get_prefers_promotional_icon"
},
{
    "product_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_product_id",
    "get_product_id"
},
{
    "style",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_style",
    "get_style"
},
{
    "system_icon_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_system_icon_name",
    "get_system_icon_name"
},
};
static const GAPStubConstantDescriptor gap_class_64_constants[] = {
{ "ViewStyle", "AUTOMATIC", 0, 0 },
{ "ViewStyle", "COMPACT", 1, 0 },
{ "ViewStyle", "LARGE", 2, 0 },
{ "ViewStyle", "REGULAR", 3, 0 },
};
static const GAPStubMethodDescriptor gap_class_65_methods[] = {
{
    "proxy",
    "SignalProxy.proxy",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubNamedTypeInfo gap_class_66_method_1_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_method_2_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_method_3_args[] = {
    { "productIds", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_0_args[] = {
    { "products", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_1_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "error_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_2_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_3_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "error_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_4_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreSubscriptionInfoStatus", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_5_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_66_signal_6_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "verification_error", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_66_methods[] = {
{
    "fetch_current_entitlements",
    "StoreKitManager.fetch_current_entitlements",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "purchase",
    "StoreKitManager.purchase",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_66_method_1_args,
    1
},
{
    "purchase_with_options",
    "StoreKitManager.purchase_with_options",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_66_method_2_args,
    2
},
{
    "request_products",
    "StoreKitManager.request_products",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_66_method_3_args,
    1
},
{
    "restore_purchases",
    "StoreKitManager.restore_purchases",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "start",
    "StoreKitManager.start",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubSignalDescriptor gap_class_66_signals[] = {
{
    "products_request_completed",
    gap_class_66_signal_0_args,
    2
},
{
    "purchase_completed",
    gap_class_66_signal_1_args,
    3
},
{
    "purchase_intent",
    gap_class_66_signal_2_args,
    1
},
{
    "restore_completed",
    gap_class_66_signal_3_args,
    2
},
{
    "supscription_update",
    gap_class_66_signal_4_args,
    1
},
{
    "transaction_updated",
    gap_class_66_signal_5_args,
    1
},
{
    "unverified_transaction_updated",
    gap_class_66_signal_6_args,
    2
},
};
static const GAPStubConstantDescriptor gap_class_66_constants[] = {
{ "StoreKitStatus", "OK", 0, 0 },
{ "StoreKitStatus", "INVALID_PRODUCT", 1, 0 },
{ "StoreKitStatus", "CANCELLED", 2, 0 },
{ "StoreKitStatus", "UNVERIFIED_TRANSACTION", 3, 0 },
{ "StoreKitStatus", "USER_CANCELLED", 4, 0 },
{ "StoreKitStatus", "PURCHASE_PENDING", 5, 0 },
{ "StoreKitStatus", "UNKNOWN_STATUS", 6, 0 },
{ "VerificationError", "REVOKED_CERTIFICATE", 0, 0 },
{ "VerificationError", "INVALID_CERTIFICATE_CHAIN", 1, 0 },
{ "VerificationError", "INVALID_DEVICE_VERIFICATION", 2, 0 },
{ "VerificationError", "INVALID_ENCODING", 3, 0 },
{ "VerificationError", "INVALID_SIGNATURE", 4, 0 },
{ "VerificationError", "MISSING_REQUIRED_PROPERTIES", 5, 0 },
{ "VerificationError", "OTHER", 6, 0 },
};
static const GAPStubMethodDescriptor gap_class_67_methods[] = {
{
    "get_description_value",
    "StoreProduct.get_description_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_display_name",
    "StoreProduct.get_display_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_display_price",
    "StoreProduct.get_display_price",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_family_shareable",
    "StoreProduct.get_is_family_shareable",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_json_representation",
    "StoreProduct.get_json_representation",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_price",
    "StoreProduct.get_price",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_product_id",
    "StoreProduct.get_product_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_67_properties[] = {
{
    "description_value",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_description_value"
},
{
    "display_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_display_name"
},
{
    "display_price",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_display_price"
},
{
    "is_family_shareable",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_family_shareable"
},
{
    "json_representation",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_json_representation"
},
{
    "price",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_price"
},
{
    "product_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_product_id"
},
};
static const GAPStubMethodDescriptor gap_class_68_methods[] = {
{
    "get_free_trial",
    "StoreProductPaymentMode.get_free_trial",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPaymentMode", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_pay_as_you_go",
    "StoreProductPaymentMode.get_pay_as_you_go",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPaymentMode", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "pay_up_front",
    "StoreProductPaymentMode.pay_up_front",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPaymentMode", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_localized_description",
    "StoreProductPaymentMode.get_localized_description",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_68_properties[] = {
{
    "localized_description",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_localized_description"
},
};
static const GAPStubNamedTypeInfo gap_class_69_method_0_args[] = {
    { "stringUuidToken", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_69_method_1_args[] = {
    { "jws", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_69_method_2_args[] = {
    { "value", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_69_method_3_args[] = {
    { "enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_69_method_4_args[] = {
    { "offer", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductSubscriptionOffer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_69_methods[] = {
{
    "app_account_token",
    "StoreProductPurchaseOption.app_account_token",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_69_method_0_args,
    1
},
{
    "introductory_offer_elligibility",
    "StoreProductPurchaseOption.introductory_offer_elligibility",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_69_method_1_args,
    1
},
{
    "quantity",
    "StoreProductPurchaseOption.quantity",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_69_method_2_args,
    1
},
{
    "simulate_ask_to_buy_in_sandbox",
    "StoreProductPurchaseOption.simulate_ask_to_buy_in_sandbox",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_69_method_3_args,
    1
},
{
    "win_back_offer",
    "StoreProductPurchaseOption.win_back_offer",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_69_method_4_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_70_constants[] = {
{ "OfferType", "INTRODUCTORY", 0, 0 },
{ "OfferType", "PROMOTIONAL", 1, 0 },
{ "OfferType", "WIN_BACK", 2, 0 },
{ "OfferType", "UNKNOWN", 3, 0 },
};
static const GAPStubConstantDescriptor gap_class_71_constants[] = {
{ "Unit", "DAY", 0, 0 },
{ "Unit", "MONTH", 1, 0 },
{ "Unit", "WEEK", 2, 0 },
{ "Unit", "YEAR", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_72_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_72_method_1_args[] = {
    { "group_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_72_method_2_args[] = {
    { "transaction_id", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_72_methods[] = {
{
    "getStatus",
    "StoreSubscriptionInfo.getStatus",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_72_method_0_args,
    1
},
{
    "status_for_group_id",
    "StoreSubscriptionInfo.status_for_group_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_72_method_1_args,
    2
},
{
    "status_for_transaction",
    "StoreSubscriptionInfo.status_for_transaction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_72_method_2_args,
    2
},
{
    "get_group_display_name",
    "StoreSubscriptionInfo.get_group_display_name",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_group_level",
    "StoreSubscriptionInfo.get_group_level",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_subscription_group_id",
    "StoreSubscriptionInfo.get_subscription_group_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_72_properties[] = {
{
    "group_display_name",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_display_name"
},
{
    "group_level",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_group_level"
},
{
    "subscription_group_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_subscription_group_id"
},
};
static const GAPStubMethodDescriptor gap_class_73_methods[] = {
{
    "get_app_account_token",
    "StoreSubscriptionInfoRenewalInfo.get_app_account_token",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_app_transaction_id",
    "StoreSubscriptionInfoRenewalInfo.get_app_transaction_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_current_product_id",
    "StoreSubscriptionInfoRenewalInfo.get_current_product_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_original_transaction_id",
    "StoreSubscriptionInfoRenewalInfo.get_original_transaction_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_73_properties[] = {
{
    "app_account_token",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_app_account_token"
},
{
    "app_transaction_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_app_transaction_id"
},
{
    "current_product_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_current_product_id"
},
{
    "original_transaction_id",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_original_transaction_id"
},
};
static const GAPStubMethodDescriptor gap_class_74_methods[] = {
{
    "get_renewal_info",
    "StoreSubscriptionInfoStatus.get_renewal_info",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreSubscriptionInfoRenewalInfo", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_state",
    "StoreSubscriptionInfoStatus.get_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_transaction",
    "StoreSubscriptionInfoStatus.get_transaction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_74_properties[] = {
{
    "renewal_info",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreSubscriptionInfoRenewalInfo", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_renewal_info"
},
{
    "state",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_state"
},
{
    "transaction",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_transaction"
},
};
static const GAPStubConstantDescriptor gap_class_74_constants[] = {
{ "RenewalState", "UNKNOWN", 0, 0 },
{ "RenewalState", "EXPIRED", 1, 0 },
{ "RenewalState", "SUBSCRIBED", 2, 0 },
{ "RenewalState", "IN_BILLING_RETRY_PERIOD", 3, 0 },
{ "RenewalState", "IN_GRACE_PERIOD", 4, 0 },
{ "RenewalState", "REVOKED", 5, 0 },
};
static const GAPStubMethodDescriptor gap_class_75_methods[] = {
{
    "finish",
    "StoreTransaction.finish",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_expiration_date",
    "StoreTransaction.get_expiration_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_is_upgraded",
    "StoreTransaction.get_is_upgraded",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_original_id",
    "StoreTransaction.get_original_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_ownership_type",
    "StoreTransaction.get_ownership_type",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_product_id",
    "StoreTransaction.get_product_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_purchase_date",
    "StoreTransaction.get_purchase_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_revocation_date",
    "StoreTransaction.get_revocation_date",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_transaction_id",
    "StoreTransaction.get_transaction_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
};
static const GAPStubPropertyDescriptor gap_class_75_properties[] = {
{
    "expiration_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_expiration_date"
},
{
    "is_upgraded",
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_is_upgraded"
},
{
    "original_id",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_original_id"
},
{
    "ownership_type",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_ownership_type"
},
{
    "product_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_product_id"
},
{
    "purchase_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_purchase_date"
},
{
    "revocation_date",
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_revocation_date"
},
{
    "transaction_id",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_transaction_id"
},
};
static const GAPStubNamedTypeInfo gap_class_76_method_3_args[] = {
    { "product_ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_76_methods[] = {
{
    "dismiss",
    "StoreView.dismiss",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "present",
    "StoreView.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_product_ids",
    "StoreView.get_product_ids",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_product_ids",
    "StoreView.set_product_ids",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_76_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_76_properties[] = {
{
    "product_ids",
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_product_ids",
    "get_product_ids"
},
};
static const GAPStubNamedTypeInfo gap_class_77_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_77_method_3_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_77_signal_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_77_methods[] = {
{
    "dismiss",
    "SubscriptionOfferView.dismiss",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "present",
    "SubscriptionOfferView.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_77_method_1_args,
    1
},
{
    "get_title",
    "SubscriptionOfferView.get_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_title",
    "SubscriptionOfferView.set_title",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_77_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_77_properties[] = {
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_title",
    "get_title"
},
};
static const GAPStubSignalDescriptor gap_class_77_signals[] = {
{
    "error",
    gap_class_77_signal_0_args,
    1
},
{
    "success",
    NULL,
    0
},
};
static const GAPStubNamedTypeInfo gap_class_78_method_3_args[] = {
    { "control_style", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_78_method_5_args[] = {
    { "group_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_78_method_7_args[] = {
    { "product_i_ds", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_78_methods[] = {
{
    "dismiss",
    "SubscriptionStoreView.dismiss",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "present",
    "SubscriptionStoreView.present",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "get_control_style",
    "SubscriptionStoreView.get_control_style",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_control_style",
    "SubscriptionStoreView.set_control_style",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_78_method_3_args,
    1
},
{
    "get_group_id",
    "SubscriptionStoreView.get_group_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_group_id",
    "SubscriptionStoreView.set_group_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_78_method_5_args,
    1
},
{
    "get_product_i_ds",
    "SubscriptionStoreView.get_product_i_ds",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    NULL,
    0
},
{
    "set_product_i_ds",
    "SubscriptionStoreView.set_product_i_ds",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_78_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_78_properties[] = {
{
    "control_style",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_control_style",
    "get_control_style"
},
{
    "group_id",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_group_id",
    "get_group_id"
},
{
    "product_i_ds",
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_product_i_ds",
    "get_product_i_ds"
},
};
static const GAPStubConstantDescriptor gap_class_78_constants[] = {
{ "ControlStyle", "AUTOMATIC", 0, 0 },
{ "ControlStyle", "PICKER", 1, 0 },
{ "ControlStyle", "BUTTONS", 2, 0 },
{ "ControlStyle", "COMPACT_PICKER", 3, 0 },
{ "ControlStyle", "PROMINENT_PICKER", 4, 0 },
{ "ControlStyle", "PAGED_PICKER", 5, 0 },
{ "ControlStyle", "PAGED_PROMINENT_PICKER", 6, 0 },
};
static const GAPStubClassDescriptor gap_classes[] = {
{
    "ARAnchor",
    "RefCounted",
    gap_class_0_methods,
    7,
    gap_class_0_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "ARBodyAnchor",
    "RefCounted",
    gap_class_1_methods,
    10,
    gap_class_1_properties,
    5,
    NULL,
    0,
    NULL,
    0
},
{
    "ARBodySkeleton",
    "RefCounted",
    gap_class_2_methods,
    5,
    gap_class_2_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "ARBodyTrackingConfiguration",
    "RefCounted",
    gap_class_3_methods,
    7,
    gap_class_3_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "ARCamera",
    "RefCounted",
    gap_class_4_methods,
    14,
    gap_class_4_properties,
    10,
    NULL,
    0,
    gap_class_4_constants,
    8
},
{
    "ARCoachingOverlay",
    "RefCounted",
    gap_class_5_methods,
    6,
    gap_class_5_properties,
    2,
    gap_class_5_signals,
    1,
    gap_class_5_constants,
    5
},
{
    "ARCollaborationData",
    "RefCounted",
    gap_class_6_methods,
    4,
    gap_class_6_properties,
    1,
    NULL,
    0,
    gap_class_6_constants,
    2
},
{
    "AREnvironmentProbeAnchor",
    "RefCounted",
    gap_class_7_methods,
    12,
    gap_class_7_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "ARFaceAnchor",
    "RefCounted",
    gap_class_8_methods,
    19,
    gap_class_8_properties,
    9,
    NULL,
    0,
    gap_class_8_constants,
    52
},
{
    "ARFrame",
    "RefCounted",
    gap_class_9_methods,
    17,
    gap_class_9_properties,
    9,
    NULL,
    0,
    gap_class_9_constants,
    4
},
{
    "ARGeoAnchor",
    "RefCounted",
    gap_class_10_methods,
    12,
    gap_class_10_properties,
    5,
    NULL,
    0,
    gap_class_10_constants,
    4
},
{
    "ARGeoTrackingConfiguration",
    "RefCounted",
    gap_class_11_methods,
    13,
    gap_class_11_properties,
    5,
    NULL,
    0,
    NULL,
    0
},
{
    "ARHandAnchor",
    "RefCounted",
    gap_class_12_methods,
    10,
    gap_class_12_properties,
    5,
    NULL,
    0,
    gap_class_12_constants,
    2
},
{
    "ARHandSkeleton",
    "RefCounted",
    gap_class_13_methods,
    5,
    gap_class_13_properties,
    1,
    NULL,
    0,
    gap_class_13_constants,
    27
},
{
    "ARImageAnchor",
    "RefCounted",
    gap_class_14_methods,
    12,
    gap_class_14_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "ARLightEstimate",
    "RefCounted",
    gap_class_15_methods,
    11,
    gap_class_15_properties,
    5,
    NULL,
    0,
    NULL,
    0
},
{
    "ARMeshAnchor",
    "RefCounted",
    gap_class_16_methods,
    15,
    gap_class_16_properties,
    6,
    NULL,
    0,
    gap_class_16_constants,
    8
},
{
    "ARPlaneAnchor",
    "RefCounted",
    gap_class_17_methods,
    33,
    gap_class_17_properties,
    14,
    NULL,
    0,
    gap_class_17_constants,
    21
},
{
    "ARPointCloud",
    "RefCounted",
    gap_class_18_methods,
    8,
    gap_class_18_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "ARRaycastQuery",
    "RefCounted",
    gap_class_19_methods,
    9,
    gap_class_19_properties,
    4,
    NULL,
    0,
    gap_class_19_constants,
    6
},
{
    "ARRaycastResult",
    "RefCounted",
    gap_class_20_methods,
    8,
    gap_class_20_properties,
    4,
    NULL,
    0,
    gap_class_20_constants,
    6
},
{
    "ARSession",
    "RefCounted",
    gap_class_21_methods,
    21,
    gap_class_21_properties,
    4,
    gap_class_21_signals,
    13,
    gap_class_21_constants,
    4
},
{
    "ARTrackedRaycast",
    "RefCounted",
    gap_class_22_methods,
    3,
    gap_class_22_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "ARWorldMap",
    "RefCounted",
    gap_class_23_methods,
    10,
    gap_class_23_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "ARWorldTrackingConfiguration",
    "RefCounted",
    gap_class_24_methods,
    37,
    gap_class_24_properties,
    16,
    NULL,
    0,
    gap_class_24_constants,
    14
},
{
    "ASAuthorizationAppleIDCredential",
    "RefCounted",
    gap_class_25_methods,
    9,
    gap_class_25_properties,
    9,
    NULL,
    0,
    gap_class_25_constants,
    6
},
{
    "ASAuthorizationController",
    "RefCounted",
    gap_class_26_methods,
    2,
    NULL,
    0,
    gap_class_26_signals,
    2,
    NULL,
    0
},
{
    "ASPasswordCredential",
    "RefCounted",
    gap_class_27_methods,
    2,
    gap_class_27_properties,
    2,
    NULL,
    0,
    NULL,
    0
},
{
    "ASWebAuthenticationSession",
    "RefCounted",
    gap_class_28_methods,
    2,
    NULL,
    0,
    gap_class_28_signals,
    3,
    NULL,
    0
},
{
    "AVAudioSession",
    "RefCounted",
    gap_class_29_methods,
    3,
    gap_class_29_properties,
    1,
    NULL,
    0,
    gap_class_29_constants,
    27
},
{
    "AppleFilePicker",
    "RefCounted",
    gap_class_30_methods,
    1,
    NULL,
    0,
    gap_class_30_signals,
    3,
    NULL,
    0
},
{
    "AppleURL",
    "RefCounted",
    gap_class_31_methods,
    9,
    NULL,
    0,
    NULL,
    0,
    NULL,
    0
},
{
    "Foundation",
    "RefCounted",
    gap_class_32_methods,
    1,
    NULL,
    0,
    NULL,
    0,
    NULL,
    0
},
{
    "GKAccessPoint",
    "RefCounted",
    gap_class_33_methods,
    22,
    gap_class_33_properties,
    7,
    NULL,
    0,
    gap_class_33_constants,
    4
},
{
    "GKAchievement",
    "RefCounted",
    gap_class_34_methods,
    16,
    gap_class_34_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKChallenge",
    "RefCounted",
    gap_class_35_methods,
    9,
    gap_class_35_properties,
    7,
    NULL,
    0,
    gap_class_35_constants,
    7
},
{
    "GKAchievementChallenge",
    "GKChallenge",
    gap_class_36_methods,
    1,
    gap_class_36_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "GKAchievementDescription",
    "RefCounted",
    gap_class_37_methods,
    16,
    gap_class_37_properties,
    12,
    NULL,
    0,
    NULL,
    0
},
{
    "GKChallengeDefinition",
    "RefCounted",
    gap_class_38_methods,
    11,
    gap_class_38_properties,
    8,
    NULL,
    0,
    NULL,
    0
},
{
    "GKError",
    "RefCounted",
    gap_class_39_methods,
    6,
    gap_class_39_properties,
    3,
    NULL,
    0,
    gap_class_39_constants,
    42
},
{
    "GKGameActivity",
    "RefCounted",
    gap_class_40_methods,
    35,
    gap_class_40_properties,
    13,
    NULL,
    0,
    NULL,
    0
},
{
    "GKGameActivityDefinition",
    "RefCounted",
    gap_class_41_methods,
    17,
    gap_class_41_properties,
    12,
    NULL,
    0,
    NULL,
    0
},
{
    "GKGameCenterViewController",
    "RefCounted",
    gap_class_42_methods,
    6,
    NULL,
    0,
    NULL,
    0,
    gap_class_42_constants,
    6
},
{
    "GKInvite",
    "RefCounted",
    gap_class_43_methods,
    4,
    gap_class_43_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboard",
    "RefCounted",
    gap_class_44_methods,
    18,
    gap_class_44_properties,
    12,
    NULL,
    0,
    gap_class_44_constants,
    8
},
{
    "GKLeaderboardEntry",
    "RefCounted",
    gap_class_45_methods,
    6,
    gap_class_45_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboardScore",
    "RefCounted",
    gap_class_46_methods,
    8,
    gap_class_46_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboardSet",
    "RefCounted",
    gap_class_47_methods,
    6,
    gap_class_47_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "GKPlayer",
    "RefCounted",
    gap_class_48_methods,
    9,
    gap_class_48_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLocalPlayer",
    "GKPlayer",
    gap_class_49_methods,
    18,
    gap_class_49_properties,
    4,
    gap_class_49_signals,
    14,
    NULL,
    0
},
{
    "GKMatch",
    "RefCounted",
    gap_class_50_methods,
    12,
    gap_class_50_properties,
    5,
    gap_class_50_signals,
    4,
    gap_class_50_constants,
    2
},
{
    "GKMatchRequest",
    "RefCounted",
    gap_class_51_methods,
    23,
    gap_class_51_properties,
    11,
    NULL,
    0,
    gap_class_51_constants,
    10
},
{
    "GKMatchmaker",
    "RefCounted",
    gap_class_52_methods,
    15,
    NULL,
    0,
    gap_class_52_signals,
    2,
    NULL,
    0
},
{
    "GKMatchmakerViewController",
    "RefCounted",
    gap_class_53_methods,
    15,
    gap_class_53_properties,
    5,
    gap_class_53_signals,
    5,
    NULL,
    0
},
{
    "GKNotificationBanner",
    "RefCounted",
    gap_class_54_methods,
    2,
    NULL,
    0,
    NULL,
    0,
    NULL,
    0
},
{
    "GKSavedGame",
    "GKPlayer",
    gap_class_55_methods,
    4,
    gap_class_55_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "GKScoreChallenge",
    "GKChallenge",
    gap_class_56_methods,
    7,
    gap_class_56_properties,
    7,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedExchange",
    "RefCounted",
    gap_class_57_methods,
    12,
    gap_class_57_properties,
    10,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedExchangeReply",
    "RefCounted",
    gap_class_58_methods,
    4,
    gap_class_58_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedMatch",
    "RefCounted",
    gap_class_59_methods,
    35,
    gap_class_59_properties,
    14,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedMatchmakerViewController",
    "RefCounted",
    gap_class_60_methods,
    7,
    gap_class_60_properties,
    2,
    gap_class_60_signals,
    4,
    gap_class_60_constants,
    4
},
{
    "GKTurnBasedParticipant",
    "RefCounted",
    gap_class_61_methods,
    6,
    gap_class_61_properties,
    5,
    NULL,
    0,
    NULL,
    0
},
{
    "GKVoiceChat",
    "RefCounted",
    gap_class_62_methods,
    10,
    gap_class_62_properties,
    4,
    gap_class_62_signals,
    1,
    NULL,
    0
},
{
    "GameCenterManager",
    "RefCounted",
    gap_class_63_methods,
    4,
    gap_class_63_properties,
    2,
    gap_class_63_signals,
    2,
    NULL,
    0
},
{
    "ProductView",
    "RefCounted",
    gap_class_64_methods,
    10,
    gap_class_64_properties,
    4,
    NULL,
    0,
    gap_class_64_constants,
    4
},
{
    "SignalProxy",
    "Object",
    gap_class_65_methods,
    1,
    NULL,
    0,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreKitManager",
    "RefCounted",
    gap_class_66_methods,
    6,
    NULL,
    0,
    gap_class_66_signals,
    7,
    gap_class_66_constants,
    14
},
{
    "StoreProduct",
    "RefCounted",
    gap_class_67_methods,
    7,
    gap_class_67_properties,
    7,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreProductPaymentMode",
    "RefCounted",
    gap_class_68_methods,
    4,
    gap_class_68_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreProductPurchaseOption",
    "RefCounted",
    gap_class_69_methods,
    5,
    NULL,
    0,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreProductSubscriptionOffer",
    "RefCounted",
    NULL,
    0,
    NULL,
    0,
    NULL,
    0,
    gap_class_70_constants,
    4
},
{
    "StoreProductSubscriptionPeriod",
    "RefCounted",
    NULL,
    0,
    NULL,
    0,
    NULL,
    0,
    gap_class_71_constants,
    4
},
{
    "StoreSubscriptionInfo",
    "RefCounted",
    gap_class_72_methods,
    6,
    gap_class_72_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreSubscriptionInfoRenewalInfo",
    "RefCounted",
    gap_class_73_methods,
    4,
    gap_class_73_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreSubscriptionInfoStatus",
    "RefCounted",
    gap_class_74_methods,
    3,
    gap_class_74_properties,
    3,
    NULL,
    0,
    gap_class_74_constants,
    6
},
{
    "StoreTransaction",
    "RefCounted",
    gap_class_75_methods,
    9,
    gap_class_75_properties,
    8,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreView",
    "RefCounted",
    gap_class_76_methods,
    4,
    gap_class_76_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "SubscriptionOfferView",
    "RefCounted",
    gap_class_77_methods,
    4,
    gap_class_77_properties,
    1,
    gap_class_77_signals,
    2,
    NULL,
    0
},
{
    "SubscriptionStoreView",
    "RefCounted",
    gap_class_78_methods,
    8,
    gap_class_78_properties,
    3,
    NULL,
    0,
    gap_class_78_constants,
    7
},
};
static const size_t gap_class_count = sizeof(gap_classes) / sizeof(gap_classes[0]);

static void gap_register_method(const GAPStubMethodDescriptor *method, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage *method_name;
    GAPStubPropertyInfoTemp *return_temp = NULL;
    GAPStubPropertyInfoTemp *argument_temps = NULL;
    GDExtensionPropertyInfo *return_info = NULL;
    GDExtensionPropertyInfo *arguments_info = NULL;
    GDExtensionClassMethodArgumentMetadata *arguments_metadata = NULL;
    GDExtensionClassMethodInfo *method_info;

    method_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    method_info = (GDExtensionClassMethodInfo *)gap_persistent_alloc(sizeof(GDExtensionClassMethodInfo), 1, NULL);
    if (method_name == NULL || method_info == NULL) {
        gap_report_unimplemented(method->display_name);
        return;
    }

    gap_string_name_init(method_name, method->name);
    method_info->name = (GDExtensionStringNamePtr)method_name;
    method_info->method_userdata = (void *)method;
    method_info->call_func = gap_stub_method_call;
    method_info->ptrcall_func = NULL;
    method_info->method_flags = method->flags;
    method_info->has_return_value = method->has_return_value;
    method_info->return_value_metadata = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;

    if (method->has_return_value) {
        return_temp = (GAPStubPropertyInfoTemp *)gap_persistent_alloc(sizeof(GAPStubPropertyInfoTemp), 1, gap_cleanup_property_info_temp);
        return_info = (GDExtensionPropertyInfo *)gap_persistent_alloc(sizeof(GDExtensionPropertyInfo), 1, NULL);
        if (return_temp == NULL || return_info == NULL) {
            gap_report_unimplemented(method->display_name);
            return;
        }
        gap_property_info_init(return_temp, return_info, "return_value", &method->return_type);
        method_info->return_value_info = return_info;
    }

    if (method->argument_count > 0) {
        uint32_t index;

        argument_temps = (GAPStubPropertyInfoTemp *)gap_persistent_alloc(sizeof(GAPStubPropertyInfoTemp), method->argument_count, gap_cleanup_property_info_temp);
        arguments_info = (GDExtensionPropertyInfo *)gap_persistent_alloc(sizeof(GDExtensionPropertyInfo), method->argument_count, NULL);
        arguments_metadata = (GDExtensionClassMethodArgumentMetadata *)gap_persistent_alloc(sizeof(GDExtensionClassMethodArgumentMetadata), method->argument_count, NULL);

        if (argument_temps == NULL || arguments_info == NULL || arguments_metadata == NULL) {
            gap_report_unimplemented(method->display_name);
            return;
        }

        for (index = 0; index < method->argument_count; ++index) {
            gap_property_info_init(&argument_temps[index], &arguments_info[index], method->arguments[index].name, &method->arguments[index].type);
            arguments_metadata[index] = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
        }

        method_info->argument_count = method->argument_count;
        method_info->arguments_info = arguments_info;
        method_info->arguments_metadata = arguments_metadata;
    }

    gap_api.classdb_register_extension_class_method(gap_library, class_name, method_info);
}

static void gap_register_property(const GAPStubPropertyDescriptor *property, GDExtensionConstStringNamePtr class_name) {
    GAPStubPropertyInfoTemp *property_temp;
    GAPStubStringNameStorage *setter_name;
    GAPStubStringNameStorage *getter_name;
    GDExtensionPropertyInfo *property_info;

    property_temp = (GAPStubPropertyInfoTemp *)gap_persistent_alloc(sizeof(GAPStubPropertyInfoTemp), 1, gap_cleanup_property_info_temp);
    setter_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    getter_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    property_info = (GDExtensionPropertyInfo *)gap_persistent_alloc(sizeof(GDExtensionPropertyInfo), 1, NULL);

    if (property_temp == NULL || setter_name == NULL || getter_name == NULL || property_info == NULL) {
        gap_report_unimplemented(property->name);
        return;
    }

    gap_property_info_init(property_temp, property_info, property->name, &property->type);
    gap_string_name_init(setter_name, property->setter);
    gap_string_name_init(getter_name, property->getter);

    gap_api.classdb_register_extension_class_property(
        gap_library,
        class_name,
        property_info,
        (GDExtensionConstStringNamePtr)setter_name,
        (GDExtensionConstStringNamePtr)getter_name
    );
}

static void gap_register_signal(const GAPStubSignalDescriptor *signal, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage *signal_name;
    GAPStubPropertyInfoTemp *argument_temps = NULL;
    GDExtensionPropertyInfo *arguments = NULL;

    signal_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    if (signal_name == NULL) {
        gap_report_unimplemented(signal->name);
        return;
    }

    gap_string_name_init(signal_name, signal->name);

    if (signal->argument_count > 0) {
        uint32_t index;

        argument_temps = (GAPStubPropertyInfoTemp *)gap_persistent_alloc(sizeof(GAPStubPropertyInfoTemp), signal->argument_count, gap_cleanup_property_info_temp);
        arguments = (GDExtensionPropertyInfo *)gap_persistent_alloc(sizeof(GDExtensionPropertyInfo), signal->argument_count, NULL);

        if (argument_temps == NULL || arguments == NULL) {
            gap_report_unimplemented(signal->name);
            return;
        }

        for (index = 0; index < signal->argument_count; ++index) {
            gap_property_info_init(&argument_temps[index], &arguments[index], signal->arguments[index].name, &signal->arguments[index].type);
        }
    }

    gap_api.classdb_register_extension_class_signal(
        gap_library,
        class_name,
        (GDExtensionConstStringNamePtr)signal_name,
        arguments,
        signal->argument_count
    );
}

static void gap_register_constant(const GAPStubConstantDescriptor *constant, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage *enum_name;
    GAPStubStringNameStorage *constant_name;

    enum_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    constant_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    if (enum_name == NULL || constant_name == NULL) {
        gap_report_unimplemented(constant->name);
        return;
    }

    gap_string_name_init(enum_name, constant->enum_name);
    gap_string_name_init(constant_name, constant->name);

    gap_api.classdb_register_extension_class_integer_constant(
        gap_library,
        class_name,
        (GDExtensionConstStringNamePtr)enum_name,
        (GDExtensionConstStringNamePtr)constant_name,
        constant->value,
        constant->is_bitfield
    );
}

static void gap_register_class_shell(const GAPStubClassDescriptor *descriptor) {
    GAPStubStringNameStorage *class_name;
    GAPStubStringNameStorage *parent_name;
    GDExtensionClassCreationInfo2 *class_info;

    class_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    parent_name = (GAPStubStringNameStorage *)gap_persistent_alloc(sizeof(GAPStubStringNameStorage), 1, gap_cleanup_string_name_storage);
    class_info = (GDExtensionClassCreationInfo2 *)gap_persistent_alloc(sizeof(GDExtensionClassCreationInfo2), 1, NULL);

    if (class_name == NULL || parent_name == NULL || class_info == NULL) {
        gap_report_unimplemented(descriptor->name);
        return;
    }

    gap_string_name_init(class_name, descriptor->name);
    gap_string_name_init(parent_name, descriptor->parent_name);

    class_info->is_exposed = 1;
    class_info->create_instance_func = gap_stub_create_instance;
    class_info->free_instance_func = gap_stub_free_instance;
    class_info->class_userdata = (void *)descriptor;

    gap_api.classdb_register_extension_class2(
        gap_library,
        (GDExtensionConstStringNamePtr)class_name,
        (GDExtensionConstStringNamePtr)parent_name,
        class_info
    );
}

static void gap_register_class_members(const GAPStubClassDescriptor *descriptor) {
    GAPStubStringNameStorage class_name;
    uint32_t index;

    gap_string_name_init(&class_name, descriptor->name);

    for (index = 0; index < descriptor->method_count; ++index) {
        gap_register_method(&descriptor->methods[index], (GDExtensionConstStringNamePtr)&class_name);
    }

    for (index = 0; index < descriptor->property_count; ++index) {
        gap_register_property(&descriptor->properties[index], (GDExtensionConstStringNamePtr)&class_name);
    }

    for (index = 0; index < descriptor->signal_count; ++index) {
        gap_register_signal(&descriptor->signals[index], (GDExtensionConstStringNamePtr)&class_name);
    }

    for (index = 0; index < descriptor->constant_count; ++index) {
        gap_register_constant(&descriptor->constants[index], (GDExtensionConstStringNamePtr)&class_name);
    }

    gap_string_name_destroy(&class_name);
}

static void gap_register_all(void) {
    size_t index;

    for (index = 0; index < gap_class_count; ++index) {
        gap_register_class_shell(&gap_classes[index]);
    }

    for (index = 0; index < gap_class_count; ++index) {
        gap_register_class_members(&gap_classes[index]);
    }
}

static void gap_unregister_all(void) {
    GAPStubStringNameStorage class_name;
    size_t index;

    if (gap_api.classdb_unregister_extension_class == NULL) {
        return;
    }

    for (index = gap_class_count; index > 0; --index) {
        gap_string_name_init(&class_name, gap_classes[index - 1].name);
        gap_api.classdb_unregister_extension_class(gap_library, (GDExtensionConstStringNamePtr)&class_name);
        gap_string_name_destroy(&class_name);
    }
}

static void gap_initialize(void *userdata, GDExtensionInitializationLevel level) {
    (void)userdata;
    if (level == GDEXTENSION_INITIALIZATION_SCENE) {
        gap_register_all();
    }
}

static void gap_deinitialize(void *userdata, GDExtensionInitializationLevel level) {
    (void)userdata;
    if (level == GDEXTENSION_INITIALIZATION_SCENE) {
        gap_unregister_all();
        gap_release_persistent_allocations();
    }
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-mismatch"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

static GDExtensionBool gap_load_api(GDExtensionInterfaceGetProcAddress p_get_proc_address) {
#define GAP_LOAD_REQUIRED(field, symbol, type)     do {         gap_api.field = (type)p_get_proc_address(symbol);         if (gap_api.field == NULL) {             return 0;         }     } while (0)

#define GAP_LOAD_OPTIONAL(field, symbol, type)     do {         gap_api.field = (type)p_get_proc_address(symbol);     } while (0)

    memset(&gap_api, 0, sizeof(gap_api));

    GAP_LOAD_REQUIRED(mem_alloc, "mem_alloc", GDExtensionInterfaceMemAlloc);
    GAP_LOAD_REQUIRED(mem_free, "mem_free", GDExtensionInterfaceMemFree);
    GAP_LOAD_REQUIRED(print_error_with_message, "print_error_with_message", GDExtensionInterfacePrintErrorWithMessage);
    GAP_LOAD_REQUIRED(string_new_with_utf8_chars, "string_new_with_utf8_chars", GDExtensionInterfaceStringNewWithUtf8Chars);
    GAP_LOAD_REQUIRED(string_name_new_with_latin1_chars, "string_name_new_with_latin1_chars", GDExtensionInterfaceStringNameNewWithLatin1Chars);
    GAP_LOAD_REQUIRED(variant_get_ptr_destructor, "variant_get_ptr_destructor", GDExtensionInterfaceVariantGetPtrDestructor);
    GAP_LOAD_REQUIRED(variant_new_nil, "variant_new_nil", GDExtensionInterfaceVariantNewNil);
    GAP_LOAD_REQUIRED(classdb_register_extension_class2, "classdb_register_extension_class2", GDExtensionInterfaceClassdbRegisterExtensionClass2);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_method, "classdb_register_extension_class_method", GDExtensionInterfaceClassdbRegisterExtensionClassMethod);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_property, "classdb_register_extension_class_property", GDExtensionInterfaceClassdbRegisterExtensionClassProperty);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_signal, "classdb_register_extension_class_signal", GDExtensionInterfaceClassdbRegisterExtensionClassSignal);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_integer_constant, "classdb_register_extension_class_integer_constant", GDExtensionInterfaceClassdbRegisterExtensionClassIntegerConstant);
    GAP_LOAD_OPTIONAL(classdb_unregister_extension_class, "classdb_unregister_extension_class", GDExtensionInterfaceClassdbUnregisterExtensionClass);

    gap_api.string_destructor = gap_api.variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING);
    gap_api.string_name_destructor = gap_api.variant_get_ptr_destructor(GDEXTENSION_VARIANT_TYPE_STRING_NAME);

    return 1;

#undef GAP_LOAD_OPTIONAL
#undef GAP_LOAD_REQUIRED
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

GDExtensionBool godot_apple_plugins_start(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization
) {
    if (!gap_load_api(p_get_proc_address)) {
        return 0;
    }

    gap_library = p_library;

    r_initialization->userdata = NULL;
    r_initialization->initialize = gap_initialize;
    r_initialization->deinitialize = gap_deinitialize;
    r_initialization->minimum_initialization_level = GDEXTENSION_INITIALIZATION_SCENE;

    return 1;
}