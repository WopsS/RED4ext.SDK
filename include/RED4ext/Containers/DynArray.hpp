#pragma once

#include <cassert>
#include <cstdint>

#include <RED4ext/Common.hpp>
#include <RED4ext/Detail/Memory.hpp>
#include <RED4ext/Memory/Allocators.hpp>

////////////////////////
#include <algorithm>
#include <cstdint>
#include <functional>
#include <type_traits>

#include <RED4ext/Addresses.hpp>
#include <RED4ext/Iterators/DynArrayIters.hpp>
#include <RED4ext/Relocation.hpp>

namespace RED4ext
{
namespace Containers
{
template<typename T, typename TAllocator = Allocator<T>>
requires Detail::IsAllocator_New<TAllocator>
struct DynArray
{
    using AllocatorType = TAllocator;

    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;

    using SizeType = std::uint32_t;
    using DifferenceType = std::ptrdiff_t;

    /*
     * Future-proof assert.
     *
     * The game stores the allocator as the last entry in the "entries" field (entries[capacity]). The DynArray /
     * DynamicBuffer always allocate capacity to fit this allocator.
     *
     * But, I cannot figure out how CDPR pass their allocators in constructor, thus our class will use a placement new
     * to store the allocator in the "entries" pointer.
     *
     * From what I see the size of the allocators is always 8. This assert will make sure that the size of the
     * allocators used in the template parameter will never be higher than of "RED4ext::Allocator" to prevent undefined
     * behavior (e.g. constructing the allocator and overriding some unrelated memory).
     *
     * We could allocate more memory to fit a custom allocator (sizeof(TAllocator) > 8) but I saw that the functions
     * related to the DynArray are always using 8 bytes for the allocator.
     */
    static_assert(sizeof(TAllocator) == sizeof(Allocator<void*>),
                  "The size of the specified allocator must be equal with the size of 'RED4ext::Allocator'");

    constexpr explicit DynArray(const TAllocator& aAllocator = TAllocator()) noexcept
        : entries(nullptr)
        , size(0)
        , capacity(0)
    {
        SetAllocator(aAllocator);
    }

    // constexpr DynArray(SizeType aCount, const T& aValue, const TAllocator& aAllocator = TAllocator()) noexcept;
    // constexpr explicit DynArray(SizeType aCount, const TAllocator& aAllocator = TAllocator()) noexcept;

    // template<class InputIt>
    // constexpr DynArray(InputIt aFirst, InputIt aLast, const TAllocator& aAllocator = TAllocator()) noexcept;

    // constexpr DynArray(std::initializer_list<T> aInit, const TAllocator& aAllocator = TAllocator()) noexcept;
    // constexpr DynArray(const DynArray& aOther) noexcept;
    // constexpr DynArray(const DynArray& aOther, const TAllocator& aAllocator) noexcept;
    // constexpr DynArray(DynArray&& aOther) noexcept;
    // constexpr DynArray(DynArray&& aOther, const TAllocator& aAllocator) noexcept;

    // constexpr DynArray& operator=(const DynArray& aOther) noexcept;
    // constexpr DynArray& operator=(DynArray&& aOther) noexcept;
    // constexpr DynArray& operator=(std::initializer_list<T> aInit) noexcept;

    // constexpr ~DynArray() noexcept;

    ////////////////////////////////////////////////////////////////////////////////////////

    // constexpr void Assign(SizeType aCount, const T& aValue) noexcept;

    // template<class InputIt>
    // constexpr void Assign(InputIt aFirst, InputIt aLast) noexcept;

    // constexpr void Assign(std::initializer_list<T> aInit) noexcept;

    ////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] constexpr Reference operator[](SizeType aIndex) noexcept
    {
        assert(aIndex < Size());
        return Data()[aIndex];
    }

    [[nodiscard]] constexpr ConstReference operator[](SizeType aIndex) const noexcept
    {
        assert(aIndex < Size());
        return Data()[aIndex];
    }

    [[nodiscard]] constexpr Reference At(SizeType aIndex) noexcept
    {
        if (aIndex >= Size())
        {
            throw std::out_of_range("DynArray::at out of range");
        }

        return Data()[aIndex];
    }

    [[nodiscard]] constexpr ConstReference At(SizeType aIndex) const noexcept
    {
        if (aIndex >= Size())
        {
            throw std::out_of_range("DynArray::at out of range");
        }

        return Data()[aIndex];
    }

    [[nodiscard]] constexpr Reference Front() noexcept
    {
        assert(!Empty());
        return Data()[0];
    }

    [[nodiscard]] constexpr ConstReference Front() const noexcept
    {
        assert(!Empty());
        return Data()[0];
    }

