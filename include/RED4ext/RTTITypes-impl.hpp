#pragma once

#ifdef RED4EXT_STATIC_LIB
#include <RED4ext/RTTITypes.hpp>
#endif

#include <RED4ext/Addresses.hpp>
#include <RED4ext/REDfunc.hpp>

RED4ext::CClassFunction* RED4ext::CClass::GetFunction(CName aName)
{
    using func_t = CClassFunction* (*)(CClass*, CName);
    static REDfunc<func_t> func(Addresses::CClass_GetFunction);
    return func(this, aName);
}
