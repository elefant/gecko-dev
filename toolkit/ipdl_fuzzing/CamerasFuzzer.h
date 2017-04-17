/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et ft=cpp : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_CamerasFuzzer_h
#define mozilla_CamerasFuzzer_h

//#include "mozilla/Move.h"
//#include "mozilla/Pair.h"
//#include "mozilla/dom/ContentChild.h"
#include "mozilla/camera/PCamerasChild.h"
//#include "mozilla/Mutex.h"
//#include "base/singleton.h"
#include "FuzzerBase.h"

// conflicts with #include of scoped_ptr.h
//#undef FF
//#include "webrtc/common.h"
//#include "webrtc/video_renderer.h"
//#include "webrtc/modules/video_capture/video_capture_defines.h"

namespace mozilla {
namespace camera {

class CamerasFuzzer final : public PCamerasChild
                          , public FuzzerBase
{
public:
  CamerasFuzzer() = default;

private:
  virtual ~CamerasFuzzer() {};

  virtual nsresult CreateParentActor() override;
  virtual bool SendOneIPCMessage() override;

  // IPC messages recevied, received on the PBackground thread
  // these are the actual callbacks with data
  virtual mozilla::ipc::IPCResult RecvDeliverFrame(const CaptureEngine&, const int&,
                                                   mozilla::ipc::Shmem&&,
                                                   const VideoFrameProperties & prop) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvFrameSizeChange(const CaptureEngine&, const int&,
                                                      const int& w, const int& h) override { return IPC_OK(); }

  virtual mozilla::ipc::IPCResult RecvDeviceChange() override { return IPC_OK(); }

  // these are response messages to our outgoing requests
  virtual mozilla::ipc::IPCResult RecvReplyNumberOfCaptureDevices(const int&) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplyNumberOfCapabilities(const int&) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplyAllocateCaptureDevice(const int&) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplyGetCaptureCapability(const VideoCaptureCapability& capability) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplyGetCaptureDevice(const nsCString& device_name,
                                                            const nsCString& device_id,
                                                            const bool& scary) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplyFailure(void) override { return IPC_OK(); }
  virtual mozilla::ipc::IPCResult RecvReplySuccess(void) override { return IPC_OK(); }

  virtual void ActorDestroy(ActorDestroyReason aWhy) override
  {
    mIPCIsAlive = false;
  }

  uint32_t mCallIndex = 0;
  const uint32_t kParentMessageNum = 11;
};

} // namespace camera
} // namespace mozilla

#endif  // mozilla_CamerasFuzzer_h
