#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <RED4ext/Common.hpp>
#include <RED4ext/Hashing/FNV1a.hpp>
#include <RED4ext/Iterators/HashMapIters.hpp>
#include <RED4ext/Memory/Allocators.hpp>

namespace RED4ext
{
#pragma region HashMapHash
template<typename T, typename enabled = void /* for enable_if specialization */>
struct HashMapHash
{
    uint32_t operator()(const T& aKey) const noexcept
    {
        // You can pass your own hasher as the 3rd template argument to the HashMap
        // Or you can make a specialized version of this class

        static_assert(!std::is_same_v<enabled, void>,
                      "Please define a HashMapHash<T> specialization for your key type");
    }
};

template<typename T>
struct HashMapHash<T, std::enable_if_t<std::is_same_v<T, uint32_t>>>
{
    // Let's give this a try :)

    uint32_t operator()(const T& aKey) const noexcept
    {
        return static_cast<uint32_t>(aKey);
    }
};

template<typename T>
struct HashMapHash<T, std::enable_if_t<std::is_pointer_v<T>>>
{
    // Used in TweakDB, probably the same for all pointer keys

    uint32_t operator()(const T& aKey) const noexcept
    {
        return FNV1a32(reinterpret_cast<const uint8_t*>(&aKey), sizeof(void*));
    }
};
#pragma endregion

template<typename K, typename T, typename Hasher = HashMapHash<K>>
struct HashMap
{
    using ValueType = std::pair<const K, T>;
    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;
    using DifferenceType = std::ptrdiff_t;

    using Iterator = HashMapIter<HashMap<K, T, Hasher>>;
    using ConstIterator = HashMapConstIter<HashMap<K, T, Hasher>>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    static const uint32_t INVALID_INDEX = static_cast<uint32_t>(-1);

    struct Node
    {
        uint32_t next;      // 00
        uint32_t hashedKey; // 04
        ValueType value;    // 08
    };

    struct NodeList
    {
        Node* nodes;       // 00
        uint32_t capacity; // 08
        uint32_t stride;   // 0C - sizeof(Node)
        uint32_t nextIdx;  // 10 - next available node index to write to
        uint32_t size;     // 14

        NodeList(Node* aNodes = nullptr, uint32_t aCapacity = 0)
            : nodes(aNodes)
            , capacity(aCapacity)
            , stride(sizeof(Node))
            , nextIdx(aNodes ? 0 : -1)
            , size(0)
        {
        }

        Node* GetNextAvailNode()
        {
            if (nextIdx == INVALID_INDEX)
                return nullptr;

            if (size == nextIdx)
            {
                if (size + 1 < capacity)
                {
                    ++size;
                    return &nodes[nextIdx++];
                }
                else
                {
                    Node* node = &nodes[nextIdx];
                    nextIdx = INVALID_INDEX;
                    return node;
                }
            }
            else
            {
                Node* node = &nodes[nextIdx];
                nextIdx = node->next;
                return node;
            }
        }
    };

    HashMap(Memory::IAllocator* aAllocator = nullptr)
        : indexTable(nullptr)
        , size(0)
        , capacity(0)
    {
        // vftable because IMemoryAllocator is abstract
        allocator = aAllocator ? *reinterpret_cast<uintptr_t*>(aAllocator) : 0;
    }

    void for_each(std::function<void(const K&, T&)> aFunctor) const
    {
        ForEach(aFunctor);
    }

    void ForEach(std::function<void(const K&, T&)> aFunctor) const
    {
        if (size == 0)
            return;

        for (uint32_t index = 0; index != capacity; ++index)
        {
            uint32_t idx = indexTable[index];
            while (idx != INVALID_INDEX)
            {
                Node* node = &nodeList.nodes[idx];
                auto& value = node->value;

                aFunctor(value.first, value.second);
                idx = node->next;
            }
        }
    }

