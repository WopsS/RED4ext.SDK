#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/PlayerScriptableSystemRequest.hpp>

namespace RED4ext
{
namespace game { 
struct PlayerDetachRequest : game::PlayerScriptableSystemRequest
{
    static constexpr const char* NAME = "gamePlayerDetachRequest";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(PlayerDetachRequest, 0x58);
} // namespace game
} // namespace RED4ext
