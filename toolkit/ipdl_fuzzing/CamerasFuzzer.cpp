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

nsresult
CamerasFuzzer::CreateParentActor()
{
  auto pbackgroundChild = EnsurePBackgroundChildForCurrentThread();
  if (!pbackgroundChild->SendPCamerasConstructor(this)) {
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

bool CamerasFuzzer::SendOneIPCMessage()
{
  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  case 0:  return FUZZY_CALL1(NumberOfCaptureDevices, CaptureEngine);
  case 1:  return FUZZY_CALL2(NumberOfCapabilities, CaptureEngine, nsCString);
  case 2:  return FUZZY_CALL3(GetCaptureCapability, CaptureEngine, nsCString, int);
  case 3:  return FUZZY_CALL2(GetCaptureDevice, CaptureEngine, int);
  case 4:  return FUZZY_CALL3(AllocateCaptureDevice, CaptureEngine, nsCString, PrincipalInfo);
  case 5:  return FUZZY_CALL2(ReleaseCaptureDevice, CaptureEngine, int);
  case 6:  return FUZZY_CALL3(StartCapture, CaptureEngine, int, VideoCaptureCapability);
  case 7:  return FUZZY_CALL2(StopCapture, CaptureEngine, int);
  //case 8:  callRet = FUZZY_CALL1(ReleaseFrame, Shmem); break;
  case 9:  return FUZZY_CALL0(AllDone);
  case 10: return FUZZY_CALL1(EnsureInitialized, CaptureEngine);
  default: LOG(("Unknown call index: %d", callIndex));
  }

  return true;
}

} // end of namespace camera
} // end of namespace mozilla
