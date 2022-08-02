#pragma once

#include <iterator>

namespace RED4ext
{
template<typename TContainer>
class DynArrayConstIter
{
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using difference_type = typename TContainer::DifferenceType;
    using pointer = typename TContainer::ConstPointer;
    using reference = typename TContainer::ConstReference;

    using ElementsType = typename TContainer::Pointer;

    constexpr DynArrayConstIter(ElementsType aElements = nullptr) noexcept
        : m_elements(aElements)
    {
    }

    constexpr DynArrayConstIter(const DynArrayConstIter&) noexcept = default;
    constexpr DynArrayConstIter(DynArrayConstIter&&) noexcept = default;

    constexpr DynArrayConstIter& operator=(const DynArrayConstIter&) noexcept = default;
    constexpr DynArrayConstIter& operator=(DynArrayConstIter&&) noexcept = default;

    constexpr ~DynArrayConstIter() noexcept = default;

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

    [[nodiscard]] constexpr DynArrayConstIter operator+(const difference_type aOffset) const noexcept
    {
        DynArrayConstIter tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr DynArrayConstIter operator-(const difference_type aOffset) const noexcept
    {
        DynArrayConstIter tmp = *this;
        tmp -= aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr difference_type operator-(const DynArrayConstIter& aRhs) const noexcept
    {
        return m_elements - aRhs.m_elements;
    }

    constexpr DynArrayConstIter& operator++() noexcept
    {
        ++m_elements;
        return *this;
    }

    constexpr DynArrayConstIter operator++(int) noexcept
    {
        DynArrayConstIter tmp = *this;
        ++(*this);

        return tmp;
    }

    constexpr DynArrayConstIter& operator--() noexcept
    {
        --m_elements;
        return *this;
    }

    constexpr DynArrayConstIter operator--(int) noexcept
    {
        DynArrayConstIter tmp = *this;
        --(*this);

        return tmp;
    }

    constexpr DynArrayConstIter& operator+=(const difference_type aOffset) noexcept
    {
        m_elements += aOffset;
        return *this;
    }

    constexpr DynArrayConstIter& operator-=(const difference_type aOffset) noexcept
    {
        return *this += -aOffset;
    }

    [[nodiscard]] constexpr bool operator==(const DynArrayConstIter& aRhs) const noexcept
    {
        return m_elements == aRhs.m_elements;
    }

    [[nodiscard]] constexpr std::strong_ordering operator<=>(const DynArrayConstIter& aRhs) const noexcept
    {
        // TODO: _Unfancy
        return m_elements <=> aRhs.m_elements;
    }

private:
    ElementsType m_elements;
};

template<typename TContainer>
class DynArrayIter : public DynArrayConstIter<TContainer>
{
public:
    using BaseType = DynArrayConstIter<TContainer>;

    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using difference_type = typename TContainer::DifferenceType;
    using pointer = typename TContainer::Pointer;
    using reference = typename TContainer::Reference;

    using BaseType::DynArrayConstIter;

    constexpr DynArrayIter(const DynArrayIter&) noexcept = default;
    constexpr DynArrayIter(DynArrayIter&&) noexcept = default;

    constexpr DynArrayIter& operator=(const DynArrayIter&) noexcept = default;
    constexpr DynArrayIter& operator=(DynArrayIter&&) noexcept = default;

    constexpr ~DynArrayIter() noexcept = default;

    using BaseType::operator-;

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return const_cast<reference>(BaseType::operator*());
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return const_cast<pointer>(BaseType::operator->());
    }

    [[nodiscard]] constexpr reference operator[](const difference_type aOffset) const noexcept
    {
        return const_cast<reference>(BaseType::operator[](aOffset));
    }

    [[nodiscard]] constexpr DynArrayIter operator+(const difference_type aOffset) const noexcept
    {
        DynArrayIter tmp = *this;
        tmp += aOffset;

        return tmp;
    }

    [[nodiscard]] constexpr DynArrayIter operator-(const difference_type aOffset) const noexcept
    {
        DynArrayIter tmp = *this;
        tmp -= aOffset;

        return tmp;
    }

    constexpr DynArrayIter& operator++() noexcept
    {
        BaseType::operator++();
        return *this;
    }

    constexpr DynArrayIter operator++(int) noexcept
    {
        DynArrayIter tmp = *this;
        BaseType::operator++();

        return tmp;
    }

    constexpr DynArrayIter& operator--() noexcept
    {
        BaseType::operator--();
        return *this;
    }

    constexpr DynArrayIter operator--(int) noexcept
    {
        DynArrayIter tmp = *this;
        BaseType::operator--();

        return tmp;
    }

    constexpr DynArrayIter& operator+=(const difference_type aOffset) noexcept
    {
        BaseType::operator+=(aOffset);
        return *this;
    }

    constexpr DynArrayIter& operator-=(const difference_type aOffset) noexcept
    {
        BaseType::operator-=(aOffset);
        return *this;
    }
};
} // namespace RED4ext
