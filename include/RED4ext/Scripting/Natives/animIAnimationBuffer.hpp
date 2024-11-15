#pragma once

#include <cstdint>
#include <RED4ext/Scripting/Natives/Generated/anim/AnimDataAddress.hpp>
#include <RED4ext/ISerializable.hpp>

namespace RED4ext
{
namespace anim
{
struct IAnimationBuffer : ISerializable
{
    static constexpr const char* NAME = "animIAnimationBuffer";
    static constexpr const char* ALIAS = NAME;

    virtual bool Initialize() = 0;
    virtual bool Load();
    virtual bool Unload();
    virtual bool IsReady();
    virtual bool HasInplaceCompressedBuffer();
    virtual bool sub_100() = 0;
    virtual float GetDuration() = 0;
    virtual float GetFPS() = 0;
    virtual AnimDataAddress* GetDataAddress();
    virtual void GetSize();
    virtual void GetNumExtraJoints() = 0;
    virtual void GetExtraData();
    virtual void sub_138();
    virtual bool HasFallbackFrames();
    virtual void sub_140();
    virtual void sub_148();
    virtual void CreateFallbackFrame();
    virtual void GatherInfo();
    virtual bool sub_168();
    virtual void sub_170();
    virtual bool sub_178();
    virtual void sub_180();

};
RED4EXT_ASSERT_SIZE(IAnimationBuffer, 0x30);
} // namespace anim
using animIAnimationBuffer = anim::IAnimationBuffer;
} // namespace RED4ext
