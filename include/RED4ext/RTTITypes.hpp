#pragma once

#include <cstdint>

#include <RED4ext/CName.hpp>
#include <RED4ext/CString.hpp>
#include <RED4ext/Common.hpp>
#include <RED4ext/DynamicBuffer.hpp>
#include <RED4ext/Scripting/Functions.hpp>
#include <RED4ext/Unks.hpp>

namespace RED4ext
{
enum class ERTTITypeType : uint8_t
{
    Name = 0,
    Fundamental = 1,
    Class = 2,
    Array = 3,
    Simple = 4,
    Enum = 5,
    StaticArray = 6,
    NativeArray = 7,
    Pointer = 8,
    Handle = 9,
    WeakHandle = 10,
    ResourceReference = 11,
    ResourceAsyncReference = 12,
    BitField = 13,
    LegacySingleChannelCurve = 14,
    ScriptReference = 15,
};

struct IRTTIType
{
    virtual ~IRTTIType() = 0;
    virtual void GetName(CName& aOut) = 0;
    virtual void sub_10() = 0;
    virtual void sub_18() = 0;
    virtual ERTTITypeType GetType() = 0;
    virtual void GetTypeName(CString& aOut) = 0;
    virtual void sub_30() = 0;
    virtual void sub_38() = 0;
    virtual void sub_40() = 0;
    virtual void sub_48() = 0;
    virtual void sub_50() = 0;
    virtual void sub_58() = 0;
    virtual void sub_60() = 0;
    virtual void sub_68() = 0;
    virtual void sub_70() = 0;
    virtual void sub_78() = 0;
    virtual void sub_80() = 0;
    virtual void sub_88() = 0;
    virtual void sub_90() = 0;
    virtual void sub_98() = 0;
    virtual void sub_A0() = 0;
    virtual void sub_A8() = 0;
    virtual void sub_B0() = 0;
    virtual void sub_B8() = 0;
};

RED4EXT_ASSERT_SIZE(IRTTIType, 0x8);

struct CRTTIType : IRTTIType
{
    int64_t unk8;
};

RED4EXT_ASSERT_SIZE(CRTTIType, 0x10);

struct CClass : CRTTIType
{
    bool IsA(IRTTIType* aType);
    CClassFunction* GetFunction(CName aName);

    CClass* parent;
    CName name;
    int64_t unk20;
    DynamicBuffer<void*> unk28;
    DynamicBuffer<void*> unk38;
    DynamicBuffer<void*> funcs48;
    DynamicBuffer<void*> funcs58;
    int32_t unk68;
    int32_t unk6C;
    int32_t unk70;
    int32_t unk74;
    int64_t unk78;
    int64_t unk80;
    int64_t unk88;
    int64_t unk90;
    int32_t unk98;
    int32_t unk9C;
    Unk425 unkA0;
    Unk425 unkD0;
    int64_t unk100;
    int32_t unk108;
    int32_t unk10C;
    int64_t unk110;
    DynamicBuffer<void*> unk118;
    DynamicBuffer<void*> unk128;
    DynamicBuffer<void*> unk138;
    DynamicBuffer<void*> unk148;
    DynamicBuffer<void*> unk158;
    DynamicBuffer<void*> unk168;
    int64_t unk178;
    DynamicBuffer<void*> unk180;
    int8_t unk190[256];
    int16_t unk290;
    int32_t unk294;
    int8_t unk298;
    int8_t unk299;
};

RED4EXT_ASSERT_SIZE(CClass, 0x2A0);
RED4EXT_ASSERT_OFFSET(CClass, parent, 0x10);
RED4EXT_ASSERT_OFFSET(CClass, name, 0x18);
RED4EXT_ASSERT_OFFSET(CClass, funcs48, 0x48);
RED4EXT_ASSERT_OFFSET(CClass, funcs58, 0x58);
} // namespace RED4ext

#ifdef RED4EXT_HEADER_ONLY
#include <RED4ext/RTTITypes-impl.hpp>
#endif
