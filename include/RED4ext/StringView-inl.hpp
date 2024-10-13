#pragma once

#ifdef RED4EXT_STATIC_LIB
#include <RED4ext/StringView.hpp>
#endif

#include <RED4ext/CString.hpp>

RED4EXT_INLINE RED4ext::StringView::StringView(const RED4ext::CString& aStr) noexcept
    : ptr(aStr.c_str())
    , len(aStr.Length())
{
}

RED4EXT_INLINE bool RED4ext::StringView::operator==(const RED4ext::CString& aRhs) const noexcept
{
    return *this == StringView{aRhs};
}

RED4EXT_INLINE bool RED4ext::StringView::operator !=(const RED4ext::CString& aRhs) const noexcept
{
    return *this != StringView{aRhs};
}
