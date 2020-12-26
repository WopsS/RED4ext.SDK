#pragma once

#include <cstdint>

namespace RED4ext
{
struct CName
{
    CName(uint64_t aHash);
    CName(const char* aName);

    const char* operator()();

    uint64_t hash;
};
} // namespace RED4ext

#ifdef RED4EXT_HEADER_ONLY
#include <RED4ext/CName-inl.hpp>
#endif
