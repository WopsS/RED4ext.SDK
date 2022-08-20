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
    using iterator_concept = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using reference = typename TContainer::ConstReference;
    using pointer = typename TContainer::ConstPointer;
    using difference_type = typename TContainer::DifferenceType;

    using KeyType = typename DynArray<typename TContainer::KeyType>::Iterator;
    using MappedType = typename DynArray<typename TContainer::MappedType>::Iterator;

    constexpr MapConstIter(KeyType aKeys = nullptr, MappedType aValues = nullptr) noexcept
        : m_keys(aKeys)
        , m_values(aValues)
    {
    }

    // https://stackoverflow.com/questions/52856168/arrow-operator-operator-return-type-when-dereference-operator-returns-by
    // c++ input iterator arrow operator
    // https://stackoverflow.com/questions/19655941/create-iterator-to-c-container-that-returns-a-stdpair
    // https://www.reedbeta.com/blog/ranges-compatible-containers/

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

    using iterator_concept = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using reference = typename TContainer::Reference;
    using pointer = typename TContainer::Pointer;
    using difference_type = typename TContainer::DifferenceType;

    using BaseType::MapConstIter;

    constexpr MapIter(const MapIter&) noexcept = default;
    constexpr MapIter(MapIter&&) noexcept = default;
    
    constexpr MapIter& operator=(const MapIter&) noexcept = default;
    constexpr MapIter& operator=(MapIter&&) noexcept = default;

    constexpr ~MapIter() noexcept = default;

    constexpr MapIter& operator++() noexcept
    {
        BaseType::operator++();
        return *this;
    }

    constexpr MapIter& operator++(int) noexcept
    {
        MapIter tmp = *this;
        BaseType::operator++();

        return tmp;
    }
};
} // namespace RED4ext
