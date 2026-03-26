import Foundation

struct Options {
    var baseDirectory: String = "."
    var docsPath: String = "doc_classes"
    var outputPath: String = "Generated/GodotApplePluginsStub"
    var entrySymbol: String = "godot_apple_plugins_stub_init"
    var libraryBasename: String = "godot_apple_plugins_stub"
    var selectedFiles: [String] = []

    init(arguments: [String]) throws {
        var iterator = arguments.dropFirst().makeIterator()
        while let argument = iterator.next() {
            switch argument {
            case "--directory", "--base-directory", "--project-dir":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for \(argument)")
                }
                baseDirectory = value
            case "--file":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --file")
                }
                selectedFiles.append(value)
            case "--files":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --files")
                }
                selectedFiles.append(contentsOf: value.split(separator: ",").map { String($0) }.filter { !$0.isEmpty })
            case "--docs":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --docs")
                }
                docsPath = value
            case "--output":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --output")
                }
                outputPath = value
            case "--entry-symbol":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --entry-symbol")
                }
                entrySymbol = value
            case "--library-name":
                guard let value = iterator.next() else {
                    throw GeneratorError.usage("Missing value for --library-name")
                }
                libraryBasename = value
            case "--help", "-h":
                throw GeneratorError.usage(nil)
            default:
                if argument.hasPrefix("-") {
                    throw GeneratorError.usage("Unknown argument: \(argument)")
                }
                baseDirectory = argument
            }
        }
    }

    func resolvedBaseDirectory(from currentDirectory: URL) -> URL {
        resolvePath(baseDirectory, relativeTo: currentDirectory, isDirectory: true)
    }

    func resolvedDocsURL(from currentDirectory: URL) -> URL {
        let base = resolvedBaseDirectory(from: currentDirectory)
        return resolvePath(docsPath, relativeTo: base, isDirectory: true)
    }

    func resolvedOutputURL(from currentDirectory: URL) -> URL {
        let base = resolvedBaseDirectory(from: currentDirectory)
        return resolvePath(outputPath, relativeTo: base, isDirectory: true)
    }

    private func resolvePath(_ path: String, relativeTo base: URL, isDirectory: Bool) -> URL {
        let candidate = URL(fileURLWithPath: path, isDirectory: isDirectory)
        if candidate.path.hasPrefix("/") {
            return candidate.standardizedFileURL
        }
        return URL(fileURLWithPath: path, relativeTo: base).standardizedFileURL
    }

    static let usage = """
    Usage:
      swift run GodotApplePluginsStubGenerator [directory] [options]

    Options:
      --directory <path>     Base directory for docs/output paths. Default: current directory
      --file <path>          Include one XML file or class name. Repeatable.
      --files <a,b,c>        Comma-separated XML files or class names to include.
      --docs <path>          XML documentation directory. Default: doc_classes
      --output <path>        Output directory. Default: Generated/GodotApplePluginsStub
      --entry-symbol <name>  GDExtension entry symbol. Default: godot_apple_plugins_stub_init
      --library-name <name>  Basename for generated files/library. Default: godot_apple_plugins_stub
    """
}

enum GeneratorError: Error, CustomStringConvertible {
    case usage(String?)
    case message(String)

    var description: String {
        switch self {
        case .usage(let message):
            if let message {
                return "\(message)\n\n\(Options.usage)"
            }
            return Options.usage
        case .message(let message):
            return message
        }
    }
}

struct GodotTypeInfo {
    let variantType: String
    let className: String
    let usageExpression: String
    let hintExpression: String = "GAP_PROPERTY_HINT_NONE"
    let hintString: String = ""
}

struct NamedType {
    let name: String
    let typeName: String
}

struct DocMethod {
    let name: String
    let isStatic: Bool
    let returnType: String?
    let arguments: [NamedType]
    let synthesizedFromMember: Bool
}

struct DocMember {
    let name: String
    let typeName: String
    let setter: String
    let getter: String
}

struct DocSignal {
    let name: String
    let arguments: [NamedType]
}

struct DocConstant {
    let enumName: String
    let name: String
    let value: Int64
}

struct DocClass {
    let name: String
    let parentName: String
    let sourceFileName: String
    let methods: [DocMethod]
    let members: [DocMember]
    let signals: [DocSignal]
    let constants: [DocConstant]
}

