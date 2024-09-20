#include <iostream>

#include <deque>
#include <map>
#include <set>
#include <stack>

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/Vector4.hpp>
#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>

struct SomePool : RED4ext::Memory::PoolInfo
{
};

template<typename TRetType, typename... Args>
struct TestFuncType
{
    using type = void (*)(RED4ext::Memory::Vault*, TRetType&, Args...);
};

template<typename... Args>
struct TestFuncType<void, Args...>
{
    using type = void (*)(RED4ext::Memory::Vault*, Args...);
};

template<typename TResult, typename... Args>
auto InvokeNative(std::uintptr_t aAddress, Args&&... aArgs)
{
    const auto pool = RED4ext::Memory::PoolAI_Attitudes::Get();
    auto storage = reinterpret_cast<RED4ext::Memory::Vault*>(pool->storage->allocatorStorage & -8);

    using func_t = void (*)(RED4ext::Memory::Vault*, Args...);
    RED4ext::RelocFunc<func_t> func(aAddress);

    func(storage, std::forward<Args>(aArgs)...);
}

/*
 * To run this plugin you need to load it with RED4ext (https://github.com/WopsS/RED4ext).
 */

void SummonVehiclePlease(RED4ext::IScriptable* aContext, RED4ext::CStackFrame* aFrame, void* aOut, int64_t a4)
{
    RED4EXT_UNUSED_PARAMETER(aContext);
    RED4EXT_UNUSED_PARAMETER(aOut);
    RED4EXT_UNUSED_PARAMETER(a4);

    aFrame->code++; // skip ParamEnd

    // Retrieve global game instance
    RED4ext::ScriptGameInstance gameInstance;
    RED4ext::Handle<RED4ext::IScriptable> handle;
    // using test = decltype(&RED4ext::IScriptable::GetAllocator);
    // auto testasd = std::bind(&RED4ext::IScriptable::GetAllocator, aContext);
    // testasd();

    //// Get the player from the game instance
    // RED4ext::ExecuteGlobalFunction("GetPlayer;GameInstance", &handle, gameInstance);
    // std::vector<int> a;
    // std::allocator<int> a;
    // a.begin();
    // a.cbegin();
    // a.end();
    // a.get_allocator();
    // a.empty();
    // a.at(10);
    // a.reserve(123);
    // constexpr auto asd = a.max_size();

    // std::map<int, int> map;
    // map.begin();

    // std::_Compressed_pair<int[], RED4ext::Memory::DefaultAllocator> _Compressed_pair(
    //     std::_One_then_variadic_args_t{}, 0, RED4ext::Memory::DefaultAllocator::Get());
    //_Compressed_pair._Myval1;
    // auto sizeof_Compressed_pair = sizeof(_Compressed_pair);

    // int a[1];

    // std::unordered_map<int, int> unordered_map;
    // unordered_map.begin();

    // std::array<int, 10> arraya;

    //// RED4ext::Memory::RTTIAllocator allocator;
    //// constexpr RED4ext::DynArray<int, RED4ext::Memory::RTTIAllocator> DynArrayRTTIFunctionAllocatorConstexpr;
    //// RED4ext::DynArray<int, RED4ext::Memory::RTTIAllocator> DynArray(allocator);
    //// auto GetAllocator = DynArray.GetAllocator();
    //// sizeof(RED4ext::Memory::IAllocator);
    //// for (const auto asd : a)
    ////{
    //// }

    //{
    //    std::map<int, char*> map;
    //    const auto a = map.begin();
    //    auto b = *a;
    //}

    //{
    //    std::istream_iterator<char> a;
    //}

    //{
    //    std::pair<int, int> p;
    //}

    //{
    //    std::array<int, 4> a;
    //}

    //{
    //    RED4ext::HashMap<int*, const char*> map;
    //    map.Emplace((int*)10, "asd");
    //    map.Reserve(123);

    //    auto begin = map.begin();
    //    auto a = begin->second;

    //    for (auto a : map)
    //    {
    //        std::cout << "a";
    //    }

    //    auto asd = map.begin();
    //}

    //{
    //    RED4ext::Containers::DynArray<int, RED4ext::Memory::RTTIAllocator> DynArrayDefaultAlloc;
    //    constexpr auto max_size = DynArrayDefaultAlloc.MaxSize();
    //}

    //{
    //    constexpr RED4ext::Allocator<int, RED4ext::Memory::PoolAI> alloc;
    //    auto a = alloc.Allocate(12);
    //}

    // constexpr auto b = sizeof(a);
    // std::vector<int>::difference_type;
    if (handle)
    {
        auto rtti = RED4ext::CRTTISystem::Get();

        std::invoke(&RED4ext::CRTTISystem::GetClass, rtti, RED4ext::CName("test"));
        auto greet = std::mem_fn(&RED4ext::CRTTISystem::GetClass);
        // Get the PlayerPuppet class
        auto playerPuppetCls = rtti->GetClass("PlayerPuppet");
        // Set up the GetQuickSlotsManager function on the PlayerPuppet class
        auto getQuickSlotsManagerFunc = playerPuppetCls->GetFunction("GetQuickSlotsManager");

        RED4ext::Handle<RED4ext::IScriptable> quickSlotManager;
        // Execute the function on our Player instance
        RED4ext::ExecuteFunction(handle, getQuickSlotsManagerFunc, &quickSlotManager, {});

        auto quickSlotsManagerCls = rtti->GetClass("QuickSlotsManager ");
        auto summonVehicleFunc = quickSlotsManagerCls->GetFunction("SummonVehicle");

        RED4ext::ExecuteFunction(quickSlotManager, summonVehicleFunc, nullptr, {});
    }
    {
        RED4ext::Containers::DynArray<int, RED4ext::Allocator<int, RED4ext::Memory::PoolRTTIFunction>>
            DynArrayDefaultAlloc;
        constexpr auto max_size = DynArrayDefaultAlloc.MaxSize();

        //std::allocator_traits<RED4ext::Allocator<int>>::pointer a;
    }

    {
        RED4ext::Allocator<int, RED4ext::Memory::PoolAI> allocator;
        auto allocate = allocator.Allocate(12);
        auto a = allocator.Reallocate(allocate, 1234);
        allocator.Deallocate(a);

        auto allocateAtLeast = allocator.AllocateAtLeast(255);
        allocator.Deallocate(allocateAtLeast);

        std::size_t size = 123;
        InvokeNative<int*>(0, size, 1, 2);
    }

    {
        std::allocator<int> allocator;
        auto a = allocator.allocate(12);
    }
}

