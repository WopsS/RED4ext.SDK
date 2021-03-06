#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/quest/PhaseInstance.hpp>

namespace RED4ext
{
namespace quest { 
struct RootInstance : quest::PhaseInstance
{
    static constexpr const char* NAME = "questRootInstance";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(RootInstance, 0x110);
} // namespace quest
} // namespace RED4ext
