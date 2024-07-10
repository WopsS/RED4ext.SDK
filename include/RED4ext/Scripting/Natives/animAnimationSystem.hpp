#pragma once

#include <RED4ext/Common.hpp>
#include <RED4ext/HashMap.hpp>
#include <RED4ext/NativeTypes.hpp>
#include <RED4ext/Scripting/Natives/AnimatedEntityData.hpp>
#include <RED4ext/Scripting/Natives/Generated/Box.hpp>
#include <RED4ext/Scripting/Natives/Generated/appearance/AppearanceResource.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/RagdollComponent.hpp>
#include <RED4ext/Scripting/Natives/Generated/world/IRuntimeSystem.hpp>
#include <RED4ext/Scripting/Natives/animRig.hpp>
#include <RED4ext/Scripting/Natives/entEntity.hpp>
#include <RED4ext/Scripting/Natives/metaRig.hpp>
#include <RED4ext/SystemUpdate.hpp>
#include <cstdint>

namespace RED4ext
{
namespace ent
{
    struct AnimatedComponent;
    struct EntityID;
    struct IPlacedComponent;
    struct ITransformAttachment;
}
namespace anim { struct AnimSetupResource; }

namespace world
{
struct AnimationSystemScriptInterface;
struct RuntimeSystemRendering;

struct AnimatedEntitiesBucket
{
    struct componentBindings
    {
        struct binding
        {
            Handle<ent::IPlacedComponent> placedComponent;         // 00 
            ent::AnimatedComponent* animComponent;                 // 10
            Handle<ent::ITransformAttachment> transformAttachment; // 18
        };
        RED4EXT_ASSERT_SIZE(binding, 0x28);

        DynArray<binding> bindings; // 00
        uint64_t unk10;             // 10
    };
    RED4EXT_ASSERT_SIZE(componentBindings, 0x18);

    static constexpr uint32_t MaxEntries = 2048;

    HashMap<ent::EntityID, uint32_t> entityIDs;                              // 00
    StaticArray<anim::AnimatedEntityData*, MaxEntries> animatedEntitiesData; // 30
    StaticArray<Box, MaxEntries> animationBounds;                            // 4090
    StaticArray<ent::AnimatedComponent*, MaxEntries> animatedComponents;     // 140A0
    StaticArray<Handle<ent::Entity>, MaxEntries> entities;                   // 180A8
    StaticArray<componentBindings, MaxEntries> componentBindings;            // 200B0

    struct unkStruct { uint32_t unk; };

    StaticArray<unkStruct, MaxEntries> unk2C0B8;                            // 2C0B8
    uint64_t unk2E0C0[0x2E0E0 - 0x2E0C0];                                   // 2E0C0
};
RED4EXT_ASSERT_SIZE(AnimatedEntitiesBucket, 0x2E170);

struct RagdollEntitiesBucket
{
    StaticArray<anim::Rig*, 2048> rigs;                          // A2948
    StaticArray<ent::RagdollComponent*, 2048> ragdollComponents; // A6950
};

struct __declspec(align(0x10)) AnimationSystem : world::IRuntimeSystem
{
    static constexpr const char* NAME = "worldAnimationSystem";
    static constexpr const char* ALIAS = NAME;

    static constexpr uint8_t BucketCount = static_cast<uint8_t>(UpdateBucketEnum::BucketCount);

    AnimatedEntitiesBucket entitityBuckets[BucketCount];                // 00
    uint8_t unk8A4D0[0x8A5A0 - 0x8A4A0];                                // 8A4D0
    HashMap<ent::EntityID, ent::AnimatedComponent*> animatedComponents; // 8A5A0
    uint8_t unk8A600[0xA2948 - 0x8A5D0];                                // 8A600
    RagdollEntitiesBucket ragdollEntities;                              // A2948
    uint8_t unkAA958[0xAAA00 - 0xAA958];                                // AA958
    uint64_t globalTimeMS;                                              // AAA00 - milliseconds                              
    double globalTimeS;                                                 // AAA08 - seconds
    uint8_t unkAAA10[0xB2AE8 - 0xAAA10];                                // AAA10
    RuntimeSystemRendering* renderingSystem;                            // B2AF0
    anim::MetaRigInfo metaRigInfo;                                      // B2AF8
    Handle<AnimationSystemScriptInterface> scriptInterface;             // B2B10
    uint8_t unkB2B28[0xB2B38 - 0xB2B20];                                // B2B28
    Handle<anim::AnimSetupResource> animSetupResource;                  // B2B38
    uint8_t unkB2B48[0xB2B90 - 0xB2B48];                                // B2B48
};
RED4EXT_ASSERT_SIZE(AnimationSystem, 0xB2B90);
} // namespace world
using worldAnimationSystem = world::AnimationSystem;
} // namespace RED4ext
