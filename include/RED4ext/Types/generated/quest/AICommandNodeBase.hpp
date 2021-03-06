#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/quest/SignalStoppingNodeDefinition.hpp>

namespace RED4ext
{
namespace quest { 
struct AICommandNodeBase : quest::SignalStoppingNodeDefinition
{
    static constexpr const char* NAME = "questAICommandNodeBase";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(AICommandNodeBase, 0x48);
} // namespace quest
} // namespace RED4ext
