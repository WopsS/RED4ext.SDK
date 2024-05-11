#pragma once

#ifdef RED4EXT_STATIC_LIB
#include <RED4ext/Dump/Reflection.hpp>
#endif

#include <RED4ext/RTTISystem.hpp>
#include <RED4ext/Scripting/CProperty.hpp>

#include <array>
#include <fstream>
#include <numeric>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>

namespace RED4ext::GameReflection
{
RED4EXT_INLINE void Dump(std::filesystem::path aOutPath, std::filesystem::path aIncludePath, bool aVerbose,
                         bool aExtendedPath, bool aPropertyHolders)
{
    if (aIncludePath.empty())
    {
        // Use SDK path that was used to build this binary as a fallback
        aIncludePath = std::filesystem::path(__FILE__).parent_path().parent_path();
    }

    auto rttiSystem = RED4ext::CRTTISystem::Get();
    auto* scriptable = rttiSystem->GetClass("IScriptable");
    auto* serializable = rttiSystem->GetClass("ISerializable");
    auto* redEvent = rttiSystem->GetClass("redEvent");

    std::unordered_map<const RED4ext::CClass*, ClassDependencyBuilder> descriptorMap;

    // Collect all the prefixes so we can determine nesting
    std::unordered_map<std::string, std::vector<std::string>> prefixHierarchy;

    // Trim the preceeding lower-case suffix, this seems to be either a namespace or directory, or both
    auto GetPrefix = [](const std::string& aInput) -> std::string {
        size_t i = 0;

        // Special case for AI
        if (aInput.size() >= 2 && aInput[0] == 'A' && aInput[1] == 'I')
        {
            i = 2;
        }

        // Special case of "in", this will break directory layout for "ink", "interop", etc..
        if (aInput.starts_with("inGame"))
        {
            return "";
        }

        for (; i < aInput.size(); ++i)
        {
            if (isupper(aInput[i]))
            {
                break;
            }
        }

        if (i == 0)
            return "";

        return aInput.substr(0, i);
    };

    // First pass gather all properties and descriptors
    rttiSystem->types.for_each([&descriptorMap, GetPrefix, &prefixHierarchy,
                                aPropertyHolders](RED4ext::CName aName, RED4ext::CBaseRTTIType*& aType) {
        if (aType->GetType() == RED4ext::ERTTIType::Class)
        {
            auto classType = static_cast<const RED4ext::CClass*>(aType);
            if (classType->flags.isNative)
            {
                ClassDependencyBuilder builder;
                builder.pType = classType;

                for (uint32_t i = 0; i < classType->unk118.size; ++i)
                {
                    auto prop = classType->unk118.entries[i];
                    if (!prop->flags.inValueHolder)
                    {
                        builder.mPropertyMap.emplace(prop->valueOffset, prop);
                    }
                    else if (aPropertyHolders)
                    {
                        builder.mHolderPropertyMap.emplace(prop->valueOffset, prop);
                    }
                }

                descriptorMap.emplace(classType, builder);
            }
        }

        switch (aType->GetType())
        {
        case RED4ext::ERTTIType::Class:
        case RED4ext::ERTTIType::Enum:
        case RED4ext::ERTTIType::BitField:
        {
            std::string prefix = GetPrefix(aName.ToString());
            if (!prefix.empty())
            {
                prefixHierarchy[prefix] = std::vector<std::string>();
            }
            break;
        }
        default:
            break;
        }
    });

    // Build a mapped list of nested prefixes
    for (auto& [prefix, children] : prefixHierarchy)
    {
        // Special case for localization (prevents loc::alization namespace)
        if (prefix != "localization")
        {
            for (auto i = 0; i < prefix.size(); ++i)
            {
                auto testPrefix = prefix.substr(0, i + 1);

                // Find a substring of ourselves somewhere else in the list
                auto it = prefixHierarchy.find(testPrefix);
                if (it != prefixHierarchy.end())
                {
                    children.push_back(testPrefix);
                }
            }
        }
        else
        {
            children.push_back(prefix);
        }

        for (auto it = children.rbegin(); it != children.rend(); ++it)
        {
            auto next = it;
            if (++next != children.rend())
            {
                *it = it->substr(next->size(), it->size() - next->size());
            }
        }
    }

    // Second pass traverse parents to move properties into parent class if it happened to be abstract
    for (auto& [classType, builder] : descriptorMap)
    {
        std::stack<const RED4ext::CClass*> stack;
        const RED4ext::CClass* parent = classType->parent;
        while (parent)
        {
            stack.push(parent);
            parent = parent->parent;
        }

        auto it = builder.mPropertyMap.begin();
        while (it != builder.mPropertyMap.end() && !stack.empty())
        {
            auto parentClass = stack.top();
            if (it->first < parentClass->GetSize()) // Property offset fits within this parent class
            {
                auto pit = descriptorMap.find(parentClass);
                if (pit != descriptorMap.end())
                {
                    pit->second.mPropertyMap[it->first] = it->second;

                    it = builder.mPropertyMap.erase(it);
                    continue;
                }
            }
            else
            {
                it = builder.mPropertyMap.begin();
                stack.pop();
                continue;
            }

            ++it;
        }
    }

    auto GetGeneratedPath = [aExtendedPath, redEvent, scriptable, serializable, GetPrefix,
                             &prefixHierarchy](const RED4ext::CBaseRTTIType* aType) -> std::string
    {
        auto name = aType->GetName();

        std::string pathPrefix = GetPrefix(name.ToString());
        if (!pathPrefix.empty())
        {
            auto it = prefixHierarchy.find(pathPrefix);
            if (it != prefixHierarchy.end())
            {
                pathPrefix = std::accumulate(it->second.begin(), it->second.end(), std::string(),
                                             [](const std::string& a, const std::string& b) -> std::string {
                                                 return a + (a.length() > 0 ? "/" : "") + b;
                                             });
            }

            pathPrefix += "/";
        }

        // Additional categorization
        if (aExtendedPath)
        {
            if (aType->GetType() == RED4ext::ERTTIType::Class)
            {
                auto* pClass = static_cast<const RED4ext::CClass*>(aType);
                if (pClass->IsA(redEvent))
                {
                    return pathPrefix + "event/";
                }
                else if (pClass->IsA(scriptable))
                {
                    return pathPrefix + "scriptable/";
                }
                else if (pClass->IsA(serializable))
                {
                    return pathPrefix + "serializable/";
                }
                else
                {
                    return pathPrefix + "types/";
                }
            }
            else if (aType->GetType() == RED4ext::ERTTIType::Enum)
            {
                return pathPrefix + "enum/";
            }
            else if (aType->GetType() == RED4ext::ERTTIType::BitField)
            {
                return pathPrefix + "bitfield/";
            }
        }

        return "Scripting/Natives/Generated/" + pathPrefix;
    };

    auto GetOverridePath = [&aIncludePath](const RED4ext::CBaseRTTIType* aType) -> std::string
    {
        std::string name = aType->GetName().ToString();
        std::string path = "Scripting/Natives/" + name + ".hpp";

        if (std::filesystem::exists(aIncludePath / path))
        {
            return path;
        }

        return {};
    };

    // Remove the prefix from the class
    auto SanitizeType = [GetPrefix](const RED4ext::CBaseRTTIType* aType) -> std::string {
        auto name = aType->GetName();
        std::string fullName = name.ToString();
        auto prefix = GetPrefix(fullName);
        return fullName.substr(prefix.size(), fullName.size() - prefix.size());
    };

    // Convert the prefixes into a namespace
    auto GetNamespace = [&prefixHierarchy](const std::string& aPrefix) -> std::string {
        std::string ns;
        if (!aPrefix.empty())
        {
            auto it = prefixHierarchy.find(aPrefix);
            if (it != prefixHierarchy.end())
            {
                ns = std::accumulate(it->second.begin(), it->second.end(), std::string(),
                                     [](const std::string& a, const std::string& b) -> std::string {
                                         return a + (a.length() > 0 ? "::" : "") + b;
                                     });
            }
        }

        return ns;
    };

    // Combine the namespace and sanitized name
    auto QualifiedType = [GetNamespace, GetPrefix](const RED4ext::CBaseRTTIType* aType) -> std::string
    {
        auto name = aType->GetName();

        std::string fullName = name.ToString();
        auto prefix = GetPrefix(fullName);
        auto stripped = fullName.substr(prefix.size(), fullName.size() - prefix.size());
        auto ns = GetNamespace(prefix);

        return ns.empty() ? stripped : ns + "::" + stripped;
    };

    auto IsHandleCompatible = [serializable](const RED4ext::CBaseRTTIType* aType) -> bool
    {
        if (aType->GetType() == RED4ext::ERTTIType::Class)
        {
            return reinterpret_cast<const CClass*>(aType)->IsA(serializable);
        }

        return false;
    };

    FixedTypeMapping fixedMapping = {{"ISerializable", "ISerializable"},
                                     {"IScriptable", "Scripting/IScriptable"},
                                     {"ScriptGameInstance", "Scripting/Natives/ScriptGameInstance"},
                                     {"gameItemID", "NativeTypes"},
                                     {"CBaseEngine", "GameEngine"},
                                     {"BaseGameEngine", "GameEngine"},
                                     {"CGameEngine", "GameEngine"},
                                     {"UpdateBucketEnum", "SystemUpdate"},
                                     {"worldGlobalNodeRef", "NativeTypes"}};

    std::regex invalidChars(INVALID_CHARACTERS);
    std::regex invalidKeywords(INVALID_KEYWORDS);
    NameSantizer nameSanitizer = [invalidChars, invalidKeywords](const std::string& input,
                                                                 bool& modify) -> std::string {
        modify = std::regex_search(input, invalidChars) || std::regex_search(input, invalidKeywords);
        std::string output = std::regex_replace(std::regex_replace(input, invalidChars, "_"), invalidKeywords, "$&_");
        if (!input.empty() && isdigit(input[0])) // Starting with a number is invalid, prefix it
        {
            output = "_" + output;
            modify = true;
        }
        return output;
    };

    // Third pass crawl dependencies, then build the File Descriptor to dump out
    std::set<std::string> includeCollector;

    for (auto& [classType, builder] : descriptorMap)
    {
        if (builder.pType->parent)
        {
            builder.Accumulate(builder.pType->parent);
        }

        for (auto& [hash, prop] : builder.mPropertyMap)
        {
            builder.Accumulate(prop->type);
        }

        if (aPropertyHolders)
        {
            for (auto& [hash, prop] : builder.mHolderPropertyMap)
            {
                builder.Accumulate(prop->type);
            }
        }

        // Don't emit files for the fixed mappings
        auto className = classType->GetName();

        auto it = fixedMapping.find(className);
        if (it != fixedMapping.end())
        {
            continue;
        }

        ClassFileDescriptor fileDescriptor;
        builder.ToFileDescriptor(fileDescriptor, SanitizeType, QualifiedType, GetGeneratedPath, GetOverridePath,
                                 IsHandleCompatible, fixedMapping, aVerbose);

        for (auto& dep : builder.mDirect)
        {
            // Don't emit files for the fixed mappings
            auto depName = dep->GetName();

            it = fixedMapping.find(depName);
            if (it != fixedMapping.end())
            {
                continue;
            }

            switch (dep->GetType())
            {
            case RED4ext::ERTTIType::Enum:
            {
                EnumFileDescriptor enumFd(static_cast<const RED4ext::CEnum*>(dep), SanitizeType, QualifiedType,
                                          GetGeneratedPath);
                enumFd.EmitFile(aOutPath, nameSanitizer);
                break;
            }
            case RED4ext::ERTTIType::BitField:
            {
                BitfieldFileDescriptor bfFd(static_cast<const RED4ext::CBitfield*>(dep), SanitizeType, QualifiedType,
                                            GetGeneratedPath);
                bfFd.EmitFile(aOutPath, nameSanitizer);
                break;
            }
            default:
                break;
            }
        }

        for (auto& inc : fileDescriptor.includes)
        {
            includeCollector.emplace(inc);
        }

        fileDescriptor.EmitFile(aOutPath, nameSanitizer);
    }

    EmitBulkGenerated(aOutPath, includeCollector);
}

RED4EXT_INLINE void ClassDependencyBuilder::Accumulate(const RED4ext::CBaseRTTIType* aType)
{
    switch (aType->GetType())
    {
    case RED4ext::ERTTIType::WeakHandle:
    {
        mIndirect.emplace(static_cast<const RED4ext::CRTTIWeakHandleType*>(aType)->GetInnerType());
        mDirect.emplace(aType);
        break;
    }
    case RED4ext::ERTTIType::Handle:
    {
        // Handles are indirect references to the object they carry
        mIndirect.emplace(static_cast<const RED4ext::CRTTIHandleType*>(aType)->GetInnerType());
        mDirect.emplace(aType);
        break;
    }
    case RED4ext::ERTTIType::ResourceAsyncReference:
    {
        mIndirect.emplace(static_cast<const RED4ext::CRTTIResourceAsyncReferenceType*>(aType)->innerType);
        mDirect.emplace(aType);
        break;
    }
    case RED4ext::ERTTIType::ResourceReference:
    {
        mIndirect.emplace(static_cast<const RED4ext::CRTTIResourceReferenceType*>(aType)->innerType);
        mDirect.emplace(aType);
        break;
    }
    case RED4ext::ERTTIType::LegacySingleChannelCurve:
    {
        // Curves usually contain primitives
        Accumulate(static_cast<const RED4ext::CRTTILegacySingleChannelCurveType*>(aType)->curveType);
        mDirect.emplace(aType);
        break;
    }
    case RED4ext::ERTTIType::Array:
    case RED4ext::ERTTIType::NativeArray:
    case RED4ext::ERTTIType::StaticArray:
    {
        // Arrays may contain Handles or Direct refs to other types
        Accumulate(static_cast<const RED4ext::CRTTIBaseArrayType*>(aType)->GetInnerType());
        mDirect.emplace(aType);
        break;
    }
    default:
    {
        // Promote from indirect to direct dependency
        auto it = mIndirect.find(aType);
        if (it != mIndirect.end())
        {
            mIndirect.erase(it);
        }
        mDirect.emplace(aType);
        break;
    }
    }
}

RED4EXT_INLINE EnumFileDescriptor::EnumFileDescriptor(const RED4ext::CEnum* pEnum, NameTransformer aNameTransformer,
                                                      NameTransformer aQualifiedTransformer, DescriptorPath aTypeToPath)
{
    auto enumName = pEnum->GetName();

    name = aNameTransformer(pEnum);
    nameQualified = aQualifiedTransformer(pEnum);
    trueName = enumName.ToString();
    directory = aTypeToPath(pEnum);

    auto rtti = RED4ext::CRTTISystem::Get();
    auto aliasName = rtti->nativeToScript.Get(enumName);
    if (aliasName && !rtti->types.Get(*aliasName))
    {
        alias = aliasName->ToString();
    }

    size = pEnum->GetSize();

    for (uint32_t i = 0; i < pEnum->hashList.size; ++i)
    {
        enumMap[pEnum->valueList[i]] = pEnum->hashList[i].ToString();
    }

    for (uint32_t i = 0; i < pEnum->aliasList.size; ++i)
    {
        enumAlias[pEnum->aliasList[i].ToString()] = pEnum->aliasValueList[i];
    }
}

RED4EXT_INLINE void EnumFileDescriptor::EmitFile(std::filesystem::path aOutPath, NameSantizer aSanitizer)
{
    aOutPath /= directory;
    std::filesystem::create_directories(aOutPath);
    aOutPath /= name + ".hpp";

    std::ofstream o(aOutPath);

    o << "#pragma once" << std::endl << std::endl;
    o << "// clang-format off" << std::endl << std::endl;

    o << "// This file is generated from the Game's Reflection data" << std::endl << std::endl;

    o << "#include <cstdint>" << std::endl;

    o << "namespace RED4ext" << std::endl;
    o << "{" << std::endl;

    auto nsIndex = nameQualified.find_last_of("::");
    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);
        o << "namespace " << ns << " { " << std::endl;
        o << "enum class " << name;
    }
    else
    {
        o << "enum class " << nameQualified;
    }

    o << " : ";

    switch (size)
    {
    case sizeof(uint8_t):
    {
        o << "uint8_t";
        break;
    }
    case sizeof(uint16_t):
    {
        o << "uint16_t";
        break;
    }
    case sizeof(uint32_t):
    {
        o << "uint32_t";
        break;
    }
    case sizeof(uint64_t):
    {
        o << "uint64_t";
        break;
    }
    default:
        break;
    }

    o << std::endl;
    o << "{" << std::endl;

    for (auto ev : enumMap)
    {
        bool isModified = false;
        o << "    " << aSanitizer(ev.second, isModified) << " = ";

        switch (size)
        {
        case sizeof(uint8_t):
        {
            o << static_cast<uint32_t>(static_cast<uint8_t>(ev.first));
            break;
        }
        case sizeof(uint16_t):
        {
            o << static_cast<uint16_t>(ev.first);
            break;
        }
        case sizeof(uint32_t):
        {
            o << static_cast<uint32_t>(ev.first);
            break;
        }
        case sizeof(uint64_t):
        {
            o << static_cast<uint64_t>(ev.first);
            break;
        }
        default:
            break;
        }

        o << ",";

        if (isModified)
        {
            o << " // " << ev.second;
        }

        o << std::endl;
    }

    for (auto eAlias : enumAlias)
    {
        bool isLeftModified = false;
        bool isRightModified = false;
        o << "    " << aSanitizer(eAlias.first, isLeftModified) << " = "
          << aSanitizer(enumMap[eAlias.second], isRightModified);
        o << ",";

        if (isLeftModified || isRightModified)
        {
            o << " // " << eAlias.first << " = " << enumMap[eAlias.second];
        }

        o << std::endl;
    }

    o << "};" << std::endl;

    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);
        o << "} // namespace " << ns << std::endl;
    }

    if (name != trueName)
    {
        o << "using " << trueName << " = " << nameQualified << ";" << std::endl;
    }

    if (!alias.empty() && alias != trueName)
    {
        o << "using " << alias << " = " << nameQualified << ";" << std::endl;
    }

    o << "} // namespace RED4ext" << std::endl << std::endl;
    o << "// clang-format on" << std::endl;
}

