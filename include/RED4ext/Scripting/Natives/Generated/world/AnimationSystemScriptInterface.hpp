#pragma once

// clang-format off

// This file is generated from the Game's Reflection data

#include <RED4ext/Scripting/Natives/worldAnimationSystemScriptInterface.hpp>

namespace RED4ext
{
RED4EXT_ASSERT_SIZE(world::AnimationSystemScriptInterface, 0x48);
using worldAnimationSystemScriptInterface = world::AnimationSystemScriptInterface;
using AnimationSystem = world::AnimationSystemScriptInterface;
} // namespace RED4ext

/*
#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>

namespace RED4ext
{
namespace world
{
struct AnimationSystemScriptInterface : IScriptable
{
    static constexpr const char* NAME = "worldAnimationSystemScriptInterface";
    static constexpr const char* ALIAS = "AnimationSystem";

    uint8_t unk40[0x48 - 0x40]; // 40
};
RED4EXT_ASSERT_SIZE(AnimationSystemScriptInterface, 0x48);
} // namespace world
using worldAnimationSystemScriptInterface = world::AnimationSystemScriptInterface;
using AnimationSystem = world::AnimationSystemScriptInterface;
} // namespace RED4ext
*/

// clang-format on
