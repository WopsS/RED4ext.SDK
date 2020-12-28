#pragma once

#include <cstdint>
#include <RED4ext/Common.hpp>

namespace RED4ext
{
template<typename T>
struct DynArray
{
    T operator[](uint32_t aIndex)
    {
        return entries[aIndex];
    }

    T* entries;
    uint32_t capacity;
    uint32_t size;
};

RED4EXT_ASSERT_SIZE(DynArray<void*>, 0x10);
RED4EXT_ASSERT_OFFSET(DynArray<void*>, capacity, 0x8);
RED4EXT_ASSERT_OFFSET(DynArray<void*>, size, 0xC);
} // namespace RED4ext
