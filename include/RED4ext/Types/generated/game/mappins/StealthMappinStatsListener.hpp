#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/game/IStatsListener.hpp>

namespace RED4ext
{
namespace game::mappins { 
struct StealthMappinStatsListener : game::IStatsListener
{
    static constexpr const char* NAME = "gamemappinsStealthMappinStatsListener";
    static constexpr const char* ALIAS = NAME;

    uint8_t unk40[0x50 - 0x40]; // 40
};
RED4EXT_ASSERT_SIZE(StealthMappinStatsListener, 0x50);
} // namespace game::mappins
} // namespace RED4ext