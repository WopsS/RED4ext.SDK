#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/Color.hpp>
#include <RED4ext/Types/generated/IEvaluatorColor.hpp>

namespace RED4ext
{
struct CEvaluatorColorStartEnd : IEvaluatorColor
{
    static constexpr const char* NAME = "CEvaluatorColorStartEnd";
    static constexpr const char* ALIAS = NAME;

    Color start; // 30
    Color end; // 34
};
RED4EXT_ASSERT_SIZE(CEvaluatorColorStartEnd, 0x38);
} // namespace RED4ext