    [[nodiscard]] constexpr Reference Back() noexcept
    {
        assert(!Empty());
        return Data()[Size() - 1];
    }

    [[nodiscard]] constexpr ConstReference Back() const noexcept
    {
        assert(!Empty());
        return Data()[Size() - 1];
    }

    [[nodiscard]] constexpr T* Data() noexcept
    {
        return entries;
    }

    [[nodiscard]] constexpr const T* Data() const noexcept
    {
        return entries;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] constexpr bool Empty() const noexcept
    {
        return Size() == 0;
    }

    [[nodiscard]] constexpr SizeType Size() const noexcept
    {
        return size;
    }

    [[nodiscard]] constexpr SizeType MaxSize() const noexcept
    {
        return static_cast<SizeType>(-1) / sizeof(ValueType);
    }

    [[nodiscard]] constexpr SizeType Capacity() const noexcept
    {
        return capacity;
    }

    constexpr void Reserve(SizeType aNewCapacity)
    {
        // Allocate new storage only if the new capacy is greater than the current capacity.
        // This function will never shrink.
        if (aNewCapacity > Capacity())
        {
            if (aNewCapacity > MaxSize())
            {
                throw std::length_error("DynArray::Reserve too many elements much");
            }

            //_Reallocate_exactly(_Newcapacity);

            // TODO: Do reserve.
        }
    }

    constexpr void ShrinkToFit()
    {
        // TODO: Do shrink to fit.
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    // constexpr void clear() noexcept;

    // constexpr iterator insert(const_iterator pos, const T& value);
    // constexpr iterator insert(const_iterator pos, T&& value);
    // constexpr iterator insert(const_iterator pos, size_type count, const T& value);

    // template<class InputIt>
    // constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);

    // constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    // template<class... Args>
    // constexpr iterator emplace(const_iterator pos, Args&&... args);

    // constexpr iterator erase(const_iterator pos);
    // constexpr iterator erase(const_iterator first, const_iterator last);

    // constexpr void push_back(const T& value);
    // constexpr void push_back(T&& value);

    // template<class... Args>
    // constexpr reference emplace_back(Args&&... args);

    // constexpr void pop_back();

    // constexpr void resize(size_type count);
    // constexpr void resize(size_type count, const value_type& value);

    // constexpr void swap(DynArray& other) noexcept;

    ////////////////////////////////////////////////////////////////////////////////////////
#pragma region STL
    // constexpr iterator begin() noexcept;
    // constexpr const_iterator begin() const noexcept;
    // constexpr const_iterator cbegin() const noexcept;

    // constexpr iterator end() noexcept;
    // constexpr const_iterator end() const noexcept;
    // constexpr const_iterator cend() const noexcept;

    // constexpr reverse_iterator rbegin() noexcept;
    // constexpr const_reverse_iterator rbegin() const noexcept;
    // constexpr const_reverse_iterator crbegin() const noexcept;

    // constexpr reverse_iterator rend() noexcept;
    // constexpr const_reverse_iterator rend() const noexcept;
    // constexpr const_reverse_iterator crend() const noexcept;
#pragma endregion
    ////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] constexpr AllocatorType GetAllocator() const noexcept
    {
        if (!entries)
        {
            return {};
        }

        return *reinterpret_cast<AllocatorType*>(&entries[capacity]);
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    // constexpr bool operator==(const DynArray& aRhs);
    // constexpr bool operator<=>(const DynArray& aRhs);

    ////////////////////////////////////////////////////////////////////////////////////////

    T* entries;             // 00
    std::uint32_t capacity; // 08
    std::uint32_t size;     // 0C

private:
    void SetAllocator(const AllocatorType& aAllocator)
    {
        auto location = reinterpret_cast<AllocatorType*>(std::addressof(entries));
        if (entries)
        {
            location = reinterpret_cast<AllocatorType*>(std::addressof(entries[capacity]));
        }

        std::construct_at(location, aAllocator);
    }
};
} // namespace Containers

namespace Memory
{
struct IAllocator;
}

template<typename T>
struct DynArray
{
    using ValueType = T;
    using Pointer = T*;
    using ConstPointer = const T*;
    using Reference = T&;
    using ConstReference = const T&;
    using SizeType = std::uint32_t;
    using DifferenceType = std::ptrdiff_t;

    using Iterator = DynArrayIter<DynArray<T>>;
    using ConstIterator = DynArrayConstIter<DynArray<T>>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    DynArray(Memory::IAllocator* aAllocator = nullptr)
        : entries(aAllocator ? *reinterpret_cast<T**>(aAllocator) : nullptr)
        , size(0)
        , capacity(0)
    {
    }

