#pragma once

#include <iterator>

namespace RED4ext::Iterators
{
template<typename TDynArray>
class [[nodiscard]] ConstDynArray
{
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TDynArray::ValueType;
    using difference_type = typename TDynArray::DifferenceType;
    using pointer = typename TDynArray::ConstPointer;
    using reference = typename TDynArray::ConstReference;

    using T = typename TDynArray::Pointer;

    constexpr ConstDynArray(T aPtr = nullptr, const TDynArray* aContainer = nullptr) noexcept
        : m_ptr(aPtr)
        , m_container(aContainer)
    {
    }

    ~ConstDynArray() noexcept = default;

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return *m_ptr;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return m_ptr;
    }

    [[nodiscard]] constexpr reference operator[](const difference_type aOffset) const noexcept
    {
        return *(*this + aOffset);
    }

    constexpr ConstDynArray& operator++() noexcept
    {
        ++m_ptr;
        return *this;
    }

    constexpr ConstDynArray operator++(int) noexcept
    {
        ConstDynArray tmp = *this;
        ++*this;

        return tmp;
    }

    constexpr ConstDynArray& operator--() noexcept
    {
        --m_ptr;
        return *this;
    }

    constexpr ConstDynArray operator--(int) noexcept
    {
        ConstDynArray tmp = *this;
        --*this;

        return tmp;
    }

    constexpr ConstDynArray& operator+=(const difference_type aOffset) noexcept
    {
        m_ptr += aOffset;
        return *this;
    }

    [[nodiscard]] constexpr ConstDynArray operator+(const difference_type aOffset) const noexcept
    {
        ConstDynArray tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    constexpr ConstDynArray& operator-=(const difference_type aOffset) noexcept
    {
        return *this += -aOffset;
    }

    [[nodiscard]] constexpr ConstDynArray operator-(const difference_type aOffset) const noexcept
    {
        ConstDynArray tmp = *this;
        tmp -= aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr difference_type operator-(const ConstDynArray& aRhs) const noexcept
    {
        return m_ptr - aRhs.m_ptr;
    }

    [[nodiscard]] constexpr bool operator==(const ConstDynArray& aRhs) const noexcept
    {
        return m_ptr == aRhs.m_ptr;
    }

    [[nodiscard]] bool operator!=(const ConstDynArray& aRhs) const noexcept
    {
        return !(*this == aRhs);
    }

    [[nodiscard]] bool operator<(const ConstDynArray& aRhs) const noexcept
    {
        return m_ptr < aRhs.m_ptr;
    }

    [[nodiscard]] bool operator>(const ConstDynArray& aRhs) const noexcept
    {
        return aRhs < *this;
    }

    [[nodiscard]] bool operator<=(const ConstDynArray& aRhs) const noexcept
    {
        return !(aRhs < *this);
    }

    [[nodiscard]] bool operator>=(const ConstDynArray& aRhs) const noexcept
    {
        return !(*this < aRhs);
    }

private:
    T m_ptr;
    const TDynArray* m_container;
};

template<typename TDynArray>
class [[nodiscard]] DynArray : public ConstDynArray<TDynArray>
{
public:
    using Base = ConstDynArray<TDynArray>;

    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TDynArray::ValueType;
    using difference_type = typename TDynArray::DifferenceType;
    using pointer = typename TDynArray::Pointer;
    using reference = typename TDynArray::Reference;

    using Base::ConstDynArray;
    using Base::operator-;

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return const_cast<reference>(Base::operator*());
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return const_cast<pointer>(Base::operator->());
    }

    [[nodiscard]] constexpr reference operator[](const difference_type aOffset) const noexcept
    {
        return const_cast<reference>(Base::operator[](aOffset));
    }

    constexpr DynArray& operator++() noexcept
    {
        Base::operator++();
        return *this;
    }

    constexpr DynArray operator++(int) noexcept
    {
        DynArray tmp = *this;
        Base::operator++();

        return tmp;
    }

    constexpr DynArray& operator--() noexcept
    {
        Base::operator--();
        return *this;
    }

    constexpr DynArray operator--(int) noexcept
    {
        DynArray tmp = *this;
        Base::operator--();

        return tmp;
    }

    constexpr DynArray& operator+=(const difference_type aOffset) noexcept
    {
        Base::operator+=(aOffset);
        return *this;
    }

    [[nodiscard]] constexpr DynArray operator+(const difference_type aOffset) const noexcept
    {
        DynArray tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    constexpr DynArray& operator-=(const difference_type aOffset) noexcept
    {
        Base::operator-=(aOffset);
        return *this;
    }

    [[nodiscard]] constexpr DynArray operator-(const difference_type aOffset) const noexcept
    {
        DynArray tmp = *this;
        tmp -= aOffset;

        return tmp;
    }
};
} // namespace RED4ext::Iterators
