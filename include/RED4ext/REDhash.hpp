#pragma once

#include <cstdint>

namespace RED4ext
{
constexpr uint64_t FNV1a(const char* aText)
{
    constexpr uint64_t basis = 0xCBF29CE484222325;
    constexpr uint64_t prime = 0x100000001b3;

    uint64_t hash = basis;
    while (aText && *aText)
    {
        hash ^= *aText;
        hash *= prime;

        aText++;
    }

    return hash;
}
} // namespace RED4ext