private extension XMLElement {
    var requiredName: String {
        name ?? "<unknown>"
    }

    func attr(_ name: String) -> String? {
        attribute(forName: name)?.stringValue
    }

    func childElements(named name: String) -> [XMLElement] {
        (children ?? []).compactMap { $0 as? XMLElement }.filter { $0.name == name }
    }

    func firstChild(named name: String) -> XMLElement? {
        childElements(named: name).first
    }
}

enum DocumentationParser {
    static func parseClasses(in directory: URL) throws -> [DocClass] {
        let fileManager = FileManager.default
        guard let enumerator = fileManager.enumerator(at: directory, includingPropertiesForKeys: nil) else {
            throw GeneratorError.message("Unable to enumerate documentation directory: \(directory.path)")
        }

        let files = enumerator.compactMap { $0 as? URL }.filter { $0.pathExtension == "xml" }.sorted { $0.lastPathComponent < $1.lastPathComponent }
        guard !files.isEmpty else {
            throw GeneratorError.message("No XML files found in \(directory.path)")
        }

        return try files.map(parseClass(from:)).map(synthesizingAccessors(for:))
    }

    private static func parseClass(from fileURL: URL) throws -> DocClass {
        let document = try XMLDocument(contentsOf: fileURL, options: [])
        guard let root = document.rootElement(), root.requiredName == "class" else {
            throw GeneratorError.message("Invalid class XML: \(fileURL.lastPathComponent)")
        }

        guard let className = root.attr("name"), let parentName = root.attr("inherits") else {
            throw GeneratorError.message("Missing class metadata in \(fileURL.lastPathComponent)")
        }

        let methods = root.firstChild(named: "methods")?.childElements(named: "method").map(parseMethod) ?? []
        let members = root.firstChild(named: "members")?.childElements(named: "member").map(parseMember) ?? []
        let signals = root.firstChild(named: "signals")?.childElements(named: "signal").map(parseSignal) ?? []
        let constants = root.firstChild(named: "constants")?.childElements(named: "constant").compactMap(parseConstant) ?? []

        return DocClass(
            name: className,
            parentName: parentName,
            sourceFileName: fileURL.lastPathComponent,
            methods: methods,
            members: members,
            signals: signals,
            constants: constants
        )
    }

    private static func parseMethod(_ element: XMLElement) -> DocMethod {
        let returnType = element.firstChild(named: "return")?.attr("type")
        return DocMethod(
            name: element.attr("name") ?? "",
            isStatic: element.attr("qualifiers")?.contains("static") == true,
            returnType: returnType == "void" ? nil : returnType,
            arguments: parseParameters(in: element),
            synthesizedFromMember: false
        )
    }

    private static func parseMember(_ element: XMLElement) -> DocMember {
        DocMember(
            name: element.attr("name") ?? "",
            typeName: element.attr("type") ?? "Variant",
            setter: element.attr("setter") ?? "",
            getter: element.attr("getter") ?? ""
        )
    }

    private static func parseSignal(_ element: XMLElement) -> DocSignal {
        DocSignal(
            name: element.attr("name") ?? "",
            arguments: parseParameters(in: element)
        )
    }

    private static func parseConstant(_ element: XMLElement) -> DocConstant? {
        guard
            let enumName = element.attr("enum"),
            let name = element.attr("name"),
            let valueString = element.attr("value"),
            let value = Int64(valueString)
        else {
            return nil
        }

        return DocConstant(enumName: enumName, name: name, value: value)
    }

    private static func parseParameters(in element: XMLElement) -> [NamedType] {
        element.childElements(named: "param").sorted { lhs, rhs in
            Int(lhs.attr("index") ?? "0") ?? 0 < Int(rhs.attr("index") ?? "0") ?? 0
        }.map {
            NamedType(name: $0.attr("name") ?? "arg", typeName: $0.attr("type") ?? "Variant")
        }
    }

