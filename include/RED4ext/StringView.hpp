#pragma once

#include <RED4ext/Common.hpp>
#include <RED4ext/CString.hpp>

#include <string_view>

namespace RED4ext
{
#pragma pack(push, 4)
struct StringView
{
    constexpr StringView() noexcept
        : ptr(nullptr)
        , len(0u)
    {

    }

    constexpr StringView(const char* aStr) noexcept
        : ptr(aStr)
        , len(std::char_traits<char>::length(aStr))
    {

    }

    constexpr StringView(std::string_view aView) noexcept
        : ptr(aView.data())
        , len(static_cast<std::uint32_t>(aView.size()))
    {

    }

    StringView(const RED4ext::CString& aStr) noexcept
        : ptr(aStr.c_str())
        , len(aStr.Length())
    {

    }

    constexpr bool IsValid() const noexcept
    {
        return ptr && len > 0u;
    }

    constexpr operator bool() const noexcept
    {
        return IsValid();
    }

    constexpr bool operator==(const StringView& aRhs) const noexcept
    {
        return Length() == aRhs.Length() && std::char_traits<char>::compare(Data(), aRhs.Data(), Length()); 
    }

    constexpr bool operator!=(const StringView& aRhs) const noexcept
    {
        return !(*this == aRhs);
    }

    constexpr bool operator==(const char* aRhs) const noexcept
    {
        return *this == StringView{aRhs};
    }

    constexpr bool operator!=(const char* aRhs) const noexcept
    {
        return *this != StringView{aRhs};
    }

    constexpr bool operator==(std::string_view aRhs) const noexcept
    {
        return *this == StringView{aRhs};
    }

    constexpr bool operator!=(std::string_view aRhs) const noexcept
    {
        return *this != StringView{aRhs};
    }

    bool operator==(const RED4ext::CString& aRhs) const noexcept
    {
        return *this == StringView{aRhs};
    }

    bool operator!=(const RED4ext::CString& aRhs) const noexcept
    {
        return *this != StringView{aRhs};
    }

    constexpr char operator[](std::size_t aIndex) const noexcept
    {
        return Data()[aIndex];
    }

    constexpr const char* Data() const noexcept
    {
        return ptr;
    }

    constexpr std::uint32_t Length() const noexcept
    {
        return len;
    }

    constexpr const char* begin() const noexcept
    {
        return Data();
    }

    constexpr const char* end() const noexcept
    {
        return Data() + Length();
    }

    const char* ptr{};
    std::uint32_t len{};
};
#pragma pack(pop)

RED4EXT_ASSERT_SIZE(StringView, 12u);
RED4EXT_ASSERT_OFFSET(StringView, ptr, 0u);
RED4EXT_ASSERT_OFFSET(StringView, len, 8u);
}

