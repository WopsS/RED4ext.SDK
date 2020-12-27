#pragma once

#include <cstdint>

#include <RED4ext/CName.hpp>
#include <RED4ext/Common.hpp>
#include <RED4ext/DynamicBuffer.hpp>

namespace RED4ext
{
struct CClass;

struct IFunction
{
    virtual void sub_0() = 0;
    virtual ~IFunction() = 0;

    virtual CClass* GetParent() = 0;
    virtual uint32_t GetFuncIndex() = 0;
    virtual void sub_20() = 0;
};

RED4EXT_ASSERT_SIZE(IFunction, 0x8);

struct CBaseFunction : IFunction
{
    CName name;
    CName name2;
    int64_t unk18;
    int64_t unk20;
    DynamicBuffer<void*> unk28;
    DynamicBuffer<void*> unk38;
    int8_t unk48[0x30];
    uint32_t flags;
    uint32_t unk7C;
};

RED4EXT_ASSERT_SIZE(CBaseFunction, 0x80);
RED4EXT_ASSERT_OFFSET(CBaseFunction, name, 0x8);
RED4EXT_ASSERT_OFFSET(CBaseFunction, flags, 0x78);

struct CGlobalFunction : CBaseFunction
{
    uint32_t index; // The registration index.
};

RED4EXT_ASSERT_SIZE(CGlobalFunction, 0x88);
RED4EXT_ASSERT_OFFSET(CGlobalFunction, index, 0x80);

struct CClassFunction : CBaseFunction
{
    CClass* parent;
    uint32_t index; // The registration index.
};

RED4EXT_ASSERT_SIZE(CClassFunction, 0x90);
RED4EXT_ASSERT_OFFSET(CClassFunction, parent, 0x80);
RED4EXT_ASSERT_OFFSET(CClassFunction, index, 0x88);

struct CScriptedFunction : CBaseFunction
{
    CClass* parent;
};

RED4EXT_ASSERT_SIZE(CScriptedFunction, 0x88);
RED4EXT_ASSERT_OFFSET(CScriptedFunction, parent, 0x80);
} // namespace RED4ext
