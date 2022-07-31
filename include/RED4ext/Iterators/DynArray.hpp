#pragma once

#include <iterator>

namespace RED4ext::Iterators
{
template<typename TContainer>
class ConstDynArray
{
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using difference_type = typename TContainer::DifferenceType;
    using pointer = typename TContainer::ConstPointer;
    using reference = typename TContainer::ConstReference;

    using Elements_t = typename TContainer::Pointer;

    constexpr ConstDynArray(Elements_t aElements = nullptr) noexcept
        : m_elements(aElements)
    {
    }

    constexpr ConstDynArray(const ConstDynArray&) noexcept = default;
    constexpr ConstDynArray(ConstDynArray&&) noexcept = default;

    constexpr ConstDynArray& operator=(const ConstDynArray&) noexcept = default;
    constexpr ConstDynArray& operator=(ConstDynArray&&) noexcept = default;

    constexpr ~ConstDynArray() noexcept = default;

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return *m_elements;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return m_elements;
    }

    [[nodiscard]] constexpr reference operator[](const difference_type aOffset) const noexcept
    {
        return *(*this + aOffset);
    }

    [[nodiscard]] constexpr ConstDynArray operator+(const difference_type aOffset) const noexcept
    {
        ConstDynArray tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr ConstDynArray operator-(const difference_type aOffset) const noexcept
    {
        ConstDynArray tmp = *this;
        tmp -= aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr difference_type operator-(const ConstDynArray& aRhs) const noexcept
    {
        return m_elements - aRhs.m_elements;
    }

    constexpr ConstDynArray& operator++() noexcept
    {
        ++m_elements;
        return *this;
    }

    constexpr ConstDynArray operator++(int) noexcept
    {
        ConstDynArray tmp = *this;
        ++(*this);

        return tmp;
    }

    constexpr ConstDynArray& operator--() noexcept
    {
        --m_elements;
        return *this;
    }

    constexpr ConstDynArray operator--(int) noexcept
    {
        ConstDynArray tmp = *this;
        --(*this);

        return tmp;
    }

    constexpr ConstDynArray& operator+=(const difference_type aOffset) noexcept
    {
        m_elements += aOffset;
        return *this;
    }

    constexpr ConstDynArray& operator-=(const difference_type aOffset) noexcept
    {
        return *this += -aOffset;
    }

    [[nodiscard]] constexpr bool operator==(const ConstDynArray& aRhs) const noexcept
    {
        return m_elements == aRhs.m_elements;
    }

    [[nodiscard]] bool operator!=(const ConstDynArray& aRhs) const noexcept
    {
        return !(*this == aRhs);
    }

    [[nodiscard]] bool operator<(const ConstDynArray& aRhs) const noexcept
    {
        return m_elements < aRhs.m_elements;
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
    Elements_t m_elements;
};

template<typename TContainer>
class DynArray : public ConstDynArray<TContainer>
{
public:
    using Base_t = ConstDynArray<TContainer>;

    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using difference_type = typename TContainer::DifferenceType;
    using pointer = typename TContainer::Pointer;
    using reference = typename TContainer::Reference;

    using Base_t::ConstDynArray;

    constexpr DynArray(const DynArray&) noexcept = default;
    constexpr DynArray(DynArray&&) noexcept = default;

    constexpr DynArray& operator=(const DynArray&) noexcept = default;
    constexpr DynArray& operator=(DynArray&&) noexcept = default;

    constexpr ~DynArray() noexcept = default;

    using Base_t::operator-;

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return const_cast<reference>(Base_t::operator*());
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return const_cast<pointer>(Base_t::operator->());
    }

    [[nodiscard]] constexpr reference operator[](const difference_type aOffset) const noexcept
    {
        return const_cast<reference>(Base_t::operator[](aOffset));
    }

    [[nodiscard]] constexpr DynArray operator+(const difference_type aOffset) const noexcept
    {
        DynArray tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr DynArray operator-(const difference_type aOffset) const noexcept
    {
        DynArray tmp = *this;
        tmp -= aOffset;

        return tmp;
    }

    constexpr DynArray& operator++() noexcept
    {
        Base_t::operator++();
        return *this;
    }

    constexpr DynArray operator++(int) noexcept
    {
        DynArray tmp = *this;
        Base_t::operator++();

        return tmp;
    }

    constexpr DynArray& operator--() noexcept
    {
        Base_t::operator--();
        return *this;
    }

    constexpr DynArray operator--(int) noexcept
    {
        DynArray tmp = *this;
        Base_t::operator--();

        return tmp;
    }

    constexpr DynArray& operator+=(const difference_type aOffset) noexcept
    {
        Base_t::operator+=(aOffset);
        return *this;
    }

    constexpr DynArray& operator-=(const difference_type aOffset) noexcept
    {
        Base_t::operator-=(aOffset);
        return *this;
    }
};
} // namespace RED4ext::Iterators