    const T& operator[](SizeType aIndex) const
    {
        return entries[aIndex];
    }

    T& operator[](SizeType aIndex)
    {
        return entries[aIndex];
    }

    void PushBack(const T& aItem)
    {
        EmplaceBack(std::forward<const T&>(aItem));
    }

    void PushBack(T&& aItem)
    {
        EmplaceBack(std::forward<T&&>(aItem));
    }

    template<class... TArgs>
    void EmplaceBack(TArgs&&... aArgs)
    {
        Emplace(end(), std::forward<TArgs>(aArgs)...);
    }

    template<class... TArgs>
    void Emplace(ConstIterator aPosition, TArgs&&... aArgs)
    {
        uint32_t posIdx = static_cast<uint32_t>(aPosition - begin());
        uint32_t newSize = size + 1;
        if (newSize > capacity)
        {
            Reserve(newSize);
        }

        // If not at the end
        if (posIdx != size)
        {
            uint32_t entriesCount = size - posIdx;
            MoveEntries(&entries[posIdx], &entries[posIdx + 1], entriesCount);
        }

        new (&entries[posIdx]) T(std::forward<TArgs>(aArgs)...);
        size = newSize;
    }

    bool Remove(const T& aItem)
    {
        for (uint32_t i = 0; i != size; ++i)
        {
            if (aItem == entries[i])
            {
                return RemoveAt(i);
            }
        }

        return false;
    }

    bool RemoveAt(SizeType aIndex)
    {
        if (aIndex >= size)
            return false;

        entries[aIndex].~T();
        if ((aIndex + 1) != size) // If not at the end
        {
            SizeType entriesCount = size - (aIndex + 1);
            MoveEntries(&entries[aIndex + 1], &entries[aIndex], entriesCount);
        }
        --size;
        return true;
    }

    void Clear()
    {
        for (SizeType i = 0; i != size; ++i)
        {
            entries[i].~T();
        }

        size = 0;
    }

    void Reserve(SizeType aCount)
    {
        // Alignment seems to always be 8.
        constexpr uint32_t alignment = 8;

        auto newCapacity = CalculateGrowth(aCount);
        using func_t = void (*)(DynArray * aThis, uint32_t aCapacity, uint32_t aElementSize, uint32_t aAlignment,
                                void (*a5)(int64_t, int64_t, int64_t, int64_t));

        RelocFunc<func_t> func(Addresses::DynArray_Realloc);
        func(this, newCapacity, sizeof(T), alignment, nullptr);
    }

    Memory::IAllocator* GetAllocator()
    {
        if (capacity == 0)
            return reinterpret_cast<Memory::IAllocator*>(&entries);
        else
            return reinterpret_cast<Memory::IAllocator*>(&entries[capacity]);
    }

    [[nodiscard]] constexpr Iterator Begin() noexcept
    {
        return Iterator(entries);
    }

    [[nodiscard]] constexpr ConstIterator Begin() const noexcept
    {
        return ConstIterator(entries);
    }

    [[nodiscard]] constexpr Iterator End() noexcept
    {
        return Iterator(entries + size);
    }

    [[nodiscard]] constexpr ConstIterator End() const noexcept
    {
        return ConstIterator(entries + size);
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

    T* entries;        // 00
    uint32_t capacity; // 08
    uint32_t size;     // 0C

private:
    void MoveEntries(T* aSrc, T* aDst, SizeType aCount)
    {
        if (aCount == 0 || aSrc == aDst)
            return;

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memmove(aDst, aSrc, aCount * sizeof(T));
        }
        else if (aSrc < aDst)
        {
            for (; aCount != 0; --aCount)
            {
                new (&aDst[aCount - 1]) T(std::move(aSrc[aCount - 1]));
                aSrc[aCount - 1].~T();
            }
        }
        else
        {
            for (uint32_t i = 0; i != aCount; ++i)
            {
                new (&aDst[i]) T(std::move(aSrc[i]));
                aSrc[i].~T();
            }
        }
    }

    SizeType CalculateGrowth(SizeType aNewSize)
    {
        SizeType growth = capacity + (capacity / 2);
        if (growth < aNewSize)
        {
            return aNewSize;
        }

        return growth;
    }
};
RED4EXT_ASSERT_SIZE(DynArray<void*>, 0x10);
RED4EXT_ASSERT_OFFSET(DynArray<void*>, entries, 0x0);
RED4EXT_ASSERT_OFFSET(DynArray<void*>, capacity, 0x8);
RED4EXT_ASSERT_OFFSET(DynArray<void*>, size, 0xC);
} // namespace RED4ext
