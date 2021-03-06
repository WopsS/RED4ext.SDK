#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>

namespace RED4ext
{
namespace game { 
struct GrenadeThrowQuery
{
    static constexpr const char* NAME = "gameGrenadeThrowQuery";
    static constexpr const char* ALIAS = NAME;

    uint8_t unk00[0x1C0 - 0x0]; // 0
};
RED4EXT_ASSERT_SIZE(GrenadeThrowQuery, 0x1C0);
} // namespace game
} // namespace RED4ext
