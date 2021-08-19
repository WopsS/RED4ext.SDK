#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/ScannableData_Record.hpp>

namespace RED4ext
{
namespace game::data::device_scanning_data_ { 
struct Record : game::data::ScannableData_Record
{
    static constexpr const char* NAME = "gamedatadevice_scanning_data_Record";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(Record, 0x58);
} // namespace game::data::device_scanning_data_
} // namespace RED4ext
