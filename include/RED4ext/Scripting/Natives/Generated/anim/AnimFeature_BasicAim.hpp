#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/AnimFeature.hpp>

namespace RED4ext
{
namespace anim { 
struct AnimFeature_BasicAim : anim::AnimFeature
{
    static constexpr const char* NAME = "animAnimFeature_BasicAim";
    static constexpr const char* ALIAS = NAME;

    int32_t aimState; // 40
    int32_t zoomState; // 44
};
RED4EXT_ASSERT_SIZE(AnimFeature_BasicAim, 0x48);
} // namespace anim
} // namespace RED4ext
