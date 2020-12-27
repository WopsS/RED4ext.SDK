#pragma once

#ifdef RED4EXT_STATIC_LIB
#include <RED4ext/Scripting/Stack.hpp>
#endif

#include <malloc.h>

#include <RED4ext/Addresses.hpp>
#include <RED4ext/REDfunc.hpp>

RED4ext::CStack::~CStack()
{
    _freea(this);
}

RED4ext::CStack* RED4ext::CStack::Construct(IScriptable* aScriptable, CStackType* aArgs, uint32_t aArgsCount,
                                            CStackType* aResult, int64_t a6)
{
    using func_t = CStack* (*)(CStack*, IScriptable*, CStackType*, uint32_t, CStackType*, int64_t);
    static REDfunc<func_t> func(Addresses::CStack_ctor);

    auto ptr = static_cast<CStack*>(_malloca(sizeof(CStack)));
    func(ptr, aScriptable, aArgs, aArgsCount, aResult, a6);
    return ptr;
}
