#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/game/data/MappinVariant.hpp>
#include <RED4ext/Types/generated/game/mappins/IPointOfInterestVariant.hpp>

namespace RED4ext
{
namespace game::mappins { 
struct CommonVariant : game::mappins::IPointOfInterestVariant
{
    static constexpr const char* NAME = "gamemappinsCommonVariant";
    static constexpr const char* ALIAS = NAME;

    game::data::MappinVariant variant; // 38
    uint8_t unk3C[0x40 - 0x3C]; // 3C
};
RED4EXT_ASSERT_SIZE(CommonVariant, 0x40);
} // namespace game::mappins
} // namespace RED4ext
