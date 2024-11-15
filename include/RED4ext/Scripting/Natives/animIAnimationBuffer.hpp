#pragma once

#include <RED4ext/ISerializable.hpp>
#include <RED4ext/Scripting/Natives/Generated/anim/AnimDataAddress.hpp>
#include <cstdint>

namespace RED4ext
{
namespace anim
{
struct IAnimationBuffer : ISerializable
{
    static constexpr const char* NAME = "animIAnimationBuffer";
    static constexpr const char* ALIAS = NAME;

    virtual bool Initialize() = 0;
    virtual bool Load() const {}
    virtual bool Unload() const {}
    virtual bool IsReady() const { return true; }
    virtual bool HasInplaceCompressedBuffer() const { return true; }
    virtual bool sub_100() = 0;
    virtual float GetDuration() = 0;
    virtual float GetFPS() = 0;
    virtual AnimDataAddress* GetDataAddress() const { return {}; }
    virtual void GetSize() const {}
    virtual void GetNumExtraJoints() = 0;
    virtual void GetExtraData() const {}
    virtual void sub_138() const {}
    virtual bool HasFallbackFrames() const {}
    virtual void sub_140() const {}
    virtual void sub_148() const {}
    virtual void CreateFallbackFrame() const {}
    virtual void GatherInfo() const {}
    virtual bool sub_168() const {}
    virtual void sub_170() const {}
    virtual bool sub_178() const {}
    virtual void sub_180() const {}

};
RED4EXT_ASSERT_SIZE(IAnimationBuffer, 0x30);
} // namespace anim
using animIAnimationBuffer = anim::IAnimationBuffer;
} // namespace RED4ext