    private static func synthesizingAccessors(for docClass: DocClass) -> DocClass {
        var methods = docClass.methods
        var existingNames = Set(methods.map(\.name))

        for member in docClass.members {
            if !member.getter.isEmpty && !existingNames.contains(member.getter) {
                methods.append(
                    DocMethod(
                        name: member.getter,
                        isStatic: false,
                        returnType: member.typeName,
                        arguments: [],
                        synthesizedFromMember: true
                    )
                )
                existingNames.insert(member.getter)
            }

            if !member.setter.isEmpty && !existingNames.contains(member.setter) {
                methods.append(
                    DocMethod(
                        name: member.setter,
                        isStatic: false,
                        returnType: nil,
                        arguments: [NamedType(name: member.name, typeName: member.typeName)],
                        synthesizedFromMember: true
                    )
                )
                existingNames.insert(member.setter)
            }
        }

        return DocClass(
            name: docClass.name,
            parentName: docClass.parentName,
            sourceFileName: docClass.sourceFileName,
            methods: methods,
            members: docClass.members,
            signals: docClass.signals,
            constants: docClass.constants
        )
    }
}

enum GeneratorModel {
    static func selectClasses(_ classes: [DocClass], requestedFiles: [String]) throws -> [DocClass] {
        guard !requestedFiles.isEmpty else { return classes }

        let byClassName = Dictionary(uniqueKeysWithValues: classes.map { ($0.name, $0) })

        var bySelector: [String: DocClass] = [:]
        for docClass in classes {
            for key in selectionKeys(for: docClass.sourceFileName, className: docClass.name) {
                bySelector[key] = docClass
            }
        }

        var requestedClassNames = Set<String>()
        for request in requestedFiles {
            let trimmed = request.trimmingCharacters(in: .whitespacesAndNewlines)
            guard !trimmed.isEmpty else { continue }

            let url = URL(fileURLWithPath: trimmed)
            let candidates = [
                trimmed,
                url.lastPathComponent,
                URL(fileURLWithPath: url.lastPathComponent).deletingPathExtension().lastPathComponent
            ]

            guard let matched = candidates.compactMap({ bySelector[$0] }).first else {
                let available = classes
                    .map { "\($0.sourceFileName) (\($0.name))" }
                    .sorted()
                    .joined(separator: ", ")
                throw GeneratorError.message("Requested file or class '\(trimmed)' was not found. Available entries: \(available)")
            }

            requestedClassNames.insert(matched.name)
        }

        var expanded = requestedClassNames
        var worklist = Array(requestedClassNames)
        while let name = worklist.popLast() {
            guard let docClass = byClassName[name] else { continue }
            if let parent = byClassName[docClass.parentName], expanded.insert(parent.name).inserted {
                worklist.append(parent.name)
            }
        }

        return classes.filter { expanded.contains($0.name) }
    }

    static func topologicallySorted(_ classes: [DocClass]) throws -> [DocClass] {
        let byName = Dictionary(uniqueKeysWithValues: classes.map { ($0.name, $0) })
        var visiting = Set<String>()
        var visited = Set<String>()
        var ordered: [DocClass] = []

        func visit(_ name: String) throws {
            guard let docClass = byName[name] else { return }
            if visited.contains(name) { return }
            if !visiting.insert(name).inserted {
                throw GeneratorError.message("Detected inheritance cycle involving \(name)")
            }

            if byName[docClass.parentName] != nil {
                try visit(docClass.parentName)
            }

            visiting.remove(name)
            visited.insert(name)
            ordered.append(docClass)
        }

        for docClass in classes.sorted(by: { $0.name < $1.name }) {
            try visit(docClass.name)
        }

        return ordered
    }

    private static func selectionKeys(for sourceFileName: String, className: String) -> [String] {
        let basename = URL(fileURLWithPath: sourceFileName).deletingPathExtension().lastPathComponent
        return [sourceFileName, basename, className]
    }

    static func mapType(_ name: String) -> GodotTypeInfo {
        switch name {
        case "bool":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_BOOL", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "int":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_INT", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "float":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_FLOAT", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "String":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_STRING", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "Callable":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_CALLABLE", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "Dictionary", "Dictionary[String, String]":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_DICTIONARY", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "Array":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_ARRAY", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "PackedByteArray":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "PackedStringArray":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "Rect2":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_RECT2", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        case "Variant":
            return GodotTypeInfo(
                variantType: "GDEXTENSION_VARIANT_TYPE_NIL",
                className: "",
                usageExpression: "GAP_PROPERTY_USAGE_DEFAULT | GAP_PROPERTY_USAGE_NIL_IS_VARIANT"
            )
        case "void":
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_NIL", className: "", usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        default:
            return GodotTypeInfo(variantType: "GDEXTENSION_VARIANT_TYPE_OBJECT", className: name, usageExpression: "GAP_PROPERTY_USAGE_DEFAULT")
        }
    }
}