void CustomFunctionTwo(RED4ext::IScriptable* aContext, RED4ext::CStackFrame* aFrame, void* aOut, int64_t a4)
{
    RED4EXT_UNUSED_PARAMETER(aContext);
    RED4EXT_UNUSED_PARAMETER(aOut);
    RED4EXT_UNUSED_PARAMETER(a4);

    float varOne;
    RED4ext::Vector4 varTwo;

    RED4ext::GetParameter(aFrame, &varOne);
    RED4ext::GetParameter(aFrame, &varTwo);

    aFrame->code++; // skip ParamEnd
}

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes()
{
    static RED4ext::TTypedClass<RED4ext::CName> cls("myCoolTest");

    auto rtti = RED4ext::CRTTISystem::Get();
    rtti->RegisterType(&cls);
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes()
{
    Sleep(5000);

    // RED4ext::Map<int, char*> map;
    // map.begin();

    /*{
        RED4ext::DynArray<RED4ext::CGlobalFunction*, RED4ext::Memory::RTTIAllocator> DynArrayDefaultAlloc;
        auto a = DynArrayDefaultAlloc.GetAllocator();
        a.Alloc(100);
    }

    RED4ext::Memory::RTTIAllocator allocator;
    auto test = allocator.Alloc(1000);
    std::cout << test.memory;

    RED4ext::DynArray<RED4ext::CGlobalFunction*, RED4ext::Memory::RTTIAllocator> DynArrayRTTIAllocator(&allocator);
    RED4ext::DynArray<RED4ext::CGlobalFunction*, RED4ext::Memory::RTTIAllocator> DynArrayRTTIFunctionAllocator(
        allocator);

    auto rtti = RED4ext::CRTTISystem::Get();
    RED4ext::CBaseFunction::Flags flags = {.isNative = true, .isStatic = true};

    {
        auto func =
            RED4ext::CGlobalFunction::Create("SummonVehiclePlease", "SummonVehiclePlease", &SummonVehiclePlease);
        func->flags = flags;
        rtti->RegisterFunction(func);

        DynArrayRTTIAllocator.EmplaceBack(func);
        DynArrayRTTIFunctionAllocator.EmplaceBack(func);
    }

    {
        auto func = RED4ext::CGlobalFunction::Create("CustomFunctionTwo", "CustomFunctionTwo", &CustomFunctionTwo);
        func->flags = flags;
        func->AddParam("Float", "var_one");
        func->AddParam("Vector4", "var_two");
        rtti->RegisterFunction(func);

        DynArrayRTTIAllocator.EmplaceBack(func);
        DynArrayRTTIFunctionAllocator.EmplaceBack(func);
    }

    for (const auto b : DynArrayRTTIAllocator)
    {
        std::cout << b->fullName.ToString();
    }

    for (const auto b : DynArrayRTTIFunctionAllocator)
    {
        std::cout << b->fullName.ToString();
    }*/
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle, RED4ext::EMainReason aReason,
                                        const RED4ext::Sdk* aSdk)
{
    RED4EXT_UNUSED_PARAMETER(aHandle);
    RED4EXT_UNUSED_PARAMETER(aSdk);

    switch (aReason)
    {
    case RED4ext::EMainReason::Load:
    {
        RED4ext::RTTIRegistrator::Add(RegisterTypes, PostRegisterTypes);
        break;
    }
    case RED4ext::EMainReason::Unload:
    {
        break;
    }
    }

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* aInfo)
{
    aInfo->name = L"RED4ext.NativeGlobalsRedscript";
    aInfo->author = L"Jack Humbert";
    aInfo->version = RED4EXT_SEMVER(1, 0, 0);
    aInfo->runtime = RED4EXT_RUNTIME_LATEST;
    aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_LATEST;
}