    T* Get(const K& aKey) const
    {
        if (size == 0)
            return nullptr;

        uint32_t hashedKey = Hasher{}(aKey);
        uint32_t idx = indexTable[hashedKey % capacity];
        while (idx != INVALID_INDEX)
        {
            Node* node = &nodeList.nodes[idx];
            auto& value = node->value;

            if (node->hashedKey == hashedKey && value.first == aKey)
            {
                return &value.second;
            }

            idx = node->next;
        }

        return nullptr;
    }

    bool Remove(const K& aKey)
    {
        // This function is guessed based on how the other functions are implemented
        // Couldn't easily find it in the game -Sombra

        if (size == 0)
            return false;

        uint32_t hashedKey = Hasher{}(aKey);
        uint32_t idx = indexTable[hashedKey % capacity];
        while (idx != INVALID_INDEX)
        {
            Node* prevNode = nullptr;
            Node* node = &nodeList.nodes[idx];
            const auto& value = node->value;

            if (node->hashedKey == hashedKey && value.first == aKey)
            {
                if (prevNode == nullptr)
                    indexTable[hashedKey % capacity] = node->next;
                else
                    prevNode->next = node->next;

                node->next = nodeList.nextIdx;
                nodeList.nextIdx = static_cast<uint32_t>(node - nodeList.nodes);
                node->~Node();
                --size;
                return true;
            }
            prevNode = node;
            idx = node->next;
        }

        return false;
    }

    std::pair<T*, bool> Insert(const K& aKey, const T& aValue)
    {
        return Emplace(aKey, std::forward<const T&>(aValue));
    }

    std::pair<T*, bool> Insert(const K& aKey, T&& aValue)
    {
        return Emplace(aKey, std::forward<T&&>(aValue));
    }

    std::pair<T*, bool> InsertOrAssign(const K& aKey, const T& aValue)
    {
        std::pair<T*, bool> pair = Emplace(aKey, std::forward<const T&>(aValue));
        if (!pair.second)
        {
            *pair.first = aValue;
        }
        return pair;
    }

    std::pair<T*, bool> InsertOrAssign(const K& aKey, T&& aValue)
    {
        std::pair<T*, bool> pair = Emplace(aKey, std::forward<T&&>(aValue));
        if (!pair.second)
        {
            *pair.first = std::move(aValue);
        }
        return pair;
    }

    template<class... TArgs>
    std::pair<T*, bool> Emplace(const K& aKey, TArgs&&... aArgs)
    {
        uint32_t hashedKey = Hasher{}(aKey);

        if (size != 0 && indexTable[hashedKey % capacity] != INVALID_INDEX)
        {
            uint32_t idx = indexTable[hashedKey % capacity];
            while (idx != INVALID_INDEX)
            {
                Node* node = &nodeList.nodes[idx];
                auto& value = node->value;

                if (node->hashedKey == hashedKey && value.first == aKey)
                {
                    return {&value.second, false};
                }

                idx = node->next;
            }
        }

        if (size + 1 > capacity)
        {
            uint32_t newCapacity = size + (size / 2);
            if (newCapacity < 4)
                newCapacity = 4;
            Reserve(newCapacity);
        }

        // shouldn't be null. The game doesn't even check
        Node* node = nodeList.GetNextAvailNode();
        node->hashedKey = hashedKey;

        std::construct_at(std::addressof(node->value.first), aKey);
        std::construct_at(std::addressof(node->value.second), std::forward<TArgs>(aArgs)...);

        node->next = indexTable[hashedKey % capacity];
        indexTable[hashedKey % capacity] = static_cast<uint32_t>(node - nodeList.nodes);
        ++size;
        return {&node->value.second, true};
    }

    void Clear()
    {
        for (uint32_t i = 0; i != capacity; ++i)
        {
            uint32_t idx = indexTable[i];
            while (idx != INVALID_INDEX)
            {
                Node* node = &nodeList.nodes[idx];
                idx = node->next;
                node->~Node();
            }

            indexTable[i] = static_cast<uint32_t>(-1);
        }
        size = 0;
        nodeList.nextIdx = 0;
        nodeList.size = 0;
    }