RED4EXT_INLINE BitfieldFileDescriptor::BitfieldFileDescriptor(const RED4ext::CBitfield* pBitfield,
                                                              NameTransformer aNameTransformer,
                                                              NameTransformer aQualifiedTransformer,
                                                              DescriptorPath aTypeToPath)
{
    auto bitfieldName = pBitfield->GetName();

    name = aNameTransformer(pBitfield);
    nameQualified = aQualifiedTransformer(pBitfield);
    trueName = bitfieldName.ToString();
    directory = aTypeToPath(pBitfield);

    auto rtti = RED4ext::CRTTISystem::Get();
    auto aliasName = rtti->nativeToScript.Get(bitfieldName);
    if (aliasName && !rtti->types.Get(*aliasName))
    {
        alias = aliasName->ToString();
    }

    size = pBitfield->GetSize();

    uint64_t validBits = pBitfield->validBits;
    uint32_t i = 0;
    while (i < size * 8)
    {
        if (validBits & 1)
        {
            bitNames.push_back(pBitfield->bitNames[i].ToString());
        }
        else
        {
            bitNames.push_back("b" + std::to_string(i));
        }

        validBits >>= 1;
        ++i;
    }
}

RED4EXT_INLINE void BitfieldFileDescriptor::EmitFile(std::filesystem::path aOutPath, NameSantizer aSanitizer)
{
    aOutPath /= directory;
    std::filesystem::create_directories(aOutPath);
    aOutPath /= name + ".hpp";

    std::ofstream o(aOutPath);

    o << "#pragma once" << std::endl << std::endl;
    o << "// clang-format off" << std::endl << std::endl;

    o << "// This file is generated from the Game's Reflection data" << std::endl << std::endl;

    o << "#include <cstdint>" << std::endl;

    o << "namespace RED4ext" << std::endl;
    o << "{" << std::endl;

    auto nsIndex = nameQualified.find_last_of("::");
    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);
        o << "namespace " << ns << " { " << std::endl;
        o << "struct " << name;
    }
    else
    {
        o << "struct " << nameQualified;
    }

    o << std::endl;
    o << "{" << std::endl;

    uint32_t i = 0;
    for (auto& bitName : bitNames)
    {
        o << "    ";
        switch (size)
        {
        case sizeof(uint8_t):
        {
            o << "uint8_t";
            break;
        }
        case sizeof(uint16_t):
        {
            o << "uint16_t";
            break;
        }
        case sizeof(uint32_t):
        {
            o << "uint32_t";
            break;
        }
        case sizeof(uint64_t):
        {
            o << "uint64_t";
            break;
        }
        default:
            break;
        }

        bool isModified = false;
        o << " " << aSanitizer(bitName, isModified) << " : 1; // " << i++;
        if (isModified)
        {
            o << " " << bitName;
        }
        o << std::endl;
    }

    o << "};" << std::endl;

    o << "RED4EXT_ASSERT_SIZE(" << name << ", 0x" << std::hex << std::uppercase << size << ");" << std::endl;

    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);
        o << "} // namespace " << ns << std::endl;
    }

    if (!alias.empty() && alias != trueName)
    {
        o << "using " << alias << " = " << nameQualified << ";" << std::endl;
    }

    o << "} // namespace RED4ext" << std::endl << std::endl;
    o << "// clang-format on" << std::endl;
}

