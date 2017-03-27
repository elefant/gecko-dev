/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et ft=cpp : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CamerasFuzzer.h"

#include "mozilla/Assertions.h"
#include "mozilla/ipc/BackgroundChild.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "mozilla/Logging.h"
#include "mozilla/SyncRunnable.h"
#include "mozilla/WeakPtr.h"
#include "mozilla/Unused.h"
#include "nsThreadUtils.h"

#include "mozilla/fuzzing/FuzzTraitsPCameras.h"

#undef LOG
#define LOG(args) printf_stderr(">>>>>> CamerasFuzzer <<<<<<< %s\n", (nsPrintfCString args).get())

#include "mozilla/fuzzing/FuzzyCall.h"

namespace mozilla {
namespace camera {

NS_IMPL_ISUPPORTS(CamerasFuzzer, nsITimerCallback)

static nsresult
CreatePCamerasParentActor(PCamerasChild* aPCamerasChild)
{
  auto existingBackgroundChild = ipc::BackgroundChild::GetForCurrentThread();
  if (!existingBackgroundChild) {
    existingBackgroundChild =
      ipc::BackgroundChild::SynchronouslyCreateForCurrentThread();
  }
  if (!existingBackgroundChild) {
    LOG(("Failed to get existing PBackgroundChild"));
    return NS_ERROR_FAILURE;
  }

  if(!existingBackgroundChild->SendPCamerasConstructor(aPCamerasChild)) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

CamerasFuzzer::CamerasFuzzer()
{
  mTimer = do_CreateInstance("@mozilla.org/timer;1");
}

nsresult
CamerasFuzzer::Start()
{
  // We don't mind blocking the main thread since we are fuzzing :)
  nsresult rv = CreatePCamerasParentActor(this);
  if (NS_FAILED(rv)) {
    LOG(("Failed to create PCameras parent side actor."));
    return rv;
  }

  rv = mTimer->InitWithCallback(this, 100, nsITimer::TYPE_REPEATING_SLACK);
  if (NS_FAILED(rv)) {
    LOG(("Failed to init timer for fuzzing."));
  }

  return rv;
}

///////////////////////////////////////////////////////////////////////////////
// nsITimerCallback implementation
NS_IMETHODIMP
CamerasFuzzer::Notify(nsITimer *timer)
{
  bool callRet = true;
  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  case 0:  callRet = FUZZY_CALL1(NumberOfCaptureDevices, CaptureEngine); break;
  case 1:  callRet = FUZZY_CALL2(NumberOfCapabilities, CaptureEngine, nsCString); break;
  case 2:  callRet = FUZZY_CALL3(GetCaptureCapability, CaptureEngine, nsCString, int); break;
  case 3:  callRet = FUZZY_CALL2(GetCaptureDevice, CaptureEngine, int); break;
  case 4:  callRet = FUZZY_CALL3(AllocateCaptureDevice, CaptureEngine, nsCString, PrincipalInfo); break;
  case 5:  callRet = FUZZY_CALL2(ReleaseCaptureDevice, CaptureEngine, int); break;
  case 6:  callRet = FUZZY_CALL3(StartCapture, CaptureEngine, int, VideoCaptureCapability); break;
  case 7:  callRet = FUZZY_CALL2(StopCapture, CaptureEngine, int); break;
  //case 8:  callRet = FUZZY_CALL1(ReleaseFrame, Shmem); break;
  case 9:  callRet = FUZZY_CALL0(AllDone); break;
  case 10: callRet = FUZZY_CALL1(EnsureInitialized, CaptureEngine); break;
  default: LOG(("Unknown call index: %d", callIndex)); break;
  }

  if (!callRet) {
    LOG(("Failed to call %d", callIndex));
    timer->Cancel();
  }

  return NS_OK;
}

}
}
