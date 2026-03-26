#include "godot_apple_plugins_stub.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GAP_PROPERTY_HINT_NONE 0u
#define GAP_PROPERTY_USAGE_DEFAULT 6u
#define GAP_PROPERTY_USAGE_NIL_IS_VARIANT 131072u

typedef union {
    max_align_t _align;
    unsigned char bytes[8];
} GAPStubStringStorage;

typedef union {
    max_align_t _align;
    unsigned char bytes[8];
} GAPStubStringNameStorage;

typedef union {
    max_align_t _align;
    unsigned char bytes[24];
} GAPStubVariantStorage;

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
    const GAPStubClassDescriptor *descriptor;
} GAPStubInstance;

typedef struct {
    GAPStubStringNameStorage name;
    GAPStubStringNameStorage class_name;
    GAPStubStringStorage hint_string;
} GAPStubPropertyInfoTemp;

typedef struct {
    GDExtensionInterfaceMemAlloc mem_alloc;
    GDExtensionInterfaceMemFree mem_free;
    GDExtensionInterfacePrintErrorWithMessage print_error_with_message;
    GDExtensionInterfaceStringNewWithUtf8Chars string_new_with_utf8_chars;
    GDExtensionInterfaceStringNameNewWithLatin1Chars string_name_new_with_latin1_chars;
    GDExtensionInterfaceVariantGetPtrDestructor variant_get_ptr_destructor;
    GDExtensionInterfaceVariantNewNil variant_new_nil;
    GDExtensionInterfaceClassdbConstructObject classdb_construct_object;
    GDExtensionInterfaceClassdbRegisterExtensionClass2 classdb_register_extension_class2;
    GDExtensionInterfaceClassdbRegisterExtensionClassMethod classdb_register_extension_class_method;
    GDExtensionInterfaceClassdbRegisterExtensionClassProperty classdb_register_extension_class_property;
    GDExtensionInterfaceClassdbRegisterExtensionClassSignal classdb_register_extension_class_signal;
    GDExtensionInterfaceClassdbRegisterExtensionClassIntegerConstant classdb_register_extension_class_integer_constant;
    GDExtensionInterfaceClassdbUnregisterExtensionClass classdb_unregister_extension_class;
    GDExtensionInterfaceObjectSetInstance object_set_instance;
    GDExtensionPtrDestructor string_destructor;
    GDExtensionPtrDestructor string_name_destructor;
} GAPStubAPI;

static GAPStubAPI gap_api;
static GDExtensionClassLibraryPtr gap_library;

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
    GAPStubStringNameStorage class_name;
    GAPStubInstance *instance;
    GDExtensionObjectPtr object;

    gap_string_name_init(&class_name, descriptor->name);
    object = gap_api.classdb_construct_object((GDExtensionConstStringNamePtr)&class_name);

    if (object != NULL) {
        instance = (GAPStubInstance *)gap_api.mem_alloc(sizeof(GAPStubInstance));
        if (instance != NULL) {
            instance->descriptor = descriptor;
            gap_api.object_set_instance(object, (GDExtensionConstStringNamePtr)&class_name, instance);
        }
    }

    gap_string_name_destroy(&class_name);
    return object;
}

static void gap_stub_free_instance(void *class_userdata, GDExtensionClassInstancePtr p_instance) {
    (void)class_userdata;
    if (p_instance != NULL) {
        gap_api.mem_free(p_instance);
    }
}

