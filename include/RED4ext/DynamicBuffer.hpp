#pragma once

#include <cstdint>

namespace RED4ext
{
template<typename T>
struct DynamicBuffer
{
    T* ptr;
    uint32_t capacity;
    uint32_t size;
};
} // namespace RED4ext
