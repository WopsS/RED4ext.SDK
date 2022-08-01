#pragma once

#include <compare>
#include <iterator>
#include <utility>

#include <RED4ext/DynArray.hpp>

namespace RED4ext
{
template<typename TContainer>
class MapConstIter
{
public:
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using difference_type = typename TContainer::DifferenceType;
    using pointer = typename TContainer::ConstPointer;
    using reference = typename TContainer::ConstReference;

    using KeyType = typename DynArray<typename TContainer::KeyType>::Iterator;
    using MappedType = typename DynArray<typename TContainer::MappedType>::Iterator;

    constexpr MapConstIter(KeyType aKeys = nullptr, MappedType aValues = nullptr) noexcept
        : m_keys(aKeys)
        , m_values(aValues)
    {
    }

    constexpr MapConstIter(const MapConstIter&) noexcept = default;
    constexpr MapConstIter(MapConstIter&&) noexcept = default;

    constexpr MapConstIter& operator=(const MapConstIter&) noexcept = default;
    constexpr MapConstIter& operator=(MapConstIter&&) noexcept = default;

    constexpr ~MapConstIter() noexcept = default;

    [[nodiscard]] value_type operator*() const noexcept
    {
        return {*m_keys, *m_values};
    }

    constexpr MapConstIter& operator++() noexcept
    {
        ++m_keys;
        ++m_values;

        return *this;
    }

    constexpr MapConstIter& operator++(int) noexcept
    {
        MapConstIter tmp = *this;
        ++(*this);

        return tmp;
    }

    [[nodiscard]] constexpr bool operator==(const MapConstIter& aRhs) const noexcept
    {
        return m_keys == aRhs.m_keys && m_values == aRhs.m_values;
    }

    [[nodiscard]] constexpr std::strong_ordering operator<=>(const MapConstIter& aRhs) const noexcept = default;

private:
    KeyType m_keys;
    MappedType m_values;
};

template<typename TContainer>
class MapIter : MapConstIter<TContainer>
{
public:
    using BaseType = MapConstIter<TContainer>;

    using BaseType::MapConstIter;
};
} // namespace RED4ext
