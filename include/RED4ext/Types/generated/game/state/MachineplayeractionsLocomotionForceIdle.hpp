#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/game/state/MachineplayeractionsLocomotionBase.hpp>

namespace RED4ext
{
namespace game::state { 
struct MachineplayeractionsLocomotionForceIdle : game::state::MachineplayeractionsLocomotionBase
{
    static constexpr const char* NAME = "gamestateMachineplayeractionsLocomotionForceIdle";
    static constexpr const char* ALIAS = NAME;

    uint8_t unk110[0x120 - 0x110]; // 110
};
RED4EXT_ASSERT_SIZE(MachineplayeractionsLocomotionForceIdle, 0x120);
} // namespace game::state
} // namespace RED4ext
