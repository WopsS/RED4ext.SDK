#pragma once

// This file is generated from the Game's Reflection data

#include <cstdint>
#include <RED4ext/Common.hpp>
#include <RED4ext/REDhash.hpp>
#include <RED4ext/Types/generated/CResource.hpp>
#include <RED4ext/Types/generated/ink/LayerDefinitionCollection.hpp>
#include <RED4ext/Types/generated/ink/PermanentLayerDefinitionCollection.hpp>

namespace RED4ext
{
namespace ink { 
struct LayersResource : CResource
{
    static constexpr const char* NAME = "inkLayersResource";
    static constexpr const char* ALIAS = NAME;

    ink::LayerDefinitionCollection layerDefinitions; // 40
    ink::LayerDefinitionCollection preGameLayerDefinitions; // 3B0
    ink::PermanentLayerDefinitionCollection permanentLayerDefinitions; // 720
};
RED4EXT_ASSERT_SIZE(LayersResource, 0x800);
} // namespace ink
} // namespace RED4ext
