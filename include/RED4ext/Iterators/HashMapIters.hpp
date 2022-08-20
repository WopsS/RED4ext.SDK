#pragma once

#include <cassert>
#include <iterator>
#include <memory>

namespace RED4ext
{
template<typename TContainer>
class HashMapConstIter
{
public:
    using iterator_concept = std::forward_iterator_tag;
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using reference = typename TContainer::ConstReference;
    using pointer = typename TContainer::ConstPointer;
    using difference_type = typename TContainer::DifferenceType;

    using NodeType = TContainer::Node;

    constexpr HashMapConstIter(NodeType* aNodes, std::pair<uint32_t*, uint32_t*> aIndexTable)
        : m_nodes(aNodes)
        , m_indexTable(aIndexTable)
        , m_nodeIdx(TContainer::INVALID_INDEX)
    {
        // Make first node avaiable.
        Increment();
    }

    [[nodiscard]] reference operator*() const noexcept
    {
        assert(m_nodeIdx != TContainer::INVALID_INDEX);

        const NodeType& node = m_nodes[m_nodeIdx];
        return node.value;
    }

    [[nodiscard]] pointer operator->() const noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(**this);
    }

    HashMapConstIter& operator++() noexcept
    {
        Increment();
        return *this;
    }

    HashMapConstIter operator++(int) noexcept
    {
        auto tmp = *this;
        ++(*this);

        return tmp;
    }

    [[nodiscard]] constexpr bool operator==(const HashMapConstIter& aRhs) const noexcept
    {
        return m_nodes == aRhs.m_nodes && m_indexTable == aRhs.m_indexTable;
    }

private:
    void Increment() noexcept
    {
        // Incrementing tableStart after checking INVALID_INDEX is done to prevent premature end checking of the
        // iterator.

        auto& [tableStart, tableEnd] = m_indexTable;

        if (m_nodeIdx != TContainer::INVALID_INDEX)
        {
            const NodeType& node = m_nodes[m_nodeIdx];

            m_nodeIdx = node.next;
            if (m_nodeIdx != TContainer::INVALID_INDEX)
            {
                return;
            }

            tableStart++;
        }

        while (tableStart != tableEnd)
        {
            m_nodeIdx = *tableStart;
            if (m_nodeIdx != TContainer::INVALID_INDEX)
            {
                return;
            }

            tableStart++;
        }
    }

    NodeType* m_nodes;
    std::pair<uint32_t*, uint32_t*> m_indexTable;
    uint32_t m_nodeIdx;
};

template<typename TContainer>
class HashMapIter : public HashMapConstIter<TContainer>
{
public:
    using BaseType = HashMapConstIter<TContainer>;

    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TContainer::ValueType;
    using reference = typename TContainer::Reference;
    using pointer = typename TContainer::Pointer;
    using difference_type = typename TContainer::DifferenceType;

    using BaseType::HashMapConstIter;

    [[nodiscard]] reference operator*() const noexcept
    {
        return const_cast<reference>(BaseType::operator*());
    }

    [[nodiscard]] pointer operator->() const noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(**this);
    }

    constexpr HashMapIter& operator++() noexcept
    {
        BaseType::operator++();
        return *this;
    }

    constexpr HashMapIter& operator++(int) noexcept
    {
        auto tmp = *this;
        BaseType::operator++();

        return tmp;
    }
};
} // namespace RED4ext
