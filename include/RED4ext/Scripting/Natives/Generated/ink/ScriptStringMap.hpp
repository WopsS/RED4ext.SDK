#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>

namespace RED4ext
{
namespace ink { 
struct ScriptStringMap : IScriptable
{
    static constexpr const char* NAME = "inkScriptStringMap";
    static constexpr const char* ALIAS = NAME;

    uint8_t unk40[0x70 - 0x40]; // 40
};
RED4EXT_ASSERT_SIZE(ScriptStringMap, 0x70);
} // namespace ink
} // namespace RED4ext