enum CEmitter {
    static func emitHeader(entrySymbol: String) -> String {
        """
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

        GAP_STUB_EXPORT GDExtensionBool \(entrySymbol)(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization);

        #ifdef __cplusplus
        }
        #endif

        #endif
        """
    }

    static func emitCSource(classes: [DocClass], headerName: String, entrySymbol: String) -> String {
        var lines: [String] = []

        lines.append("""
        #include "\(headerName)"

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

        """)

        for (classIndex, docClass) in classes.enumerated() {
            for (methodIndex, method) in docClass.methods.enumerated() {
                guard !method.arguments.isEmpty else { continue }
                let argsName = "gap_class_\(classIndex)_method_\(methodIndex)_args"
                lines.append("static const GAPStubNamedTypeInfo \(argsName)[] = {")
                for argument in method.arguments {
                    let mapped = GeneratorModel.mapType(argument.typeName)
                    lines.append("    { \(cString(argument.name)), { \(mapped.variantType), \(cString(mapped.className)), \(mapped.hintExpression), \(cString(mapped.hintString)), \(mapped.usageExpression) } },")
                }
                lines.append("};")
            }

            for (signalIndex, signal) in docClass.signals.enumerated() {
                guard !signal.arguments.isEmpty else { continue }
                let argsName = "gap_class_\(classIndex)_signal_\(signalIndex)_args"
                lines.append("static const GAPStubNamedTypeInfo \(argsName)[] = {")
                for argument in signal.arguments {
                    let mapped = GeneratorModel.mapType(argument.typeName)
                    lines.append("    { \(cString(argument.name)), { \(mapped.variantType), \(cString(mapped.className)), \(mapped.hintExpression), \(cString(mapped.hintString)), \(mapped.usageExpression) } },")
                }
                lines.append("};")
            }

            let methodsName = "gap_class_\(classIndex)_methods"
            if !docClass.methods.isEmpty {
                lines.append("static const GAPStubMethodDescriptor \(methodsName)[] = {")
                for (methodIndex, method) in docClass.methods.enumerated() {
                    let mappedReturn = GeneratorModel.mapType(method.returnType ?? "void")
                    let flags = method.isStatic
                        ? "GDEXTENSION_METHOD_FLAG_NORMAL | GDEXTENSION_METHOD_FLAG_STATIC"
                        : "GDEXTENSION_METHOD_FLAG_NORMAL"
                    let displayName = "\(docClass.name).\(method.name)"
                    let argsName = "gap_class_\(classIndex)_method_\(methodIndex)_args"
                    lines.append("""
                        {
                            \(cString(method.name)),
                            \(cString(displayName)),
                            \(flags),
                            \(method.returnType == nil ? "0" : "1"),
                            { \(mappedReturn.variantType), \(cString(mappedReturn.className)), \(mappedReturn.hintExpression), \(cString(mappedReturn.hintString)), \(mappedReturn.usageExpression) },
                            \(method.arguments.isEmpty ? "NULL" : argsName),
                            \(method.arguments.count)
                        },
                        """)
                }
                lines.append("};")
            }

            let propertiesName = "gap_class_\(classIndex)_properties"
            if !docClass.members.isEmpty {
                lines.append("static const GAPStubPropertyDescriptor \(propertiesName)[] = {")
                for member in docClass.members {
                    let mapped = GeneratorModel.mapType(member.typeName)
                    lines.append("""
                        {
                            \(cString(member.name)),
                            { \(mapped.variantType), \(cString(mapped.className)), \(mapped.hintExpression), \(cString(mapped.hintString)), \(mapped.usageExpression) },
                            \(cString(member.setter)),
                            \(cString(member.getter))
                        },
                        """)
                }
                lines.append("};")
            }

            let signalsName = "gap_class_\(classIndex)_signals"
            if !docClass.signals.isEmpty {
                lines.append("static const GAPStubSignalDescriptor \(signalsName)[] = {")
                for (signalIndex, signal) in docClass.signals.enumerated() {
                    let argsName = "gap_class_\(classIndex)_signal_\(signalIndex)_args"
                    lines.append("""
                        {
                            \(cString(signal.name)),
                            \(signal.arguments.isEmpty ? "NULL" : argsName),
                            \(signal.arguments.count)
                        },
                        """)
                }
                lines.append("};")
            }

            let constantsName = "gap_class_\(classIndex)_constants"
            if !docClass.constants.isEmpty {
                lines.append("static const GAPStubConstantDescriptor \(constantsName)[] = {")
                for constant in docClass.constants {
                    lines.append("""
                        { \(cString(constant.enumName)), \(cString(constant.name)), \(constant.value), 0 },
                        """)
                }
                lines.append("};")
            }
        }

        lines.append("static const GAPStubClassDescriptor gap_classes[] = {")
        for (classIndex, docClass) in classes.enumerated() {
            lines.append("""
                {
                    \(cString(docClass.name)),
                    \(cString(docClass.parentName)),
                    \(docClass.methods.isEmpty ? "NULL" : "gap_class_\(classIndex)_methods"),
                    \(docClass.methods.count),
                    \(docClass.members.isEmpty ? "NULL" : "gap_class_\(classIndex)_properties"),
                    \(docClass.members.count),
                    \(docClass.signals.isEmpty ? "NULL" : "gap_class_\(classIndex)_signals"),
                    \(docClass.signals.count),
                    \(docClass.constants.isEmpty ? "NULL" : "gap_class_\(classIndex)_constants"),
                    \(docClass.constants.count)
                },
                """)
        }
        lines.append("};")

        lines.append("""
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
        #define GAP_LOAD_REQUIRED(field, symbol, type) \
            do { \
                gap_api.field = (type)p_get_proc_address(symbol); \
                if (gap_api.field == NULL) { \
                    return 0; \
                } \
            } while (0)

        #define GAP_LOAD_OPTIONAL(field, symbol, type) \
            do { \
                gap_api.field = (type)p_get_proc_address(symbol); \
            } while (0)

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

        GDExtensionBool \(entrySymbol)(
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
        """)

        return lines.joined(separator: "\n")
    }

