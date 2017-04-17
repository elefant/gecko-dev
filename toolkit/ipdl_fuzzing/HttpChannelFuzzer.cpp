/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentChild.h"
#include "SerializedLoadContext.h"
#include "nsPrintfCString.h"

/*
#include "nsHttp.h"
#include "nsICacheEntry.h"
#include "mozilla/Unused.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/dom/DocGroup.h"
#include "mozilla/dom/TabChild.h"
#include "mozilla/dom/TabGroup.h"
#include "mozilla/ipc/FileDescriptorSetChild.h"
#include "mozilla/ipc/IPCStreamUtils.h"
*/
#include "mozilla/net/NeckoChild.h"
#include "mozilla/fuzzing/HttpChannelFuzzer.h"
#include "FuzzTraitsNeckoChannelParams.h"

/*
#include "nsISupportsPrimitives.h"
#include "nsChannelClassifier.h"
#include "nsGlobalWindow.h"
#include "nsStringStream.h"
#include "nsHttpHandler.h"
#include "nsNetUtil.h"
#include "nsSerializationHelper.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/Performance.h"
#include "mozilla/ipc/InputStreamUtils.h"
#include "mozilla/ipc/URIUtils.h"
#include "mozilla/ipc/BackgroundUtils.h"
#include "mozilla/net/ChannelDiverterChild.h"
#include "mozilla/net/DNS.h"
#include "SerializedLoadContext.h"
#include "nsInputStreamPump.h"
#include "InterceptedChannel.h"
#include "mozIThirdPartyUtil.h"
#include "nsContentSecurityManager.h"
#include "nsIDeprecationWarner.h"
#include "nsICompressConvStats.h"
#include "nsIDocument.h"
#include "nsIDOMWindowUtils.h"
#include "nsStreamUtils.h"
*/

#undef LOG
#define LOG(args) printf_stderr(">>>>>> HttpChannelFuzzer <<<<<<< %s\n", (nsPrintfCString args).get())
//#define LOG

#include "FuzzyCall.h"

using namespace mozilla::dom;
using namespace mozilla::ipc;

namespace mozilla {
namespace net {

//-----------------------------------------------------------------------------
// HttpChannelFuzzer
//-----------------------------------------------------------------------------

nsresult
HttpChannelFuzzer::CreateParentActor()
{
  // Singleton: ContentChild, NeckoChild.
  ContentChild* cc = static_cast<ContentChild*>(gNeckoChild->Manager());
  if (cc->IsShuttingDown()) {
    return NS_ERROR_FAILURE;
  }

  srand(time(NULL));

  // Non-ipdlh type.
  IPC::SerializedLoadContext loadContext;

  // ipdlh type.
  auto openArgs = FuzzTraits<HttpChannelOpenArgs>::Fuzz();

  // ipdlh type.
  auto browser = FuzzTraits<PBrowserOrId>::Fuzz();

  if (!gNeckoChild->SendPHttpChannelConstructor(this,
                                                browser,
                                                loadContext,
                                                openArgs)) {
    LOG(("Failed to construct HttpChannel parent-side actor."));
    return NS_ERROR_FAILURE;
  }

  LOG(("Successful to construct HttpChannel parent-side actor."));
  return NS_OK;
}

bool
HttpChannelFuzzer::SendOneIPCMessage()
{
  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  case 0:  return FUZZY_CALL1(this, SetClassOfService, uint32_t);
  case 1:  return FUZZY_CALL1(this, SetCacheTokenCachedCharset, nsCString);
  case 2:  return FUZZY_CALL2(this, UpdateAssociatedContentSecurity, int32_t, int32_t);
  case 3:  return FUZZY_CALL0(this, Suspend);
  case 4:  return FUZZY_CALL0(this, Resume);
  case 5:  return FUZZY_CALL1(this, Cancel, nsresult);
  case 6:  return FUZZY_CALL10(this, Redirect2Verify, nsresult, RequestHeaderTuples, uint32_t, uint32_t, OptionalURIParams, OptionalURIParams, OptionalCorsPreflightArgs, bool, bool, bool);
  case 7:  return FUZZY_CALL0(this, DocumentChannelCleanup);
  case 8:  return FUZZY_CALL0(this, MarkOfflineCacheEntryAsForeign);
  //case 9:  return FUZZY_CALL3(DivertOnDataAvailable, nsCString, uint64_t, uint32_t);
  //case 10: return = FUZZY_CALL1(DivertOnStopRequest, nsresult);
  //case 11: return FUZZY_CALL0(DivertComplete);
  case 12: return FUZZY_CALL2(this, RemoveCorsPreflightCacheEntry, URIParams, PrincipalInfo);
  case 13: return FUZZY_CALL0(this, DeletingChannel);
  //case 14: return FUZZY_CALL(__delete__, this);
  case 15: return FUZZY_CALL0(this, FinishInterceptedRedirect);
  case 16: return FUZZY_CALL1(this, SetPriority, int16_t);
  default: LOG(("Unknown call index: %d", callIndex));
  }

  return true;
}

} // namespace net
} // namespace mozilla
