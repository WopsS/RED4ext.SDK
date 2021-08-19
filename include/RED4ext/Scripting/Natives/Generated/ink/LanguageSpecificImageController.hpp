#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/CName.hpp>
#include <RED4ext/DynArray.hpp>
#include <RED4ext/NativeTypes.hpp>
#include <RED4ext/Scripting/Natives/Generated/ink/LanguageId.hpp>
#include <RED4ext/Scripting/Natives/Generated/ink/WidgetLogicController.hpp>

namespace RED4ext
{
namespace ink { struct TextureAtlas; }

namespace ink { 
struct LanguageSpecificImageController : ink::WidgetLogicController
{
    static constexpr const char* NAME = "inkLanguageSpecificImageController";
    static constexpr const char* ALIAS = NAME;

    RaRef<ink::TextureAtlas> textureAtlasForLanguage; // 68
    CName partNameForLanguage; // 70
    DynArray<ink::LanguageId> languages; // 78
    RaRef<ink::TextureAtlas> fallbackTextureAtlas; // 88
    CName fallbackPartName; // 90
};
RED4EXT_ASSERT_SIZE(LanguageSpecificImageController, 0x98);
} // namespace ink
} // namespace RED4ext
