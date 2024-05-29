#pragma once

// clang-format off

// This file is generated from the Game's Reflection data

#include <RED4ext/Common.hpp>
#include <RED4ext/Handle.hpp>
#include <RED4ext/NativeTypes.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/AnimDatabaseCollection.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IComponent.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/IKTargetRef.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/IKTargetParams_Update.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IOrientationProvider.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IPositionProvider.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/LookAtRef.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/LookAtParams_UpdatePositions.hpp>
#include <cstdint>

namespace RED4ext
{
namespace anim { struct ActionAnimDatabase; }
namespace ent
{
struct AnimationControlBinding;
struct AnimationControllerComponent;

struct IKTargetController
{
    struct IKTargetData
    {
        anim::IKTargetRef ikTargetref;                    // 00
        Handle<IPositionProvider> positionProvider;       // 10
        Handle<IOrientationProvider> orientationProvider; // 20
    };
    AnimationControllerComponent& animationControllerComponent; // 00
    uint64_t unk08;                                             // 08
    DynArray<IKTargetData> IKTargetData;                        // 10
    Handle<anim::IKTargetParams_Update> IKparams;               // 20
};
RED4EXT_ASSERT_SIZE(IKTargetController, 0x30);

struct LookAtController
{
    struct LookAtData
    {
        anim::LookAtRef lookAtRef;                  // 00
        Handle<IPositionProvider> positionProvider; // 10
        uint32_t unk20;                             // 20
    };
    struct AdditionalLookatdata
    {
        CName partName;                       // 00                    
        DynArray<anim::LookAtRef> lookAtRefs; // 08
    };
    AnimationControllerComponent& animationControllerComponent; // 00
    uint32_t unkCounter;                                        // 08
    void* unk10;                                                // 10
    bool usingTPPCamera;                                        // 18
    DynArray<LookAtData> lookAtData;                            // 20
    DynArray<AdditionalLookatdata> additionalLookatdata;        // 30
    Handle<anim::LookAtParams_UpdatePositions> lookAtparams;    // 40
    uint64_t unk50[5];                                          // 50
    float unk78;                                                // 78
    uint32_t unk7C;                                             // 7C
    uint64_t unk80;                                             // 80
};
RED4EXT_ASSERT_SIZE(LookAtController, 0x88);

struct AnimationControllerComponent : ent::IComponent
{
    static constexpr const char* NAME = "entAnimationControllerComponent";
    static constexpr const char* ALIAS = "AnimationControllerComponent";

    uint64_t unk90[7];                                   // 90 
    void* unkC8;                                         // C8
    void* unkD0;                                         // D0
    DynArray<uint64_t> unkD8Array;                       // D8
    DynArray<uint64_t> unkE8Array;                       // E8
    Ref<anim::ActionAnimDatabase> actionAnimDatabaseRef; // F8
    anim::AnimDatabaseCollection animDatabaseCollection; // 110
    LookAtController lookAtController;                   // 120
    IKTargetController ikTargetController;               // 1A0
    bool unk1D8;                                         // 1D8
    Handle<ent::AnimationControlBinding> controlBinding; // 1E0
};
RED4EXT_ASSERT_SIZE(AnimationControllerComponent, 0x1F0);

} // namespace ent
using entAnimationControllerComponent = ent::AnimationControllerComponent;
using AnimationControllerComponent = ent::AnimationControllerComponent;
} // namespace RED4ext

// clang-format on
