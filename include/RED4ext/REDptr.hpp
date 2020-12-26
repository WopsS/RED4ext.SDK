#pragma once

#include <cstdint>
#include <Windows.h>

namespace RED4ext
{
template<typename T>
class REDptr
{
public:
    REDptr(uintptr_t aAddress)
        : m_address(reinterpret_cast<T*>(aAddress + reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr))))
    {
    }

    operator T() const
    {
        return *m_address;
    }

private:
    T* m_address;
};
} // namespace RED4ext
