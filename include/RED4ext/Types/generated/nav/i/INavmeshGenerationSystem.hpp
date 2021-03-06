#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/game/IGameSystem.hpp>

namespace RED4ext
{
namespace nav::i { 
struct INavmeshGenerationSystem : game::IGameSystem
{
    static constexpr const char* NAME = "naviINavmeshGenerationSystem";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(INavmeshGenerationSystem, 0x48);
} // namespace nav::i
} // namespace RED4ext
