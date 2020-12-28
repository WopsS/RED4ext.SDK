#pragma once

#include <RED4ext/CName.hpp>
#include <RED4ext/CString.hpp>
#include <RED4ext/DynArray.hpp>
#include <RED4ext/IMemoryAllocator.hpp>
#include <RED4ext/Scripting/Functions.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>
#include <RED4ext/Unks.hpp>

namespace RED4ext
{
enum class ERTTIType : uint8_t
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
    virtual uint32_t GetSize() = 0;
    virtual uint32_t GetAlignment() = 0;
    virtual ERTTIType GetType() = 0;
    virtual void GetTypeName(CString& aOut) = 0;
    virtual void GetName2(CName& aOut) = 0;
    virtual void Init(void* aMemory) = 0;
    virtual void Destroy(void* aMemory) = 0;
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
    virtual IMemoryAllocator* GetAllocator() = 0;
};

RED4EXT_ASSERT_SIZE(IRTTIType, 0x8);

struct CRTTIType : IRTTIType
{
    int64_t unk8;
};

RED4EXT_ASSERT_SIZE(CRTTIType, 0x10);

struct BoolType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(BoolType, 0x10);

struct Int8Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Int8Type, 0x10);

struct Uint8Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Uint8Type, 0x10);

struct Int16Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Int16Type, 0x10);

struct Uint16Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Uint16Type, 0x10);

struct Int32Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Int32Type, 0x10);

struct Uint32Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Uint32Type, 0x10);

struct Int64Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Int32Type, 0x10);

struct Uint64Type : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(Uint64Type, 0x10);

struct FloatType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(FloatType, 0x10);

struct DoubleType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(DoubleType, 0x10);

struct StringType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(StringType, 0x10);

struct CNameType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(CNameType, 0x10);

struct CDateTimeType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(CDateTimeType, 0x10);

struct CGUIDType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(CGUIDType, 0x10);

struct CRUIDType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(CRUIDType, 0x10);

struct CRUIDRefType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(CRUIDRefType, 0x10);

struct TweakDBIDType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(TweakDBIDType, 0x10);

struct EditorObjectIDType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(EditorObjectIDType, 0x10);

struct VariantType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(VariantType, 0x10);

struct CClass : CRTTIType
{
    virtual void sub_C0() = 0;
    virtual void sub_C8() = 0;
    virtual void sub_D0() = 0;
    virtual void InitCls(IScriptable* aMemory) = 0;
    virtual void DestroyCls(IScriptable* aMemory) = 0;
    virtual void sub_E8() = 0;

    IScriptable* AllocInstance();

    bool IsA(IRTTIType* aType);
    CClassFunction* GetFunction(CName aName);

    CClass* parent;
    CName name;
    CName name2;
    DynArray<CProperty*> props;
    DynArray<CProperty*> overriddenProps;
    DynArray<CBaseFunction*> funcs;
    DynArray<CBaseFunction*> staticFuncs;
    uint32_t size; // The size of the real class that can be constructed.
    int32_t unk6C;
    uint32_t flags;
    uint32_t alignment;
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
    DynArray<void*> unk118;
    DynArray<void*> unk128;
    DynArray<void*> unk138;
    DynArray<void*> unk148;
    DynArray<void*> unk158;
    DynArray<void*> unk168;
    int64_t unk178;
    DynArray<void*> unk180;
    int8_t unk190[256];
    int16_t unk290;
    int32_t unk294;
    int8_t unk298;
    int8_t unk299;
};

RED4EXT_ASSERT_SIZE(CClass, 0x2A0);
RED4EXT_ASSERT_OFFSET(CClass, parent, 0x10);
RED4EXT_ASSERT_OFFSET(CClass, name, 0x18);
RED4EXT_ASSERT_OFFSET(CClass, funcs, 0x48);
RED4EXT_ASSERT_OFFSET(CClass, staticFuncs, 0x58);

struct DataBufferType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(DataBufferType, 0x10);

struct SharedDataBufferType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(SharedDataBufferType, 0x10);

struct serializationDeferredDataBufferType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(serializationDeferredDataBufferType, 0x10);

struct gamedataLocKeyWrapperType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(gamedataLocKeyWrapperType, 0x10);

struct LocalizationStringType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(LocalizationStringType, 0x10);

struct MessageResourcePathType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(MessageResourcePathType, 0x10);

struct NodeRefType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(NodeRefType, 0x10);

struct RuntimeEntityRefType : CRTTIType
{
};

RED4EXT_ASSERT_SIZE(RuntimeEntityRefType, 0x10);
} // namespace RED4ext

#ifdef RED4EXT_HEADER_ONLY
#include <RED4ext/RTTITypes-impl.hpp>
#endif
