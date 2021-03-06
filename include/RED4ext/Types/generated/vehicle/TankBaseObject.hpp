#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/vehicle/BaseObject.hpp>

namespace RED4ext
{
namespace vehicle { 
struct TankBaseObject : vehicle::BaseObject
{
    static constexpr const char* NAME = "vehicleTankBaseObject";
    static constexpr const char* ALIAS = "TankObject";

    uint8_t unk8F0[0x920 - 0x8F0]; // 8F0
};
RED4EXT_ASSERT_SIZE(TankBaseObject, 0x920);
} // namespace vehicle
using TankObject = vehicle::TankBaseObject;
} // namespace RED4ext