RED4EXT_INLINE void ClassDependencyBuilder::ToFileDescriptor(ClassFileDescriptor& aFd, NameTransformer aNameTransformer,
                                                             NameTransformer aQualifiedTransformer,
                                                             DescriptorPath aTypeToPath, DescriptorPath aTypeToOverride,
                                                             TypeChecker aHandleCompatChecker,
                                                             const FixedTypeMapping& aFixedMapping, bool aVerbose)
{
    auto name = pType->GetName();

    aFd.name = aNameTransformer(pType);
    aFd.nameQualified = aQualifiedTransformer(pType);

    auto rtti = RED4ext::CRTTISystem::Get();
    auto aliasName = rtti->nativeToScript.Get(name);
    if (aliasName && !rtti->types.Get(*aliasName))
    {
        aFd.alias = aliasName->ToString();
    }

    aFd.trueName = name.ToString();
    aFd.size = pType->GetSize();
    aFd.alignment = pType->GetAlignment();
    aFd.directory = aTypeToPath(pType);
    aFd.override = aTypeToOverride(pType);
    aFd.usedAsHandle = aHandleCompatChecker(pType);

    if (pType->parent)
    {
        aFd.parent = aNameTransformer(pType->parent);
        aFd.parentQualified = aQualifiedTransformer(pType->parent);
        aFd.parentSize = pType->parent->GetSize();
    }

    for (auto& dir : mDirect)
    {
        switch (dir->GetType())
        {
        case RED4ext::ERTTIType::Enum:
        case RED4ext::ERTTIType::BitField:
        case RED4ext::ERTTIType::Class:
        {
            auto includeName = dir->GetName();

            // Redirect dependencies if they are part of the fixed mappings
            auto it = aFixedMapping.find(includeName);
            if (it != aFixedMapping.end())
            {
                aFd.includes.emplace(it->second);
            }
            else
            {
                aFd.includes.emplace(aTypeToPath(dir) + aNameTransformer(dir));
            }
            break;
        }
        case RED4ext::ERTTIType::Array:
        {
            aFd.includes.emplace("DynArray");
            break;
        }
        case RED4ext::ERTTIType::Name:
        {
            aFd.includes.emplace("CName");
            break;
        }
        case RED4ext::ERTTIType::WeakHandle:
        case RED4ext::ERTTIType::Handle:
        {
            aFd.includes.emplace("Handle");
            break;
        }
        case RED4ext::ERTTIType::StaticArray:
        case RED4ext::ERTTIType::NativeArray:
        case RED4ext::ERTTIType::ResourceReference:
        case RED4ext::ERTTIType::ResourceAsyncReference:
        case RED4ext::ERTTIType::LegacySingleChannelCurve:
        case RED4ext::ERTTIType::Simple:
        {
            aFd.includes.emplace("NativeTypes");
            break;
        }
        default:
            break;
        }
    }

    for (auto& ind : mIndirect)
    {
        aFd.fwdDeclarations.emplace(aQualifiedTransformer(ind));
    }

    std::array<std::tuple<std::map<uint64_t, const RED4ext::CProperty*>*,
                          std::vector<ClassFileDescriptor::PropertyDescriptor>*>,
               2>
        props = {make_tuple(&mPropertyMap, &aFd.properties), make_tuple(&mHolderPropertyMap, &aFd.holderProperties)};

    for (auto propList : props)
    {
        for (auto& prop : *std::get<0>(propList))
        {
            std::string propTypeName = TypeToString(prop.second->type, aNameTransformer, aVerbose);
            std::string propTypeNameQualified = TypeToString(prop.second->type, aQualifiedTransformer, aVerbose);
            std::string propName = prop.second->name.ToString();

            std::get<1>(propList)->push_back({propTypeName, propTypeNameQualified, propName, prop.first,
                                              prop.second->type->GetSize(), prop.second->type->GetAlignment()});
        }
    }
}

