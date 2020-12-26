#pragma once

#ifdef RED4EXT_STATIC_LIB
#include <RED4ext/CName.hpp>
#endif

#include <RED4ext/Addresses.hpp>
#include <RED4ext/REDfunc.hpp>
#include <RED4ext/REDhash.hpp>

RED4ext::CName::CName(uint64_t aHash)
    : hash(aHash)
{
}

RED4ext::CName::CName(const char* aName)
    : CName(FNV1a(aName))
{
}

const char* RED4ext::CName::operator()()
{
    static REDfunc<char* (*)(uint64_t&)> func(Addresses::CNamePool_Get);
    return func(hash);
}
