#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/TweakDBRecord.hpp>

namespace RED4ext
{
namespace game::data { 
struct Affiliation_Record : game::data::TweakDBRecord
{
    static constexpr const char* NAME = "gamedataAffiliation_Record";
    static constexpr const char* ALIAS = NAME;

    uint8_t unk48[0x60 - 0x48]; // 48
};
RED4EXT_ASSERT_SIZE(Affiliation_Record, 0x60);
} // namespace game::data
} // namespace RED4ext