RED4EXT_INLINE std::string TypeToString(const RED4ext::CBaseRTTIType* aType, NameTransformer aNameTransformer,
                                        bool aVerbose)
{
    // Handle some simple type conversions and fundamentals
    static std::unordered_map<std::string, std::string> s_typeMap = {
        {"Int8", "int8_t"},   {"Int16", "int16_t"},   {"Int32", "int32_t"},   {"Int64", "int64_t"},
        {"Uint8", "uint8_t"}, {"Uint16", "uint16_t"}, {"Uint32", "uint32_t"}, {"Uint64", "uint64_t"},
        {"Float", "float"},   {"Bool", "bool"},       {"String", "CString"},  {"gameItemID", "ItemID"},
        {"Double", "double"}};

    std::string typeName;

    switch (aType->GetType())
    {
    case RED4ext::ERTTIType::WeakHandle:
    {
        typeName = "WeakHandle<" +
                   TypeToString(static_cast<const RED4ext::CRTTIWeakHandleType*>(aType)->GetInnerType(),
                                aNameTransformer, aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::Handle:
    {
        typeName = "Handle<" +
                   TypeToString(static_cast<const RED4ext::CRTTIHandleType*>(aType)->GetInnerType(), aNameTransformer,
                                aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::Array:
    {
        typeName = "DynArray<" +
                   TypeToString(static_cast<const RED4ext::CRTTIArrayType*>(aType)->GetInnerType(), aNameTransformer,
                                aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::ResourceAsyncReference:
    {
        typeName = "RaRef<" +
                   TypeToString(static_cast<const RED4ext::CRTTIResourceAsyncReferenceType*>(aType)->innerType,
                                aNameTransformer, aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::ResourceReference:
    {
        typeName = "Ref<" +
                   TypeToString(static_cast<const RED4ext::CRTTIResourceReferenceType*>(aType)->innerType,
                                aNameTransformer, aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::LegacySingleChannelCurve:
    {
        typeName = "CurveData<" +
                   TypeToString(static_cast<const RED4ext::CRTTILegacySingleChannelCurveType*>(aType)->curveType,
                                aNameTransformer, aVerbose) +
                   ">";
        break;
    }
    case RED4ext::ERTTIType::StaticArray:
    {
        if (aType->GetAlignment() >= sizeof(void*))
        {
            typeName = "alignas(" + std::to_string(aType->GetAlignment()) + ") ";
        }

        auto staticArray = static_cast<const RED4ext::CRTTIStaticArrayType*>(aType);

        typeName += "StaticArray<" + TypeToString(staticArray->GetInnerType(), aNameTransformer, aVerbose) + ", " +
                    std::to_string(staticArray->GetMaxLength()) + ">";
        break;
    }
    case RED4ext::ERTTIType::NativeArray:
    {
        auto nativeArray = static_cast<const RED4ext::CRTTINativeArrayType*>(aType);
        typeName = "NativeArray<" + TypeToString(nativeArray->GetInnerType(), aNameTransformer, aVerbose) + ", " +
                   std::to_string(nativeArray->GetMaxLength()) + ">";
        break;
    }

    case RED4ext::ERTTIType::Name:
    case RED4ext::ERTTIType::Fundamental:
    case RED4ext::ERTTIType::Class:
    case RED4ext::ERTTIType::Simple:
    case RED4ext::ERTTIType::Enum:
    case RED4ext::ERTTIType::BitField:
    {
        auto trueName = aType->GetName();

        auto it = s_typeMap.find(trueName.ToString());
        if (it != s_typeMap.end())
        {
            typeName = it->second;
        }
        else
        {
            typeName = aNameTransformer(aType);
        }
    }
        // case RED4ext::ERTTIType::Pointer:
        // case RED4ext::ERTTIType::ScriptReference:
    default:
    {
        break;
    }
    }

    if (typeName.empty())
    {
        auto trueName = aType->GetName();

        uint32_t size = aType->GetSize();
        auto tName = aType->GetTypeName();

        // We don't have this type supported yet but we can put some bytes in as placeholder as we know its size
        typeName = "std::array<uint8_t, " + std::to_string(size) + ">/* UNHANDLED: " + trueName.ToString() + " (" +
                   tName.c_str() + ") */";
    }

    if (aVerbose)
    {
        typeName += " /* align(" + std::to_string(aType->GetAlignment()) + ") size(" +
                    std::to_string(aType->GetSize()) + ") */";
    }

    return typeName;
}

RED4EXT_INLINE void ClassFileDescriptor::EmitFile(std::filesystem::path aOutPath, NameSantizer aSanitizer)
{
    aOutPath /= directory;
    std::filesystem::create_directories(aOutPath);
    aOutPath /= name + ".hpp";

    std::ofstream o(aOutPath);

    o << "#pragma once" << std::endl << std::endl;
    o << "// clang-format off" << std::endl << std::endl;

    o << "// This file is generated from the Game's Reflection data" << std::endl << std::endl;

    if (!override.empty())
    {
        o << "#include <RED4ext/" << override << ">" << std::endl << std::endl;

        o << "namespace RED4ext" << std::endl;
        o << "{" << std::endl;
        o << "RED4EXT_ASSERT_SIZE(" << nameQualified << ", 0x" << std::hex << std::uppercase << size << ");"
          << std::endl;

        if (name != trueName)
        {
            o << "using " << trueName << " = " << nameQualified << ";" << std::endl;
        }

        if (!alias.empty() && alias != trueName)
        {
            o << "using " << alias << " = " << nameQualified << ";" << std::endl;
        }

        o << "} // namespace RED4ext" << std::endl << std::endl;
        o << "/*" << std::endl;
    }

    o << "#include <cstdint>" << std::endl;
    o << "#include <RED4ext/Common.hpp>" << std::endl;
    // o << "#include <RED4ext/Hashing/FNV1a.hpp>" << std::endl;

    for (auto inc : includes)
    {
        o << "#include <RED4ext/" << inc << ".hpp>" << std::endl;
    }
    o << std::endl;

    o << "namespace RED4ext" << std::endl;
    o << "{" << std::endl;

    if (fwdDeclarations.size())
    {
        for (auto fwd : fwdDeclarations)
        {
            if (fwd != nameQualified)
            {
                auto index = fwd.find_last_of("::");
                if (index != std::string::npos)
                {
                    auto ns = fwd.substr(0, index - 1);
                    auto fwdDecl = fwd.substr(index + 1, fwd.size() - index);

                    o << "namespace " << ns << " { "
                      << "struct " << fwdDecl << "; }" << std::endl;
                }
                else
                {
                    o << "struct " << fwd << ";" << std::endl;
                }
            }
        }
        o << std::endl;
    }

    auto nsIndex = nameQualified.find_last_of("::");
    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);

        o << "namespace " << ns << std::endl;
        o << "{" << std::endl;
    }

    o << "struct ";

    if (alignment == 0x10)
    {
        o << "__declspec(align(0x" << std::hex << std::uppercase << alignment << ")) ";
    }

    if (nsIndex != std::string::npos)
    {
        o << name;
    }
    else
    {
        o << nameQualified;
    }

    if (!parent.empty())
    {
        o << " : " << parentQualified;
    }
    o << std::endl;
    o << "{" << std::endl;

    o << "    static constexpr const char* NAME = \"" << trueName << "\";" << std::endl;
    if (!alias.empty())
    {
        o << "    static constexpr const char* ALIAS = \"" << alias << "\";" << std::endl;
    }
    else
    {
        o << "    static constexpr const char* ALIAS = NAME;" << std::endl;
    }
    o << std::endl;

    if (holderProperties.size())
    {
        o << "    struct PropertyHolder" << std::endl;
        o << "    {" << std::endl;

        size_t lastOffset = 0;
        size_t lastSize = 0;

        for (auto prop : holderProperties)
        {
            // Fix gap between two properties
            int64_t byteGap = static_cast<int64_t>(prop.offset) - static_cast<int64_t>(lastOffset + lastSize);
            if (byteGap > 0)
            {
                size_t gapStart = lastOffset + lastSize;
                size_t gapEnd = prop.offset;

                o << "        uint8_t unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase
                  << gapStart << "[0x" << gapEnd << " - 0x" << gapStart << "]; // " << gapStart << std::endl;

                o.unsetf(std::ios::hex | std::ios::uppercase);
            }

            // Ugly check, but I don't care for now.
            if (prop.alignment >= sizeof(void*) && prop.type.starts_with("alignas"))
            {
                o << "#pragma warning(suppress : 4324)" << std::endl;
            }

            o << "        " << prop.typeQualified << " ";

            bool isSanitized = false;
            std::string propName = aSanitizer(prop.name, isSanitized);

            bool isValid = !prop.name.empty();

            // Some properties have C++ invalid names for unknown reasons, we will convert to something valid
            // and then print the original name in the comment
            if (isValid)
            {
                o << propName;
            }
            else
            {
                o << "unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << prop.offset;
            }

            o << "; // " << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << prop.offset;
            if (!isValid || isSanitized)
            {
                o << " -- " << prop.name;
            }
            o << std::endl;

            o.unsetf(std::ios::hex | std::ios::uppercase);

            lastOffset = prop.offset;
            lastSize = prop.size;
        }

        o << "    };" << std::endl << std::endl;
    }

    if (properties.size())
    {
        size_t lastOffset = parentSize;
        size_t lastSize = 0;

        for (auto prop : properties)
        {
            // Fix gap between two properties
            int64_t byteGap = static_cast<int64_t>(prop.offset) - static_cast<int64_t>(lastOffset + lastSize);
            if (byteGap > 0)
            {
                size_t gapStart = lastOffset + lastSize;
                size_t gapEnd = prop.offset;

                o << "    uint8_t unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << gapStart
                  << "[0x" << gapEnd << " - 0x" << gapStart << "]; // " << gapStart << std::endl;

                o.unsetf(std::ios::hex | std::ios::uppercase);
            }

            // Ugly check, but I don't care for now.
            if (prop.alignment >= sizeof(void*) && prop.type.starts_with("alignas"))
            {
                o << "#pragma warning(suppress : 4324)" << std::endl;
            }

            o << "    " << prop.typeQualified << " ";

            bool isSanitized = false;
            std::string propName = aSanitizer(prop.name, isSanitized);

            bool isValid = !prop.name.empty();

            // Some properties have C++ invalid names for unknown reasons, we will convert to something valid
            // and then print the original name in the comment
            if (isValid)
            {
                o << propName;
            }
            else
            {
                o << "unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << prop.offset;
            }

            o << "; // " << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << prop.offset;
            if (!isValid || isSanitized)
            {
                o << " -- " << prop.name;
            }
            o << std::endl;

            o.unsetf(std::ios::hex | std::ios::uppercase);

            lastOffset = prop.offset;
            lastSize = prop.size;
        }

        auto& back = properties.back();
        if (back.offset + back.size < size) // Pad remainder of class based on the last property
        {
            size_t gapStart = back.offset + back.size;
            size_t gapEnd = size;

            o << "    uint8_t unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << gapStart
              << "[0x" << gapEnd << " - 0x" << gapStart << "]; // " << gapStart << std::endl;

            o.unsetf(std::ios::hex | std::ios::uppercase);
        }
    }
    else if (size - parentSize > 0) // Pad the whole class
    {
        size_t gapStart = parentSize;
        o << "    uint8_t unk" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << gapStart << "[0x"
          << size << " - 0x" << gapStart << "]; // " << gapStart << std::endl;

        o.unsetf(std::ios::hex | std::ios::uppercase);
    }

    o << "};" << std::endl;
    o << "RED4EXT_ASSERT_SIZE(" << name << ", 0x" << std::hex << std::uppercase << size << ");" << std::endl;

    if (nsIndex != std::string::npos)
    {
        auto ns = nameQualified.substr(0, nsIndex - 1);

        o << "} // namespace " << ns << std::endl;
    }

    if (name != trueName)
    {
        o << "using " << trueName << " = " << nameQualified << ";" << std::endl;
    }

    if (!alias.empty() && alias != trueName)
    {
        o << "using " << alias << " = " << nameQualified << ";" << std::endl;
    }

    o << "} // namespace RED4ext" << std::endl;

    if (!override.empty())
    {
        o << "*/" << std::endl;
    }

    o << std::endl;
    o << "// clang-format on" << std::endl;
}

void EmitBulkGenerated(std::filesystem::path aOutPath, const std::set<std::string>& aIncludes)
{
    std::filesystem::create_directories(aOutPath);
    aOutPath = aOutPath / "Scripting" / "Natives.cpp";

    std::ofstream o(aOutPath);

    o << "#ifndef RED4EXT_STATIC_LIB" << std::endl;
    o << "#error Please define 'RED4EXT_STATIC_LIB' to compile this file." << std::endl;
    o << "#endif" << std::endl;
    o << std::endl;

    for (auto& inc : aIncludes)
    {
        o << "#include <RED4ext/" << inc << ".hpp>" << std::endl;
    }
}

} // namespace RED4ext::GameReflection
