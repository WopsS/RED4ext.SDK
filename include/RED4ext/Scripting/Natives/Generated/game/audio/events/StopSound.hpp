#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/CName.hpp>
#include <RED4ext/Scripting/Natives/Generated/red/Event.hpp>

namespace RED4ext
{
namespace game::audio::events { 
struct StopSound : red::Event
{
    static constexpr const char* NAME = "gameaudioeventsStopSound";
    static constexpr const char* ALIAS = NAME;

    CName soundName; // 40
};
RED4EXT_ASSERT_SIZE(StopSound, 0x48);
} // namespace game::audio::events
} // namespace RED4ext
