#pragma once

// clang-format off

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/AI/Command.hpp>

namespace RED4ext
{
namespace AI
{
struct RunAwayFromPlayerCommand : AI::Command
{
    static constexpr const char* NAME = "AIRunAwayFromPlayerCommand";
    static constexpr const char* ALIAS = NAME;

};
RED4EXT_ASSERT_SIZE(RunAwayFromPlayerCommand, 0x60);
} // namespace AI
using AIRunAwayFromPlayerCommand = AI::RunAwayFromPlayerCommand;
} // namespace RED4ext

// clang-format on