static const GAPStubMethodDescriptor gap_class_0_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_0_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_0_constants[] = {
{ "UserDetectionStatus", "UNSUPPORTED", 0, 0 },
{ "UserDetectionStatus", "UNKNOWN", 1, 0 },
{ "UserDetectionStatus", "LIKELY_REAL", 2, 0 },
{ "UserAgeRange", "NOT_KNOWN", 0, 0 },
{ "UserAgeRange", "CHILD", 1, 0 },
{ "UserAgeRange", "NOT_CHILD", 2, 0 },
};
static const GAPStubNamedTypeInfo gap_class_1_method_1_args[] = {
    { "scopeStrings", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_signal_0_args[] = {
    { "credential", { GDEXTENSION_VARIANT_TYPE_OBJECT, "RefCounted", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_1_signal_1_args[] = {
    { "error", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_1_methods[] = {
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
    gap_class_1_method_1_args,
    1
},
};
static const GAPStubSignalDescriptor gap_class_1_signals[] = {
{
    "authorization_completed",
    gap_class_1_signal_0_args,
    1
},
{
    "authorization_failed",
    gap_class_1_signal_1_args,
    1
},
};
static const GAPStubMethodDescriptor gap_class_2_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_2_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_3_method_1_args[] = {
    { "auth_url", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback_scheme", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "prefers_ephemeral", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_3_signal_1_args[] = {
    { "callback_url", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_3_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_3_methods[] = {
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
    gap_class_3_method_1_args,
    3
},
};
static const GAPStubSignalDescriptor gap_class_3_signals[] = {
{
    "canceled",
    NULL,
    0
},
{
    "completed",
    gap_class_3_signal_1_args,
    1
},
{
    "failed",
    gap_class_3_signal_2_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_4_method_0_args[] = {
    { "category", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "policy", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_4_method_2_args[] = {
    { "current_category", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_4_methods[] = {
{
    "set_category",
    "AVAudioSession.set_category",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_4_method_0_args,
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
    gap_class_4_method_2_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_4_properties[] = {
{
    "current_category",
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_current_category",
    "get_current_category"
},
};
static const GAPStubConstantDescriptor gap_class_4_constants[] = {
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
static const GAPStubNamedTypeInfo gap_class_5_method_0_args[] = {
    { "allowed_types", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "allow_multiple", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_5_signal_1_args[] = {
    { "url", { GDEXTENSION_VARIANT_TYPE_OBJECT, "AppleURL", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "path", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_5_signal_2_args[] = {
    { "urls", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "paths", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_5_methods[] = {
{
    "pick_document",
    "AppleFilePicker.pick_document",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_5_method_0_args,
    2
},
};
static const GAPStubSignalDescriptor gap_class_5_signals[] = {
{
    "canceled",
    NULL,
    0
},
{
    "file_selected",
    gap_class_5_signal_1_args,
    2
},
{
    "files_selected",
    gap_class_5_signal_2_args,
    2
},
};
static const GAPStubNamedTypeInfo gap_class_6_method_5_args[] = {
    { "path", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_6_method_6_args[] = {
    { "str", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_6_methods[] = {
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
    gap_class_6_method_5_args,
    1
},
{
    "set_value",
    "AppleURL.set_value",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_6_method_6_args,
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
static const GAPStubMethodDescriptor gap_class_7_methods[] = {
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
static const GAPStubNamedTypeInfo gap_class_8_method_0_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_1_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_2_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_3_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_4_args[] = {
    { "achievementID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_5_args[] = {
    { "challengeDefinitionID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_6_args[] = {
    { "gameActivity", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivity", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_7_args[] = {
    { "gameActivityDefinitionID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_8_args[] = {
    { "leaderboardID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "playerScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_9_args[] = {
    { "leaderboardSetID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_10_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_11_args[] = {
    { "state", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_13_args[] = {
    { "active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_18_args[] = {
    { "location", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_8_method_20_args[] = {
    { "show_highlights", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_8_methods[] = {
{
    "trigger",
    "GKAccessPoint.trigger",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_0_args,
    1
},
{
    "trigger_for_challenges",
    "GKAccessPoint.trigger_for_challenges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_1_args,
    1
},
{
    "trigger_for_friending",
    "GKAccessPoint.trigger_for_friending",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_2_args,
    1
},
{
    "trigger_for_play_together",
    "GKAccessPoint.trigger_for_play_together",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_3_args,
    1
},
{
    "trigger_with_achievement",
    "GKAccessPoint.trigger_with_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_4_args,
    2
},
{
    "trigger_with_challenge_definition_id",
    "GKAccessPoint.trigger_with_challenge_definition_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_5_args,
    2
},
{
    "trigger_with_game_activity",
    "GKAccessPoint.trigger_with_game_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_6_args,
    2
},
{
    "trigger_with_game_activity_definition_id",
    "GKAccessPoint.trigger_with_game_activity_definition_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_7_args,
    2
},
{
    "trigger_with_leaderboard",
    "GKAccessPoint.trigger_with_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_8_args,
    4
},
{
    "trigger_with_leaderboard_set",
    "GKAccessPoint.trigger_with_leaderboard_set",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_9_args,
    2
},
{
    "trigger_with_player",
    "GKAccessPoint.trigger_with_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_10_args,
    2
},
{
    "trigger_with_state",
    "GKAccessPoint.trigger_with_state",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_8_method_11_args,
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
    gap_class_8_method_13_args,
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
    gap_class_8_method_18_args,
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
    gap_class_8_method_20_args,
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
static const GAPStubPropertyDescriptor gap_class_8_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_8_constants[] = {
{ "Location", "TOP_LEADING", 0, 0 },
{ "Location", "TOP_TRAILING", 1, 0 },
{ "Location", "BOTTOM_LEADING", 2, 0 },
{ "Location", "BOTTOM_TRAILING", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_9_method_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_2_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_3_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_4_args[] = {
    { "achievements", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_5_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_6_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_8_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_12_args[] = {
    { "percent_complete", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_9_method_15_args[] = {
    { "shows_completion_banner", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_9_methods[] = {
{
    "challenge_compose_controller",
    "GKAchievement.challenge_compose_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_0_args,
    2
},
{
    "load_achievements",
    "GKAchievement.load_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_1_args,
    1
},
{
    "make",
    "GKAchievement.make",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_2_args,
    1
},
{
    "make_for_player",
    "GKAchievement.make_for_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_3_args,
    2
},
{
    "report_achievement",
    "GKAchievement.report_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_4_args,
    2
},
{
    "reset_achievements",
    "GKAchievement.reset_achievements",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_5_args,
    1
},
{
    "select_challengeable_players",
    "GKAchievement.select_challengeable_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_9_method_6_args,
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
    gap_class_9_method_8_args,
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
    gap_class_9_method_12_args,
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
    gap_class_9_method_15_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_9_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_10_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_10_methods[] = {
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
    gap_class_10_method_1_args,
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
static const GAPStubPropertyDescriptor gap_class_10_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_10_constants[] = {
{ "ChallengeState", "INVALID", 0, 0 },
{ "ChallengeState", "PENDING", 1, 0 },
{ "ChallengeState", "COMPLETED", 2, 0 },
{ "ChallengeState", "DECLINED", 3, 0 },
{ "ChallengeType", "SCORE", 0, 0 },
{ "ChallengeType", "ACHIEVEMENT", 1, 0 },
{ "ChallengeType", "UNKNOWN", 2, 0 },
};
static const GAPStubMethodDescriptor gap_class_11_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_11_properties[] = {
{
    "achievement",
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_achievement"
},
};
static const GAPStubNamedTypeInfo gap_class_12_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_12_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_12_methods[] = {
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
    gap_class_12_method_1_args,
    1
},
{
    "load_image",
    "GKAchievementDescription.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_12_method_2_args,
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
static const GAPStubPropertyDescriptor gap_class_12_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_13_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_13_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_13_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_13_methods[] = {
{
    "has_active_challenges",
    "GKChallengeDefinition.has_active_challenges",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_0_args,
    1
},
{
    "load_challenge_definitions",
    "GKChallengeDefinition.load_challenge_definitions",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_1_args,
    1
},
{
    "load_image",
    "GKChallengeDefinition.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_13_method_2_args,
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
static const GAPStubPropertyDescriptor gap_class_13_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_14_method_1_args[] = {
    { "code", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_3_args[] = {
    { "domain", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_14_method_5_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_14_methods[] = {
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
    gap_class_14_method_1_args,
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
    gap_class_14_method_3_args,
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
    gap_class_14_method_5_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_14_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_14_constants[] = {
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
static const GAPStubNamedTypeInfo gap_class_15_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_1_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_5_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_6_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_7_args[] = {
    { "partyCode", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_10_args[] = {
    { "achievements", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_11_args[] = {
    { "leaderboards", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_13_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_14_args[] = {
    { "achievement", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKAchievement", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "percentComplete", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_15_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_16_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_18_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_19_args[] = {
    { "definition", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivityDefinition", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "partyCode", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_15_method_32_args[] = {
    { "properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_15_methods[] = {
{
    "check_pending_game_activity_existence",
    "GKGameActivity.check_pending_game_activity_existence",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_0_args,
    1
},
{
    "create_with_definition",
    "GKGameActivity.create_with_definition",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKGameActivity", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_1_args,
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
    gap_class_15_method_3_args,
    1
},
{
    "find_players_for_hosted_match",
    "GKGameActivity.find_players_for_hosted_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_4_args,
    1
},
{
    "get_progress_on_achievement",
    "GKGameActivity.get_progress_on_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_5_args,
    1
},
{
    "get_score_on_leaderboard",
    "GKGameActivity.get_score_on_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboardScore", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_6_args,
    1
},
{
    "is_valid_party_code",
    "GKGameActivity.is_valid_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_7_args,
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
    gap_class_15_method_10_args,
    1
},
{
    "remove_scores_from_leaderboards",
    "GKGameActivity.remove_scores_from_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_11_args,
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
    gap_class_15_method_13_args,
    1
},
{
    "set_progress_on_achievement",
    "GKGameActivity.set_progress_on_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_14_args,
    2
},
{
    "set_score_on_leaderboard",
    "GKGameActivity.set_score_on_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_15_args,
    2
},
{
    "set_score_on_leaderboard_with_context",
    "GKGameActivity.set_score_on_leaderboard_with_context",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_16_args,
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
    gap_class_15_method_18_args,
    2
},
{
    "start_with_definition_and_party_code",
    "GKGameActivity.start_with_definition_and_party_code",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_15_method_19_args,
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
    gap_class_15_method_32_args,
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
static const GAPStubPropertyDescriptor gap_class_15_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_16_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_2_args[] = {
    { "ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_16_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_16_methods[] = {
{
    "load_achievement_descriptions",
    "GKGameActivityDefinition.load_achievement_descriptions",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_0_args,
    1
},
{
    "load_game_activity_definitions",
    "GKGameActivityDefinition.load_game_activity_definitions",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_1_args,
    1
},
{
    "load_game_activity_definitions_with_ids",
    "GKGameActivityDefinition.load_game_activity_definitions_with_ids",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_2_args,
    2
},
{
    "load_image",
    "GKGameActivityDefinition.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_3_args,
    1
},
{
    "load_leaderboards",
    "GKGameActivityDefinition.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_16_method_4_args,
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
static const GAPStubPropertyDescriptor gap_class_16_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_17_method_0_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_1_args[] = {
    { "leaderboard", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLeaderboard", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "scope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_2_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "scope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_3_args[] = {
    { "id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_17_method_5_args[] = {
    { "type", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_17_methods[] = {
{
    "show_achievement",
    "GKGameCenterViewController.show_achievement",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_0_args,
    1
},
{
    "show_leaderboard",
    "GKGameCenterViewController.show_leaderboard",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_1_args,
    2
},
{
    "show_leaderboard_time_period",
    "GKGameCenterViewController.show_leaderboard_time_period",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_2_args,
    3
},
{
    "show_leaderboardset",
    "GKGameCenterViewController.show_leaderboardset",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_3_args,
    1
},
{
    "show_player",
    "GKGameCenterViewController.show_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_4_args,
    1
},
{
    "show_type",
    "GKGameCenterViewController.show_type",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_17_method_5_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_17_constants[] = {
{ "State", "DEFAULT_SCREEN", 0, 0 },
{ "State", "LEADERBOARDS", 1, 0 },
{ "State", "ACHIEVEMENTS", 2, 0 },
{ "State", "LOCAL_PLAYER_PROFILE", 3, 0 },
{ "State", "DASHBOARD", 4, 0 },
{ "State", "LOCAL_PLAYER_FRIENDS_LIST", 5, 0 },
};
static const GAPStubMethodDescriptor gap_class_18_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_18_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_19_method_0_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_2_args[] = {
    { "ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_3_args[] = {
    { "playerScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeScope", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "rangeStart", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "rangeLength", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_19_method_5_args[] = {
    { "score", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_19_methods[] = {
{
    "load_entries",
    "GKLeaderboard.load_entries",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_0_args,
    3
},
{
    "load_image",
    "GKLeaderboard.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_1_args,
    1
},
{
    "load_leaderboards",
    "GKLeaderboard.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_2_args,
    2
},
{
    "load_local_player_entries",
    "GKLeaderboard.load_local_player_entries",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_3_args,
    5
},
{
    "load_previous_occurrence",
    "GKLeaderboard.load_previous_occurrence",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_4_args,
    1
},
{
    "submit_score",
    "GKLeaderboard.submit_score",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_19_method_5_args,
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
static const GAPStubPropertyDescriptor gap_class_19_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_19_constants[] = {
{ "AppleLeaderboardType", "CLASSIC", 0, 0 },
{ "AppleLeaderboardType", "RECURRING", 1, 0 },
{ "AppleLeaderboardType", "UNKNOWN", 2, 0 },
{ "TimeScope", "TODAY", 0, 0 },
{ "TimeScope", "WEEK", 1, 0 },
{ "TimeScope", "ALL_TIME", 2, 0 },
{ "PlayerScope", "GLOBAL", 0, 0 },
{ "PlayerScope", "FRIENDS_ONLY", 1, 0 },
};
static const GAPStubMethodDescriptor gap_class_20_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_20_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_21_method_1_args[] = {
    { "context", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_3_args[] = {
    { "leaderboard_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_21_method_7_args[] = {
    { "value", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_21_methods[] = {
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
    gap_class_21_method_1_args,
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
    gap_class_21_method_3_args,
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
    gap_class_21_method_5_args,
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
    gap_class_21_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_21_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_22_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_22_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_22_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_22_methods[] = {
{
    "load_image",
    "GKLeaderboardSet.load_image",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_22_method_0_args,
    1
},
{
    "load_leaderboard_sets",
    "GKLeaderboardSet.load_leaderboard_sets",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_22_method_1_args,
    1
},
{
    "load_leaderboards",
    "GKLeaderboardSet.load_leaderboards",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_22_method_2_args,
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
static const GAPStubPropertyDescriptor gap_class_22_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_23_method_0_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_23_method_1_args[] = {
    { "small", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_23_methods[] = {
{
    "anonymous_guest_player",
    "GKPlayer.anonymous_guest_player",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_0_args,
    1
},
{
    "load_photo",
    "GKPlayer.load_photo",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_23_method_1_args,
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
static const GAPStubPropertyDescriptor gap_class_23_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_24_method_0_args[] = {
    { "named", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_3_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_4_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_5_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_6_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_7_args[] = {
    { "identifiers", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_10_args[] = {
    { "conflicts", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_11_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "withName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_method_12_args[] = {
    { "identifier", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "friend_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_2_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "friend_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_3_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "challenge", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKChallenge", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "conflicting_saved_games", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "exchange", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedExchange", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_6_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "replies", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_7_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "exchange", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedExchange", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_8_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_9_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "recipient_players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_10_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_11_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "saved_game", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKSavedGame", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_12_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_24_signal_13_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "did_become_active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_24_methods[] = {
{
    "delete_saved_games",
    "GKLocalPlayer.delete_saved_games",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_0_args,
    2
},
{
    "fetch_items_for_identity_verification_signature",
    "GKLocalPlayer.fetch_items_for_identity_verification_signature",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_1_args,
    1
},
{
    "fetch_saved_games",
    "GKLocalPlayer.fetch_saved_games",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_2_args,
    1
},
{
    "load_challengeable_friends",
    "GKLocalPlayer.load_challengeable_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_3_args,
    1
},
{
    "load_default_leaderboard_identifier",
    "GKLocalPlayer.load_default_leaderboard_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_4_args,
    1
},
{
    "load_friends",
    "GKLocalPlayer.load_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_5_args,
    1
},
{
    "load_friends_authorization_status",
    "GKLocalPlayer.load_friends_authorization_status",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_6_args,
    1
},
{
    "load_friends_with_identifiers",
    "GKLocalPlayer.load_friends_with_identifiers",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_7_args,
    2
},
{
    "load_recent_friends",
    "GKLocalPlayer.load_recent_friends",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_8_args,
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
    gap_class_24_method_10_args,
    3
},
{
    "save_game_data",
    "GKLocalPlayer.save_game_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_11_args,
    3
},
{
    "set_default_leaderboard_identifier",
    "GKLocalPlayer.set_default_leaderboard_identifier",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_24_method_12_args,
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
static const GAPStubPropertyDescriptor gap_class_24_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_24_signals[] = {
{
    "challenge_completed",
    gap_class_24_signal_0_args,
    3
},
{
    "challenge_other_player_accepted",
    gap_class_24_signal_1_args,
    2
},
{
    "challenge_other_player_completed",
    gap_class_24_signal_2_args,
    3
},
{
    "challenge_received",
    gap_class_24_signal_3_args,
    2
},
{
    "conflicting_saved_games",
    gap_class_24_signal_4_args,
    2
},
{
    "exchange_canceled",
    gap_class_24_signal_5_args,
    3
},
{
    "exchange_completed",
    gap_class_24_signal_6_args,
    3
},
{
    "exchange_received",
    gap_class_24_signal_7_args,
    3
},
{
    "invite_accepted",
    gap_class_24_signal_8_args,
    2
},
{
    "match_requested_with_other_players",
    gap_class_24_signal_9_args,
    2
},
{
    "player_wants_to_quit_match",
    gap_class_24_signal_10_args,
    2
},
{
    "saved_game_modified",
    gap_class_24_signal_11_args,
    2
},
{
    "turn_based_match_ended",
    gap_class_24_signal_12_args,
    2
},
{
    "turn_event_received",
    gap_class_24_signal_13_args,
    3
},
};
static const GAPStubNamedTypeInfo gap_class_25_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_method_2_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_method_3_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "toPlayers", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "dataMode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_method_4_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "dataMode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_method_5_args[] = {
    { "channel", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_method_11_args[] = {
    { "should_reinvite_disconnected_player", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_25_signal_0_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_signal_1_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "recipient", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "from_remote_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_25_signal_3_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "connected", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_25_methods[] = {
{
    "choose_best_hosting_player",
    "GKMatch.choose_best_hosting_player",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_25_method_0_args,
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
    gap_class_25_method_2_args,
    1
},
{
    "send",
    "GKMatch.send",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_25_method_3_args,
    3
},
{
    "send_data_to_all_players",
    "GKMatch.send_data_to_all_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_25_method_4_args,
    2
},
{
    "voice_chat",
    "GKMatch.voice_chat",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKVoiceChat", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_25_method_5_args,
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
    gap_class_25_method_11_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_25_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_25_signals[] = {
{
    "data_received",
    gap_class_25_signal_0_args,
    2
},
{
    "data_received_for_recipient_from_player",
    gap_class_25_signal_1_args,
    3
},
{
    "did_fail_with_error",
    gap_class_25_signal_2_args,
    1
},
{
    "player_changed",
    gap_class_25_signal_3_args,
    2
},
};
static const GAPStubConstantDescriptor gap_class_25_constants[] = {
{ "SendDataMode", "RELIABLE", 0, 0 },
{ "SendDataMode", "UNRELIABLE", 1, 0 },
};
static const GAPStubNamedTypeInfo gap_class_26_method_0_args[] = {
    { "forType", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_2_args[] = {
    { "default_number_of_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_4_args[] = {
    { "invite_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_6_args[] = {
    { "max_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_8_args[] = {
    { "min_players", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_10_args[] = {
    { "player_attributes", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_12_args[] = {
    { "player_group", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_14_args[] = {
    { "properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_16_args[] = {
    { "queue_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_18_args[] = {
    { "recipient_properties", { GDEXTENSION_VARIANT_TYPE_DICTIONARY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_20_args[] = {
    { "recipient_response", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_26_method_22_args[] = {
    { "recipients", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_26_methods[] = {
{
    "max_players_allowed_for_match",
    "GKMatchRequest.max_players_allowed_for_match",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_26_method_0_args,
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
    gap_class_26_method_2_args,
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
    gap_class_26_method_4_args,
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
    gap_class_26_method_6_args,
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
    gap_class_26_method_8_args,
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
    gap_class_26_method_10_args,
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
    gap_class_26_method_12_args,
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
    gap_class_26_method_14_args,
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
    gap_class_26_method_16_args,
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
    gap_class_26_method_18_args,
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
    gap_class_26_method_20_args,
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
    gap_class_26_method_22_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_26_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_26_constants[] = {
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
static const GAPStubNamedTypeInfo gap_class_27_method_0_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_2_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_3_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_4_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_5_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_6_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_7_args[] = {
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_9_args[] = {
    { "groupID", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_method_10_args[] = {
    { "queueName", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "is_reachable", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_27_signal_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_27_methods[] = {
{
    "add_players",
    "GKMatchmaker.add_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_0_args,
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
    gap_class_27_method_2_args,
    1
},
{
    "find_match",
    "GKMatchmaker.find_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_3_args,
    2
},
{
    "find_matched_players",
    "GKMatchmaker.find_matched_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_4_args,
    2
},
{
    "find_players",
    "GKMatchmaker.find_players",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_5_args,
    2
},
{
    "finish_matchmaking",
    "GKMatchmaker.finish_matchmaking",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_6_args,
    1
},
{
    "match_for_invite",
    "GKMatchmaker.match_for_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_7_args,
    2
},
{
    "query_activity",
    "GKMatchmaker.query_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_8_args,
    1
},
{
    "query_player_group_activity",
    "GKMatchmaker.query_player_group_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_9_args,
    2
},
{
    "query_queue_activity",
    "GKMatchmaker.query_queue_activity",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_27_method_10_args,
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
static const GAPStubSignalDescriptor gap_class_27_signals[] = {
{
    "nearby_player_reachable",
    gap_class_27_signal_0_args,
    2
},
{
    "player_joining_group_activity",
    gap_class_27_signal_1_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_28_method_0_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_1_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_2_args[] = {
    { "invite", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKInvite", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_4_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_5_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "didConnect", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_7_args[] = {
    { "can_start_with_minimum_players", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_9_args[] = {
    { "get_match_properties_for_recipient", { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_11_args[] = {
    { "is_hosted", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_method_14_args[] = {
    { "matchmaking_mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_0_args[] = {
    { "detail", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_1_args[] = {
    { "players", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_2_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_3_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_28_signal_4_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_28_methods[] = {
{
    "add_players_to_match",
    "GKMatchmakerViewController.add_players_to_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_28_method_0_args,
    1
},
{
    "create_controller",
    "GKMatchmakerViewController.create_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_28_method_1_args,
    1
},
{
    "create_controller_from_invite",
    "GKMatchmakerViewController.create_controller_from_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_28_method_2_args,
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
    gap_class_28_method_4_args,
    2
},
{
    "set_hosted_player_did_connect",
    "GKMatchmakerViewController.set_hosted_player_did_connect",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_28_method_5_args,
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
    gap_class_28_method_7_args,
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
    gap_class_28_method_9_args,
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
    gap_class_28_method_11_args,
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
    gap_class_28_method_14_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_28_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_28_signals[] = {
{
    "cancelled",
    gap_class_28_signal_0_args,
    1
},
{
    "did_find_hosted_players",
    gap_class_28_signal_1_args,
    1
},
{
    "did_find_match",
    gap_class_28_signal_2_args,
    1
},
{
    "failed_with_error",
    gap_class_28_signal_3_args,
    1
},
{
    "hosted_player_did_accept",
    gap_class_28_signal_4_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_29_method_0_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_29_method_1_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "duration", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_29_methods[] = {
{
    "show",
    "GKNotificationBanner.show",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_29_method_0_args,
    2
},
{
    "show_with_duration",
    "GKNotificationBanner.show_with_duration",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_29_method_1_args,
    3
},
};
static const GAPStubNamedTypeInfo gap_class_30_method_0_args[] = {
    { "done", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_30_methods[] = {
{
    "load_data",
    "GKSavedGame.load_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_30_method_0_args,
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
static const GAPStubPropertyDescriptor gap_class_30_properties[] = {
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
static const GAPStubMethodDescriptor gap_class_31_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_31_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_32_method_0_args[] = {
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_32_method_1_args[] = {
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_32_methods[] = {
{
    "cancel",
    "GKTurnBasedExchange.cancel",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_32_method_0_args,
    3
},
{
    "reply",
    "GKTurnBasedExchange.reply",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_32_method_1_args,
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
static const GAPStubPropertyDescriptor gap_class_32_properties[] = {
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
static const GAPStubMethodDescriptor gap_class_33_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_33_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_34_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_2_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_3_args[] = {
    { "nextParticipants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_6_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_7_args[] = {
    { "matchID", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_8_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_9_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_10_args[] = {
    { "outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "nextParticipants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_11_args[] = {
    { "outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_12_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_13_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_14_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_15_args[] = {
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "resolvedExchanges", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_16_args[] = {
    { "participants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "data", { GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "timeout", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_17_args[] = {
    { "participants", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "localizableMessageKey", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_34_method_18_args[] = {
    { "key", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "arguments", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_34_methods[] = {
{
    "accept_invite",
    "GKTurnBasedMatch.accept_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_0_args,
    1
},
{
    "decline_invite",
    "GKTurnBasedMatch.decline_invite",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_1_args,
    1
},
{
    "end_match_in_turn",
    "GKTurnBasedMatch.end_match_in_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_2_args,
    2
},
{
    "end_turn",
    "GKTurnBasedMatch.end_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_3_args,
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
    gap_class_34_method_6_args,
    2
},
{
    "load",
    "GKTurnBasedMatch.load",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_7_args,
    2
},
{
    "load_match_data",
    "GKTurnBasedMatch.load_match_data",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_8_args,
    1
},
{
    "load_matches",
    "GKTurnBasedMatch.load_matches",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_9_args,
    1
},
{
    "participant_quit_in_turn",
    "GKTurnBasedMatch.participant_quit_in_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_10_args,
    5
},
{
    "participant_quit_out_of_turn",
    "GKTurnBasedMatch.participant_quit_out_of_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_11_args,
    2
},
{
    "rematch",
    "GKTurnBasedMatch.rematch",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_12_args,
    1
},
{
    "remove",
    "GKTurnBasedMatch.remove",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_13_args,
    1
},
{
    "save_current_turn",
    "GKTurnBasedMatch.save_current_turn",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_14_args,
    2
},
{
    "save_merged_match",
    "GKTurnBasedMatch.save_merged_match",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_15_args,
    3
},
{
    "send_exchange",
    "GKTurnBasedMatch.send_exchange",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_16_args,
    6
},
{
    "send_reminder",
    "GKTurnBasedMatch.send_reminder",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_17_args,
    4
},
{
    "set_localizable_message_with_key",
    "GKTurnBasedMatch.set_localizable_message_with_key",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_34_method_18_args,
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
static const GAPStubPropertyDescriptor gap_class_34_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_35_method_0_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_method_2_args[] = {
    { "request", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKMatchRequest", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_method_4_args[] = {
    { "matchmaking_mode", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_method_6_args[] = {
    { "show_existing_matches", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_signal_0_args[] = {
    { "detail", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_signal_1_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_signal_2_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_35_signal_3_args[] = {
    { "match", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatch", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_35_methods[] = {
{
    "create_controller",
    "GKTurnBasedMatchmakerViewController.create_controller",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKTurnBasedMatchmakerViewController", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_35_method_0_args,
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
    gap_class_35_method_2_args,
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
    gap_class_35_method_4_args,
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
    gap_class_35_method_6_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_35_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_35_signals[] = {
{
    "cancelled",
    gap_class_35_signal_0_args,
    1
},
{
    "did_find_match",
    gap_class_35_signal_1_args,
    1
},
{
    "failed_with_error",
    gap_class_35_signal_2_args,
    1
},
{
    "player_quit_for_match",
    gap_class_35_signal_3_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_35_constants[] = {
{ "MatchmakingMode", "DEFAULT", 0, 0 },
{ "MatchmakingMode", "NEARBY_ONLY", 1, 0 },
{ "MatchmakingMode", "AUTOMATCH_ONLY", 2, 0 },
{ "MatchmakingMode", "INVITE_ONLY", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_36_method_2_args[] = {
    { "match_outcome", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_36_methods[] = {
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
    gap_class_36_method_2_args,
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
static const GAPStubPropertyDescriptor gap_class_36_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_37_method_1_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "muted", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_37_method_5_args[] = {
    { "is_active", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_37_method_9_args[] = {
    { "volume", { GDEXTENSION_VARIANT_TYPE_FLOAT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_37_signal_0_args[] = {
    { "player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "state", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_37_methods[] = {
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
    gap_class_37_method_1_args,
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
    gap_class_37_method_5_args,
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
    gap_class_37_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_37_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_37_signals[] = {
{
    "player_state_changed",
    gap_class_37_signal_0_args,
    2
},
};
static const GAPStubNamedTypeInfo gap_class_38_method_3_args[] = {
    { "local_player", { GDEXTENSION_VARIANT_TYPE_OBJECT, "GKLocalPlayer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_38_signal_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_38_signal_1_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_38_methods[] = {
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
    gap_class_38_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_38_properties[] = {
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
static const GAPStubSignalDescriptor gap_class_38_signals[] = {
{
    "authentication_error",
    gap_class_38_signal_0_args,
    1
},
{
    "authentication_result",
    gap_class_38_signal_1_args,
    1
},
};
static const GAPStubNamedTypeInfo gap_class_39_method_3_args[] = {
    { "prefers_promotional_icon", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_39_method_5_args[] = {
    { "product_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_39_method_7_args[] = {
    { "style", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_39_method_9_args[] = {
    { "system_icon_name", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_39_methods[] = {
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
    gap_class_39_method_3_args,
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
    gap_class_39_method_5_args,
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
    gap_class_39_method_7_args,
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
    gap_class_39_method_9_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_39_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_39_constants[] = {
{ "ViewStyle", "AUTOMATIC", 0, 0 },
{ "ViewStyle", "COMPACT", 1, 0 },
{ "ViewStyle", "LARGE", 2, 0 },
{ "ViewStyle", "REGULAR", 3, 0 },
};
static const GAPStubMethodDescriptor gap_class_40_methods[] = {
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
static const GAPStubNamedTypeInfo gap_class_41_method_1_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_2_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "options", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_method_3_args[] = {
    { "productIds", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_0_args[] = {
    { "products", { GDEXTENSION_VARIANT_TYPE_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_1_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "error_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_2_args[] = {
    { "product", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProduct", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_3_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "error_message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_4_args[] = {
    { "status", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreSubscriptionInfoStatus", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_5_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_41_signal_6_args[] = {
    { "transaction", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreTransaction", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "verification_error", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_41_methods[] = {
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
    gap_class_41_method_1_args,
    1
},
{
    "purchase_with_options",
    "StoreKitManager.purchase_with_options",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_2_args,
    2
},
{
    "request_products",
    "StoreKitManager.request_products",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_41_method_3_args,
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
static const GAPStubSignalDescriptor gap_class_41_signals[] = {
{
    "products_request_completed",
    gap_class_41_signal_0_args,
    2
},
{
    "purchase_completed",
    gap_class_41_signal_1_args,
    3
},
{
    "purchase_intent",
    gap_class_41_signal_2_args,
    1
},
{
    "restore_completed",
    gap_class_41_signal_3_args,
    2
},
{
    "supscription_update",
    gap_class_41_signal_4_args,
    1
},
{
    "transaction_updated",
    gap_class_41_signal_5_args,
    1
},
{
    "unverified_transaction_updated",
    gap_class_41_signal_6_args,
    2
},
};
static const GAPStubConstantDescriptor gap_class_41_constants[] = {
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
static const GAPStubMethodDescriptor gap_class_42_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_42_properties[] = {
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
static const GAPStubMethodDescriptor gap_class_43_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_43_properties[] = {
{
    "localized_description",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "",
    "get_localized_description"
},
};
static const GAPStubNamedTypeInfo gap_class_44_method_0_args[] = {
    { "stringUuidToken", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_1_args[] = {
    { "jws", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_2_args[] = {
    { "value", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_3_args[] = {
    { "enabled", { GDEXTENSION_VARIANT_TYPE_BOOL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_44_method_4_args[] = {
    { "offer", { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductSubscriptionOffer", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_44_methods[] = {
{
    "app_account_token",
    "StoreProductPurchaseOption.app_account_token",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_0_args,
    1
},
{
    "introductory_offer_elligibility",
    "StoreProductPurchaseOption.introductory_offer_elligibility",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_1_args,
    1
},
{
    "quantity",
    "StoreProductPurchaseOption.quantity",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_2_args,
    1
},
{
    "simulate_ask_to_buy_in_sandbox",
    "StoreProductPurchaseOption.simulate_ask_to_buy_in_sandbox",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_3_args,
    1
},
{
    "win_back_offer",
    "StoreProductPurchaseOption.win_back_offer",
    GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC,
    1,
    { GDEXTENSION_VARIANT_TYPE_OBJECT, "StoreProductPurchaseOption", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_44_method_4_args,
    1
},
};
static const GAPStubConstantDescriptor gap_class_45_constants[] = {
{ "OfferType", "INTRODUCTORY", 0, 0 },
{ "OfferType", "PROMOTIONAL", 1, 0 },
{ "OfferType", "WIN_BACK", 2, 0 },
{ "OfferType", "UNKNOWN", 3, 0 },
};
static const GAPStubConstantDescriptor gap_class_46_constants[] = {
{ "Unit", "DAY", 0, 0 },
{ "Unit", "MONTH", 1, 0 },
{ "Unit", "WEEK", 2, 0 },
{ "Unit", "YEAR", 3, 0 },
};
static const GAPStubNamedTypeInfo gap_class_47_method_0_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_47_method_1_args[] = {
    { "group_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_47_method_2_args[] = {
    { "transaction_id", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
    { "status", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_47_methods[] = {
{
    "getStatus",
    "StoreSubscriptionInfo.getStatus",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_0_args,
    1
},
{
    "status_for_group_id",
    "StoreSubscriptionInfo.status_for_group_id",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_1_args,
    2
},
{
    "status_for_transaction",
    "StoreSubscriptionInfo.status_for_transaction",
    GDEXTENSION_METHOD_FLAG_NORMAL,
    0,
    { GDEXTENSION_VARIANT_TYPE_NIL, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    gap_class_47_method_2_args,
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
static const GAPStubPropertyDescriptor gap_class_47_properties[] = {
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
static const GAPStubMethodDescriptor gap_class_48_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_48_properties[] = {
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
static const GAPStubMethodDescriptor gap_class_49_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_49_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_49_constants[] = {
{ "RenewalState", "UNKNOWN", 0, 0 },
{ "RenewalState", "EXPIRED", 1, 0 },
{ "RenewalState", "SUBSCRIBED", 2, 0 },
{ "RenewalState", "IN_BILLING_RETRY_PERIOD", 3, 0 },
{ "RenewalState", "IN_GRACE_PERIOD", 4, 0 },
{ "RenewalState", "REVOKED", 5, 0 },
};
static const GAPStubMethodDescriptor gap_class_50_methods[] = {
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
static const GAPStubPropertyDescriptor gap_class_50_properties[] = {
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
static const GAPStubNamedTypeInfo gap_class_51_method_3_args[] = {
    { "product_ids", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_51_methods[] = {
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
    gap_class_51_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_51_properties[] = {
{
    "product_ids",
    { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_product_ids",
    "get_product_ids"
},
};
static const GAPStubNamedTypeInfo gap_class_52_method_1_args[] = {
    { "callback", { GDEXTENSION_VARIANT_TYPE_CALLABLE, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_method_3_args[] = {
    { "title", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_52_signal_0_args[] = {
    { "message", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_52_methods[] = {
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
    gap_class_52_method_1_args,
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
    gap_class_52_method_3_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_52_properties[] = {
{
    "title",
    { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT },
    "set_title",
    "get_title"
},
};
static const GAPStubSignalDescriptor gap_class_52_signals[] = {
{
    "error",
    gap_class_52_signal_0_args,
    1
},
{
    "success",
    NULL,
    0
},
};
static const GAPStubNamedTypeInfo gap_class_53_method_3_args[] = {
    { "control_style", { GDEXTENSION_VARIANT_TYPE_INT, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_5_args[] = {
    { "group_id", { GDEXTENSION_VARIANT_TYPE_STRING, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubNamedTypeInfo gap_class_53_method_7_args[] = {
    { "product_i_ds", { GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY, "", GAP_PROPERTY_HINT_NONE, "", GAP_PROPERTY_USAGE_DEFAULT } },
};
static const GAPStubMethodDescriptor gap_class_53_methods[] = {
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
    gap_class_53_method_3_args,
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
    gap_class_53_method_5_args,
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
    gap_class_53_method_7_args,
    1
},
};
static const GAPStubPropertyDescriptor gap_class_53_properties[] = {
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
static const GAPStubConstantDescriptor gap_class_53_constants[] = {
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
    "ASAuthorizationAppleIDCredential",
    "RefCounted",
    gap_class_0_methods,
    9,
    gap_class_0_properties,
    9,
    NULL,
    0,
    gap_class_0_constants,
    6
},
{
    "ASAuthorizationController",
    "RefCounted",
    gap_class_1_methods,
    2,
    NULL,
    0,
    gap_class_1_signals,
    2,
    NULL,
    0
},
{
    "ASPasswordCredential",
    "RefCounted",
    gap_class_2_methods,
    2,
    gap_class_2_properties,
    2,
    NULL,
    0,
    NULL,
    0
},
{
    "ASWebAuthenticationSession",
    "RefCounted",
    gap_class_3_methods,
    2,
    NULL,
    0,
    gap_class_3_signals,
    3,
    NULL,
    0
},
{
    "AVAudioSession",
    "RefCounted",
    gap_class_4_methods,
    3,
    gap_class_4_properties,
    1,
    NULL,
    0,
    gap_class_4_constants,
    27
},
{
    "AppleFilePicker",
    "RefCounted",
    gap_class_5_methods,
    1,
    NULL,
    0,
    gap_class_5_signals,
    3,
    NULL,
    0
},
{
    "AppleURL",
    "RefCounted",
    gap_class_6_methods,
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
    gap_class_7_methods,
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
    gap_class_8_methods,
    22,
    gap_class_8_properties,
    7,
    NULL,
    0,
    gap_class_8_constants,
    4
},
{
    "GKAchievement",
    "RefCounted",
    gap_class_9_methods,
    16,
    gap_class_9_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKChallenge",
    "RefCounted",
    gap_class_10_methods,
    9,
    gap_class_10_properties,
    7,
    NULL,
    0,
    gap_class_10_constants,
    7
},
{
    "GKAchievementChallenge",
    "GKChallenge",
    gap_class_11_methods,
    1,
    gap_class_11_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "GKAchievementDescription",
    "RefCounted",
    gap_class_12_methods,
    16,
    gap_class_12_properties,
    12,
    NULL,
    0,
    NULL,
    0
},
{
    "GKChallengeDefinition",
    "RefCounted",
    gap_class_13_methods,
    11,
    gap_class_13_properties,
    8,
    NULL,
    0,
    NULL,
    0
},
{
    "GKError",
    "RefCounted",
    gap_class_14_methods,
    6,
    gap_class_14_properties,
    3,
    NULL,
    0,
    gap_class_14_constants,
    42
},
{
    "GKGameActivity",
    "RefCounted",
    gap_class_15_methods,
    35,
    gap_class_15_properties,
    13,
    NULL,
    0,
    NULL,
    0
},
{
    "GKGameActivityDefinition",
    "RefCounted",
    gap_class_16_methods,
    17,
    gap_class_16_properties,
    12,
    NULL,
    0,
    NULL,
    0
},
{
    "GKGameCenterViewController",
    "RefCounted",
    gap_class_17_methods,
    6,
    NULL,
    0,
    NULL,
    0,
    gap_class_17_constants,
    6
},
{
    "GKInvite",
    "RefCounted",
    gap_class_18_methods,
    4,
    gap_class_18_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboard",
    "RefCounted",
    gap_class_19_methods,
    18,
    gap_class_19_properties,
    12,
    NULL,
    0,
    gap_class_19_constants,
    8
},
{
    "GKLeaderboardEntry",
    "RefCounted",
    gap_class_20_methods,
    6,
    gap_class_20_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboardScore",
    "RefCounted",
    gap_class_21_methods,
    8,
    gap_class_21_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLeaderboardSet",
    "RefCounted",
    gap_class_22_methods,
    6,
    gap_class_22_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "GKPlayer",
    "RefCounted",
    gap_class_23_methods,
    9,
    gap_class_23_properties,
    6,
    NULL,
    0,
    NULL,
    0
},
{
    "GKLocalPlayer",
    "GKPlayer",
    gap_class_24_methods,
    18,
    gap_class_24_properties,
    4,
    gap_class_24_signals,
    14,
    NULL,
    0
},
{
    "GKMatch",
    "RefCounted",
    gap_class_25_methods,
    12,
    gap_class_25_properties,
    5,
    gap_class_25_signals,
    4,
    gap_class_25_constants,
    2
},
{
    "GKMatchRequest",
    "RefCounted",
    gap_class_26_methods,
    23,
    gap_class_26_properties,
    11,
    NULL,
    0,
    gap_class_26_constants,
    10
},
{
    "GKMatchmaker",
    "RefCounted",
    gap_class_27_methods,
    15,
    NULL,
    0,
    gap_class_27_signals,
    2,
    NULL,
    0
},
{
    "GKMatchmakerViewController",
    "RefCounted",
    gap_class_28_methods,
    15,
    gap_class_28_properties,
    5,
    gap_class_28_signals,
    5,
    NULL,
    0
},
{
    "GKNotificationBanner",
    "RefCounted",
    gap_class_29_methods,
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
    gap_class_30_methods,
    4,
    gap_class_30_properties,
    3,
    NULL,
    0,
    NULL,
    0
},
{
    "GKScoreChallenge",
    "GKChallenge",
    gap_class_31_methods,
    7,
    gap_class_31_properties,
    7,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedExchange",
    "RefCounted",
    gap_class_32_methods,
    12,
    gap_class_32_properties,
    10,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedExchangeReply",
    "RefCounted",
    gap_class_33_methods,
    4,
    gap_class_33_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedMatch",
    "RefCounted",
    gap_class_34_methods,
    35,
    gap_class_34_properties,
    14,
    NULL,
    0,
    NULL,
    0
},
{
    "GKTurnBasedMatchmakerViewController",
    "RefCounted",
    gap_class_35_methods,
    7,
    gap_class_35_properties,
    2,
    gap_class_35_signals,
    4,
    gap_class_35_constants,
    4
},
{
    "GKTurnBasedParticipant",
    "RefCounted",
    gap_class_36_methods,
    6,
    gap_class_36_properties,
    5,
    NULL,
    0,
    NULL,
    0
},
{
    "GKVoiceChat",
    "RefCounted",
    gap_class_37_methods,
    10,
    gap_class_37_properties,
    4,
    gap_class_37_signals,
    1,
    NULL,
    0
},
{
    "GameCenterManager",
    "RefCounted",
    gap_class_38_methods,
    4,
    gap_class_38_properties,
    2,
    gap_class_38_signals,
    2,
    NULL,
    0
},
{
    "ProductView",
    "RefCounted",
    gap_class_39_methods,
    10,
    gap_class_39_properties,
    4,
    NULL,
    0,
    gap_class_39_constants,
    4
},
{
    "SignalProxy",
    "Object",
    gap_class_40_methods,
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
    gap_class_41_methods,
    6,
    NULL,
    0,
    gap_class_41_signals,
    7,
    gap_class_41_constants,
    14
},
{
    "StoreProduct",
    "RefCounted",
    gap_class_42_methods,
    7,
    gap_class_42_properties,
    7,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreProductPaymentMode",
    "RefCounted",
    gap_class_43_methods,
    4,
    gap_class_43_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreProductPurchaseOption",
    "RefCounted",
    gap_class_44_methods,
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
    gap_class_45_constants,
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
    gap_class_46_constants,
    4
},
{
    "StoreSubscriptionInfo",
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
    "StoreSubscriptionInfoRenewalInfo",
    "RefCounted",
    gap_class_48_methods,
    4,
    gap_class_48_properties,
    4,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreSubscriptionInfoStatus",
    "RefCounted",
    gap_class_49_methods,
    3,
    gap_class_49_properties,
    3,
    NULL,
    0,
    gap_class_49_constants,
    6
},
{
    "StoreTransaction",
    "RefCounted",
    gap_class_50_methods,
    9,
    gap_class_50_properties,
    8,
    NULL,
    0,
    NULL,
    0
},
{
    "StoreView",
    "RefCounted",
    gap_class_51_methods,
    4,
    gap_class_51_properties,
    1,
    NULL,
    0,
    NULL,
    0
},
{
    "SubscriptionOfferView",
    "RefCounted",
    gap_class_52_methods,
    4,
    gap_class_52_properties,
    1,
    gap_class_52_signals,
    2,
    NULL,
    0
},
{
    "SubscriptionStoreView",
    "RefCounted",
    gap_class_53_methods,
    8,
    gap_class_53_properties,
    3,
    NULL,
    0,
    gap_class_53_constants,
    7
},
};
static const size_t gap_class_count = sizeof(gap_classes) / sizeof(gap_classes[0]);

static void gap_register_method(const GAPStubMethodDescriptor *method, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage method_name;
    GAPStubPropertyInfoTemp return_temp;
    GAPStubPropertyInfoTemp *argument_temps = NULL;
    GDExtensionPropertyInfo return_info;
    GDExtensionPropertyInfo *arguments_info = NULL;
    GDExtensionClassMethodArgumentMetadata *arguments_metadata = NULL;
    GDExtensionClassMethodInfo method_info;
    uint32_t index;

    memset(&return_temp, 0, sizeof(return_temp));
    memset(&return_info, 0, sizeof(return_info));
    memset(&method_info, 0, sizeof(method_info));

    gap_string_name_init(&method_name, method->name);
    method_info.name = (GDExtensionStringNamePtr)&method_name;
    method_info.method_userdata = (void *)method;
    method_info.call_func = gap_stub_method_call;
    method_info.ptrcall_func = NULL;
    method_info.method_flags = method->flags;
    method_info.has_return_value = method->has_return_value;
    method_info.return_value_metadata = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;

    if (method->has_return_value) {
        gap_property_info_init(&return_temp, &return_info, "return_value", &method->return_type);
        method_info.return_value_info = &return_info;
    }

    if (method->argument_count > 0) {
        argument_temps = (GAPStubPropertyInfoTemp *)gap_api.mem_alloc(sizeof(GAPStubPropertyInfoTemp) * method->argument_count);
        arguments_info = (GDExtensionPropertyInfo *)gap_api.mem_alloc(sizeof(GDExtensionPropertyInfo) * method->argument_count);
        arguments_metadata = (GDExtensionClassMethodArgumentMetadata *)gap_api.mem_alloc(sizeof(GDExtensionClassMethodArgumentMetadata) * method->argument_count);

        if (argument_temps == NULL || arguments_info == NULL || arguments_metadata == NULL) {
            gap_report_unimplemented(method->display_name);
            goto cleanup;
        }

        memset(argument_temps, 0, sizeof(GAPStubPropertyInfoTemp) * method->argument_count);
        memset(arguments_info, 0, sizeof(GDExtensionPropertyInfo) * method->argument_count);

        for (index = 0; index < method->argument_count; ++index) {
            gap_property_info_init(&argument_temps[index], &arguments_info[index], method->arguments[index].name, &method->arguments[index].type);
            arguments_metadata[index] = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
        }

        method_info.argument_count = method->argument_count;
        method_info.arguments_info = arguments_info;
        method_info.arguments_metadata = arguments_metadata;
    }

    gap_api.classdb_register_extension_class_method(gap_library, class_name, &method_info);

cleanup:
    if (method->argument_count > 0 && argument_temps != NULL) {
        for (index = 0; index < method->argument_count; ++index) {
            gap_property_info_destroy(&argument_temps[index]);
        }
    }

    if (arguments_metadata != NULL) {
        gap_api.mem_free(arguments_metadata);
    }
    if (arguments_info != NULL) {
        gap_api.mem_free(arguments_info);
    }
    if (argument_temps != NULL) {
        gap_api.mem_free(argument_temps);
    }
    if (method->has_return_value) {
        gap_property_info_destroy(&return_temp);
    }
    gap_string_name_destroy(&method_name);
}

static void gap_register_property(const GAPStubPropertyDescriptor *property, GDExtensionConstStringNamePtr class_name) {
    GAPStubPropertyInfoTemp property_temp;
    GAPStubStringNameStorage setter_name;
    GAPStubStringNameStorage getter_name;
    GDExtensionPropertyInfo property_info;

    memset(&property_temp, 0, sizeof(property_temp));
    memset(&property_info, 0, sizeof(property_info));

    gap_property_info_init(&property_temp, &property_info, property->name, &property->type);
    gap_string_name_init(&setter_name, property->setter);
    gap_string_name_init(&getter_name, property->getter);

    gap_api.classdb_register_extension_class_property(
        gap_library,
        class_name,
        &property_info,
        (GDExtensionConstStringNamePtr)&setter_name,
        (GDExtensionConstStringNamePtr)&getter_name
    );

    gap_string_name_destroy(&getter_name);
    gap_string_name_destroy(&setter_name);
    gap_property_info_destroy(&property_temp);
}

static void gap_register_signal(const GAPStubSignalDescriptor *signal, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage signal_name;
    GAPStubPropertyInfoTemp *argument_temps = NULL;
    GDExtensionPropertyInfo *arguments = NULL;
    uint32_t index;

    gap_string_name_init(&signal_name, signal->name);

    if (signal->argument_count > 0) {
        argument_temps = (GAPStubPropertyInfoTemp *)gap_api.mem_alloc(sizeof(GAPStubPropertyInfoTemp) * signal->argument_count);
        arguments = (GDExtensionPropertyInfo *)gap_api.mem_alloc(sizeof(GDExtensionPropertyInfo) * signal->argument_count);

        if (argument_temps == NULL || arguments == NULL) {
            gap_report_unimplemented(signal->name);
            goto cleanup;
        }

        memset(argument_temps, 0, sizeof(GAPStubPropertyInfoTemp) * signal->argument_count);
        memset(arguments, 0, sizeof(GDExtensionPropertyInfo) * signal->argument_count);

        for (index = 0; index < signal->argument_count; ++index) {
            gap_property_info_init(&argument_temps[index], &arguments[index], signal->arguments[index].name, &signal->arguments[index].type);
        }
    }

    gap_api.classdb_register_extension_class_signal(
        gap_library,
        class_name,
        (GDExtensionConstStringNamePtr)&signal_name,
        arguments,
        signal->argument_count
    );

cleanup:
    if (signal->argument_count > 0 && argument_temps != NULL) {
        for (index = 0; index < signal->argument_count; ++index) {
            gap_property_info_destroy(&argument_temps[index]);
        }
    }
    if (arguments != NULL) {
        gap_api.mem_free(arguments);
    }
    if (argument_temps != NULL) {
        gap_api.mem_free(argument_temps);
    }
    gap_string_name_destroy(&signal_name);
}

static void gap_register_constant(const GAPStubConstantDescriptor *constant, GDExtensionConstStringNamePtr class_name) {
    GAPStubStringNameStorage enum_name;
    GAPStubStringNameStorage constant_name;

    gap_string_name_init(&enum_name, constant->enum_name);
    gap_string_name_init(&constant_name, constant->name);

    gap_api.classdb_register_extension_class_integer_constant(
        gap_library,
        class_name,
        (GDExtensionConstStringNamePtr)&enum_name,
        (GDExtensionConstStringNamePtr)&constant_name,
        constant->value,
        constant->is_bitfield
    );

    gap_string_name_destroy(&constant_name);
    gap_string_name_destroy(&enum_name);
}

static void gap_register_class_shell(const GAPStubClassDescriptor *descriptor) {
    GAPStubStringNameStorage class_name;
    GAPStubStringNameStorage parent_name;
    GDExtensionClassCreationInfo2 class_info;

    memset(&class_info, 0, sizeof(class_info));
    gap_string_name_init(&class_name, descriptor->name);
    gap_string_name_init(&parent_name, descriptor->parent_name);

    class_info.is_exposed = 1;
    class_info.create_instance_func = gap_stub_create_instance;
    class_info.free_instance_func = gap_stub_free_instance;
    class_info.class_userdata = (void *)descriptor;

    gap_api.classdb_register_extension_class2(
        gap_library,
        (GDExtensionConstStringNamePtr)&class_name,
        (GDExtensionConstStringNamePtr)&parent_name,
        &class_info
    );

    gap_string_name_destroy(&parent_name);
    gap_string_name_destroy(&class_name);
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
    GAP_LOAD_REQUIRED(classdb_construct_object, "classdb_construct_object", GDExtensionInterfaceClassdbConstructObject);
    GAP_LOAD_REQUIRED(classdb_register_extension_class2, "classdb_register_extension_class2", GDExtensionInterfaceClassdbRegisterExtensionClass2);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_method, "classdb_register_extension_class_method", GDExtensionInterfaceClassdbRegisterExtensionClassMethod);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_property, "classdb_register_extension_class_property", GDExtensionInterfaceClassdbRegisterExtensionClassProperty);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_signal, "classdb_register_extension_class_signal", GDExtensionInterfaceClassdbRegisterExtensionClassSignal);
    GAP_LOAD_REQUIRED(classdb_register_extension_class_integer_constant, "classdb_register_extension_class_integer_constant", GDExtensionInterfaceClassdbRegisterExtensionClassIntegerConstant);
    GAP_LOAD_REQUIRED(object_set_instance, "object_set_instance", GDExtensionInterfaceObjectSetInstance);
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

GDExtensionBool godot_apple_plugins_stub_init(
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