    static func emitMakefile(libraryBasename: String, sourceName: String) -> String {
        """
        CC ?= cc
        GODOT_HEADERS_DIR ?= /path/to/godot-cpp/gdextension
        CFLAGS ?= -std=c11 -Wall -Wextra

        ifeq ($(OS),Windows_NT)
        SHARED_EXT := dll
        SHARED_FLAGS := -shared
        else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S),Darwin)
        SHARED_EXT := dylib
        SHARED_FLAGS := -dynamiclib
        else
        SHARED_EXT := so
        SHARED_FLAGS := -shared -fPIC
        endif
        endif

        LIBRARY := \(libraryBasename).$(SHARED_EXT)

        .PHONY: all clean

        all: $(LIBRARY)

        $(LIBRARY): \(sourceName)
        	$(CC) $(CFLAGS) -I "$(GODOT_HEADERS_DIR)" $(SHARED_FLAGS) $< -o $@

        clean:
        	rm -f $(LIBRARY)
        """
    }

    static func emitReadme(options: Options, classes: [DocClass], sourceName: String, headerName: String) -> String {
        let methodCount = classes.reduce(0) { $0 + $1.methods.count }
        let memberCount = classes.reduce(0) { $0 + $1.members.count }
        let signalCount = classes.reduce(0) { $0 + $1.signals.count }
        let constantCount = classes.reduce(0) { $0 + $1.constants.count }
        let regenerateBase = shellEscape(options.baseDirectory == "." ? "." : options.baseDirectory)
        let regenerateOutput = shellEscape(options.outputPath)
        let selectionText: String
        if options.selectedFiles.isEmpty {
            selectionText = "This run used the full `doc_classes/*.xml` set."
        } else {
            let selected = options.selectedFiles.joined(separator: ", ")
            selectionText = "This run was filtered to: `\(selected)` (plus any documented parent classes required for inheritance)."
        }

        return """
        # GodotApplePlugins Stub Library

        This directory was generated by `GodotApplePluginsStubGenerator`.

        It mirrors the public API described in `doc_classes/*.xml` and emits a C GDExtension stub library intended for non-Apple targets such as Windows and Linux.

        \(selectionText)

        ## Generated Surface

        - Classes: \(classes.count)
        - Methods: \(methodCount)
        - Members: \(memberCount)
        - Signals: \(signalCount)
        - Integer constants: \(constantCount)

        ## Behavior

        - All documented classes are registered in ClassDB.
        - All documented methods, synthesized member accessors, properties, signals, and enum constants are registered.
        - Every callable entry point reports that the API is unavailable on the current platform and returns an unimplemented stub result.

        ## Files

        - `\(sourceName)`: generated GDExtension implementation
        - `\(headerName)`: generated public header with entry symbol `\(options.entrySymbol)`
        - `Makefile`: minimal shared-library build scaffold

        ## Build

        ```sh
        make GODOT_HEADERS_DIR=/path/to/godot-cpp/gdextension
        ```

        ## Regenerate

        You can point the generator at another checkout directory instead of changing directories first.

        ```sh
        swift run GodotApplePluginsStubGenerator \(regenerateBase) --output \(regenerateOutput)
        ```

        To generate a subset:

        ```sh
        swift run GodotApplePluginsStubGenerator \(regenerateBase) --file GKPlayer.xml --file GKLocalPlayer.xml --output \(regenerateOutput)
        ```
        """
    }

