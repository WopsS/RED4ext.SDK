#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/game/IGameSystem.hpp>

namespace RED4ext
{
struct FunctionalTestsIGameSystem : game::IGameSystem
{
    static constexpr const char* NAME = "FunctionalTestsIGameSystem";
    static constexpr const char* ALIAS = "FunctionalTestsGameSystemInterface";

};
RED4EXT_ASSERT_SIZE(FunctionalTestsIGameSystem, 0x48);
using FunctionalTestsGameSystemInterface = FunctionalTestsIGameSystem;
} // namespace RED4ext
