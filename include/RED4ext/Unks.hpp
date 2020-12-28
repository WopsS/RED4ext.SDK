#pragma once

#include <cstdint>
#include <RED4ext/Common.hpp>

namespace RED4ext
{
struct Unk425
{
    uint64_t unk0;
    int64_t unk8;
    int64_t unk10;
    int64_t unk18;
    int64_t unk20;
    int32_t unk28;
    int32_t unk2C;
};

RED4EXT_ASSERT_SIZE(Unk425, 0x30);

struct Unk530
{
    int64_t unk0;
    int64_t unk8;
};

RED4EXT_ASSERT_SIZE(Unk530, 0x10);
} // namespace RED4ext