    private static func cString(_ value: String) -> String {
        let escaped = value
            .replacingOccurrences(of: "\\", with: "\\\\")
            .replacingOccurrences(of: "\"", with: "\\\"")
        return "\"\(escaped)\""
    }

    private static func shellEscape(_ value: String) -> String {
        if value.allSatisfy({ $0.isLetter || $0.isNumber || $0 == "/" || $0 == "." || $0 == "_" || $0 == "-" }) {
            return value
        }
        return "'\(value.replacingOccurrences(of: "'", with: "'\"'\"'"))'"
    }
}

@main
struct GodotApplePluginsStubGenerator {
    static func main() {
        do {
            let options = try Options(arguments: CommandLine.arguments)
            let currentDirectory = URL(fileURLWithPath: FileManager.default.currentDirectoryPath, isDirectory: true)
            let docsURL = options.resolvedDocsURL(from: currentDirectory)
            let outputURL = options.resolvedOutputURL(from: currentDirectory)

            let parsedClasses = try DocumentationParser.parseClasses(in: docsURL)
            let selectedClasses = try GeneratorModel.selectClasses(parsedClasses, requestedFiles: options.selectedFiles)
            let orderedClasses = try GeneratorModel.topologicallySorted(selectedClasses)

            let fileManager = FileManager.default
            try fileManager.createDirectory(at: outputURL, withIntermediateDirectories: true, attributes: nil)

            let headerName = "\(options.libraryBasename).h"
            let sourceName = "\(options.libraryBasename).c"

            let header = CEmitter.emitHeader(entrySymbol: options.entrySymbol)
            let source = CEmitter.emitCSource(classes: orderedClasses, headerName: headerName, entrySymbol: options.entrySymbol)
            let makefile = CEmitter.emitMakefile(libraryBasename: options.libraryBasename, sourceName: sourceName)
            let readme = CEmitter.emitReadme(options: options, classes: orderedClasses, sourceName: sourceName, headerName: headerName)

            try header.write(to: outputURL.appendingPathComponent(headerName), atomically: true, encoding: .utf8)
            try source.write(to: outputURL.appendingPathComponent(sourceName), atomically: true, encoding: .utf8)
            try makefile.write(to: outputURL.appendingPathComponent("Makefile"), atomically: true, encoding: .utf8)
            let legacyCMake = outputURL.appendingPathComponent("CMakeLists.txt")
            if FileManager.default.fileExists(atPath: legacyCMake.path) {
                try FileManager.default.removeItem(at: legacyCMake)
            }
            try readme.write(to: outputURL.appendingPathComponent("README.md"), atomically: true, encoding: .utf8)

            let synthesizedMethods = orderedClasses.flatMap(\.methods).filter(\.synthesizedFromMember).count
            FileHandle.standardOutput.write(Data("""
            Generated \(orderedClasses.count) classes into \(outputURL.path)
            Synthesized \(synthesizedMethods) member accessor stubs
            """.utf8))
        } catch {
            FileHandle.standardError.write(Data("\(error)\n".utf8))
            Foundation.exit(1)
        }
    }
}
