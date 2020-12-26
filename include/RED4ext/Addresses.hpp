#pragma once

#include <cstdint>

// Addresses for Cyberpunk 2077 1.06
namespace RED4ext::Addresses
{
// 40 53 48 83 EC 20 65 48 8B 04 25 58 00 00 00 48 8D 1D ? ? ? ?, expected: 1, index: 0
constexpr uintptr_t CRTTISystem_Get = 0x140267450 - 0x140000000;

// 40 53 48 83 EC 20 33 C0 C6 01 00, expected: 4, index: 2
constexpr uintptr_t CString_ctor = 0x1401B6FC0 - 0x140000000;

// 40 53 48 83 EC 20 48 8B D9 8B 49 14 8B C1 C1 E8, expected: 1, index: 0
constexpr uintptr_t CString_dtor = 0x1401B70D0 - 0x140000000;

// 40 53 48 83 EC 20 48 8B D9 48 3B CA, expected: 8, index: 0
constexpr uintptr_t CString_copy = 0x1401B7230 - 0x140000000;

// 48 89 05 ? ? ? ? 49 8D 9D 88 00 00 00 49 8B 07 4C 8B C3, expected: 1, offset: found_address + 3
constexpr uintptr_t CGameEngine = 0x1440287E8 - 0x140000000;
} // namespace RED4ext::Addresses
