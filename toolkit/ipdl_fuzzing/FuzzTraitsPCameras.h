#include "mozilla/fuzzing/FuzzTraitsPrimitives.h"
#include "mozilla/fuzzing/FuzzTraitsCommon.h"
#include "mozilla/media/CamerasTypes.h"
#include "mozilla/camera/PCamerasChild.h"

using namespace mozilla::dom;
using namespace mozilla::ipc;

template<>
struct FuzzTraits<mozilla::camera::VideoCaptureCapability>
{
  using ParamType = mozilla::camera::VideoCaptureCapability;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::camera::VideoFrameProperties>
{
  using ParamType = mozilla::camera::VideoFrameProperties;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::camera::CaptureEngine>
{
  using ParamType = mozilla::camera::CaptureEngine;
  inline static ParamType Fuzz();
};

/*
template<>
struct FuzzTraits<Shmem>
{
  using ParamType = Shmem;
  static ParamType Fuzz();
};
*/

//////////////////////////////////////////////////////////////////
// Implementations.

#define AUTO_FUZZ_ATTRIBUTE(attrName) AutoTypeFuzz(obj.attrName())

auto
FuzzTraits<mozilla::camera::VideoCaptureCapability>::Fuzz() -> ParamType
{
  auto obj = mozilla::camera::VideoCaptureCapability();

  AUTO_FUZZ_ATTRIBUTE(width);
  AUTO_FUZZ_ATTRIBUTE(height);
  AUTO_FUZZ_ATTRIBUTE(maxFPS);
  AUTO_FUZZ_ATTRIBUTE(expectedCaptureDelay);
  AUTO_FUZZ_ATTRIBUTE(rawType);
  AUTO_FUZZ_ATTRIBUTE(codecType);
  AUTO_FUZZ_ATTRIBUTE(interlaced);

  return obj;
}

auto
FuzzTraits<mozilla::camera::VideoFrameProperties>::Fuzz() -> ParamType
{
  auto obj = mozilla::camera::VideoFrameProperties();

  AUTO_FUZZ_ATTRIBUTE(bufferSize);
  AUTO_FUZZ_ATTRIBUTE(timeStamp);
  AUTO_FUZZ_ATTRIBUTE(ntpTimeMs);
  AUTO_FUZZ_ATTRIBUTE(renderTimeMs);
  AUTO_FUZZ_ATTRIBUTE(rotation);
  AUTO_FUZZ_ATTRIBUTE(yAllocatedSize);
  AUTO_FUZZ_ATTRIBUTE(uAllocatedSize);
  AUTO_FUZZ_ATTRIBUTE(vAllocatedSize);
  AUTO_FUZZ_ATTRIBUTE(width);
  AUTO_FUZZ_ATTRIBUTE(height);
  AUTO_FUZZ_ATTRIBUTE(yStride);
  AUTO_FUZZ_ATTRIBUTE(uStride);
  AUTO_FUZZ_ATTRIBUTE(vStride);

  return obj;
}

auto
FuzzTraits<mozilla::camera::CaptureEngine>::Fuzz() -> ParamType
{
  return RandomPick<ParamType>({
    mozilla::camera::ScreenEngine,
    mozilla::camera::BrowserEngine,
    mozilla::camera::WinEngine,
    mozilla::camera::AppEngine,
    mozilla::camera::CameraEngine,
  });
}

/*
auto
FuzzTraits<Shmem>::Fuzz() -> ParamType
{
  return NS_OK;
}
*/

