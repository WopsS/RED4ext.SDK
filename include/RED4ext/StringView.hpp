#pragma once

#include <RED4ext/Common.hpp>

#include <string_view>

namespace RED4ext
{
struct CString;

#pragma pack(push, 4)
struct StringView
{
    RED4EXT_INLINE constexpr StringView() noexcept
        : ptr(nullptr)
        , len(0u)
    {
    }

    RED4EXT_INLINE constexpr StringView(const char* aStr) noexcept
        : ptr(aStr)
        , len(std::char_traits<char>::length(aStr))
    {
    }

    RED4EXT_INLINE constexpr StringView(std::string_view aView) noexcept
        : ptr(aView.data())
        , len(static_cast<std::uint32_t>(aView.size()))
    {
    }

    RED4EXT_INLINE StringView(const RED4ext::CString& aStr) noexcept;

    RED4EXT_INLINE constexpr bool IsValid() const noexcept
    {
        return ptr && len > 0u;
    }

    RED4EXT_INLINE constexpr operator bool() const noexcept
    {
        return IsValid();
    }

    RED4EXT_INLINE constexpr bool operator==(const StringView& aRhs) const noexcept
    {
        return Length() == aRhs.Length() && std::char_traits<char>::compare(Data(), aRhs.Data(), Length());
    }

    RED4EXT_INLINE constexpr bool operator!=(const StringView& aRhs) const noexcept
    {
        return !(*this == aRhs);
    }

    RED4EXT_INLINE constexpr bool operator==(const char* aRhs) const noexcept
    {
        return *this == StringView{aRhs};
    }

    RED4EXT_INLINE constexpr bool operator!=(const char* aRhs) const noexcept
    {
        return *this != StringView{aRhs};
    }

    RED4EXT_INLINE constexpr bool operator==(std::string_view aRhs) const noexcept
    {
        return *this == StringView{aRhs};
    }

    RED4EXT_INLINE constexpr bool operator!=(std::string_view aRhs) const noexcept
    {
        return *this != StringView{aRhs};
    }

    bool operator==(const RED4ext::CString& aRhs) const noexcept;
    bool operator!=(const RED4ext::CString& aRhs) const noexcept;

    RED4EXT_INLINE constexpr char operator[](std::size_t aIndex) const noexcept
    {
        return Data()[aIndex];
    }

    RED4EXT_INLINE constexpr const char* Data() const noexcept
    {
        return ptr;
    }

    RED4EXT_INLINE constexpr std::uint32_t Length() const noexcept
    {
        return len;
    }

    RED4EXT_INLINE constexpr const char* begin() const noexcept
    {
        return Data();
    }

    RED4EXT_INLINE constexpr const char* end() const noexcept
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
} // namespace RED4ext

#ifdef RED4EXT_HEADER_ONLY
#include <RED4ext/StringView-inl.hpp>
#endif // !RED4EXT_STATIC_LIB