    void Reserve(uint32_t aSize)
    {
        if (aSize <= capacity)
            return;

        uint32_t requiredBytes = aSize * (sizeof(Node) + 4 /* *indexTable */);
        auto allocResult = GetAllocator()->AllocAligned(requiredBytes, 8);
        memset(allocResult.memory, 0, allocResult.size);
        NodeList newNodeList(reinterpret_cast<Node*>(allocResult.memory), aSize);
        uint32_t* newIndexTable = reinterpret_cast<uint32_t*>((uintptr_t)allocResult.memory + (aSize * sizeof(Node)));

        for (uint32_t i = 0; i != aSize; ++i)
        {
            newIndexTable[i] = INVALID_INDEX;
        }

        if (capacity != 0)
        {
            if (size != 0)
            {
                for (uint32_t i = 0; i != capacity; ++i)
                {
                    uint32_t idx = indexTable[i];
                    while (idx != INVALID_INDEX)
                    {
                        Node* oldNode = &nodeList.nodes[idx];
                        uint32_t hashedKey = oldNode->hashedKey;

                        Node* node = newNodeList.GetNextAvailNode();
                        node->hashedKey = hashedKey;

                        std::construct_at(std::addressof(node->value.first), oldNode->value.first);
                        std::construct_at(std::addressof(node->value.second), std::move(oldNode->value.second));

                        node->next = newIndexTable[hashedKey % aSize];
                        newIndexTable[hashedKey % aSize] = static_cast<uint32_t>(node - newNodeList.nodes);

                        idx = oldNode->next;
                        oldNode->~Node();
                    }

                    indexTable[i] = INVALID_INDEX;
                }
            }

            GetAllocator()->Free(nodeList.nodes);
        }

        capacity = aSize;
        indexTable = newIndexTable;
        nodeList = newNodeList;
    }

    Memory::IAllocator* GetAllocator()
    {
        return reinterpret_cast<Memory::IAllocator*>(&allocator);
    }

    [[nodiscard]] constexpr Iterator Begin() noexcept
    {
        return Iterator(nodeList.nodes, {indexTable, indexTable + capacity});
    }

    [[nodiscard]] constexpr ConstIterator Begin() const noexcept
    {
        return ConstIterator(nodeList.nodes, {indexTable, indexTable + capacity});
    }

    [[nodiscard]] constexpr Iterator End() noexcept
    {
        return Iterator(nodeList.nodes, {indexTable + capacity, indexTable + capacity});
    }

    [[nodiscard]] constexpr ConstIterator End() const noexcept
    {
        return ConstIterator(nodeList.nodes, {indexTable + capacity, indexTable + capacity});
    }

#pragma region STL
    [[nodiscard]] constexpr Iterator begin() noexcept
    {
        return Begin();
    }

    [[nodiscard]] constexpr ConstIterator begin() const noexcept
    {
        return Begin();
    }

    [[nodiscard]] constexpr Iterator end() noexcept
    {
        return End();
    }

    [[nodiscard]] constexpr ConstIterator end() const noexcept
    {
        return End();
    }
#pragma endregion

    uint32_t* indexTable; // 00
    uint32_t size;        // 08
    uint32_t capacity;    // 0C
    NodeList nodeList;    // 10
    uintptr_t allocator;  // 28
};
RED4EXT_ASSERT_SIZE(RED4EXT_ASSERT_ESCAPE(HashMap<uint64_t, void*>), 0x30);
RED4EXT_ASSERT_SIZE(RED4EXT_ASSERT_ESCAPE(HashMap<uint64_t, void*>::Node), 0x8 + sizeof(uint64_t) + sizeof(void*));
} // namespace RED4ext
