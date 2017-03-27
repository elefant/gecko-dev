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
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::camera::VideoFrameProperties>
{
  using ParamType = mozilla::camera::VideoFrameProperties;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::camera::CaptureEngine>
{
  using ParamType = mozilla::camera::CaptureEngine;
  static ParamType Fuzz();
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

auto
FuzzTraits<mozilla::camera::VideoCaptureCapability>::Fuzz() -> ParamType
{
  return mozilla::camera::VideoCaptureCapability();
}

auto
FuzzTraits<mozilla::camera::VideoFrameProperties>::Fuzz() -> ParamType
{
  return mozilla::camera::VideoFrameProperties();
}

auto
FuzzTraits<mozilla::camera::CaptureEngine>::Fuzz() -> ParamType
{
  return mozilla::camera::CaptureEngine();
}

/*
auto
FuzzTraits<Shmem>::Fuzz() -> ParamType
{
  return NS_OK;
}
*/

