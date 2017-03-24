/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// HttpLog.h should generally be included first
#include "HttpLog.h"


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
#include "mozilla/net/HttpChannelFuzzer.h"
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

using namespace mozilla::dom;
using namespace mozilla::ipc;



namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS(HttpChannelFuzzer, nsITimerCallback)

extern bool
WillRedirect(nsHttpResponseHead * response);

//-----------------------------------------------------------------------------
// HttpChannelFuzzer
//-----------------------------------------------------------------------------

HttpChannelFuzzer::HttpChannelFuzzer()
  : mCallIndex(0)
{
  mTimer = do_CreateInstance("@mozilla.org/timer;1");
}

nsresult
HttpChannelFuzzer::Start()
{
  // Singleton: ContentChild, NeckoChild.
  ContentChild* cc = static_cast<ContentChild*>(gNeckoChild->Manager());
  if (cc->IsShuttingDown()) {
    return NS_ERROR_FAILURE;
  }

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

  nsresult rv = mTimer->InitWithCallback(this,
                                         100,
                                         nsITimer::TYPE_REPEATING_SLACK);
  if (NS_FAILED(rv)) {
    LOG(("Failed to init timer for fuzzing."));
  }

  return NS_OK;
}

HttpChannelFuzzer::~HttpChannelFuzzer()
{
  LOG(("Destroying HttpChannelFuzzer @%p\n", this));
}

///////////////////////////////////////////////////////////////////////////////
// nsITimerCallback implementation
NS_IMETHODIMP
HttpChannelFuzzer::Notify(nsITimer *timer)
{
  bool callRet = false;
  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  case 0:
    LOG(("Calling SendSetClassOfService()"));
    callRet = SendSetClassOfService(FuzzTraits<uint32_t>::Fuzz());
    break;
  case 1:
    LOG(("Calling SendSetCacheTokenCachedCharset()"));
    callRet = SendSetCacheTokenCachedCharset(FuzzTraits<nsCString>::Fuzz());
    break;
  case 2:
    LOG(("Calling SendUpdateAssociatedContentSecurity()"));
    callRet = SendUpdateAssociatedContentSecurity(FuzzTraits<int32_t>::Fuzz(),
                                                  FuzzTraits<int32_t>::Fuzz());
    break;
  case 3:
      LOG(("Calling SendSuspend()"));
    callRet = SendSuspend();
    break;
  case 4:
    LOG(("Calling SendResume()"));
    callRet = SendResume();
    break;
  case 5:
    LOG(("Calling SendCancel()"));
    callRet = SendCancel(FuzzTraits<nsresult>::Fuzz());
    break;
  case 6:
    LOG(("Calling SendRedirect2Verify()"));
    callRet = SendRedirect2Verify(FuzzTraits<nsresult>::Fuzz(),
                                  FuzzTraits<RequestHeaderTuples>::Fuzz(),
                                  FuzzTraits<uint32_t>::Fuzz(),
                                  FuzzTraits<uint32_t>::Fuzz(),
                                  FuzzTraits<OptionalURIParams>::Fuzz(),
                                  FuzzTraits<OptionalURIParams>::Fuzz(),
                                  FuzzTraits<OptionalCorsPreflightArgs>::Fuzz(),
                                  FuzzTraits<bool>::Fuzz(),
                                  FuzzTraits<bool>::Fuzz(),
                                  FuzzTraits<bool>::Fuzz());
    break;
  case 7:
    LOG(("Calling SendDocumentChannelCleanup()"));
    callRet = SendDocumentChannelCleanup();
    break;
  case 8:
    LOG(("Calling SendMarkOfflineCacheEntryAsForeign()"));
    callRet = SendMarkOfflineCacheEntryAsForeign();
    break;
  case 9:
    LOG(("Calling SendDivertOnDataAvailable()"));
    callRet = SendDivertOnDataAvailable(FuzzTraits<nsCString>::Fuzz(),
                                        FuzzTraits<uint64_t>::Fuzz(),
                                        FuzzTraits<uint32_t>::Fuzz());
    break;
  case 10:
    LOG(("Calling SendDivertOnStopRequest()"));
    callRet = SendDivertOnStopRequest(FuzzTraits<nsresult>::Fuzz());
    break;
  case 11:
    LOG(("Calling SendDivertComplete()"));
    callRet = SendDivertComplete();
    break;
  case 12:
    LOG(("Calling SendRemoveCorsPreflightCacheEntry()"));
    callRet = SendRemoveCorsPreflightCacheEntry(FuzzTraits<URIParams>::Fuzz(),
                                                FuzzTraits<PrincipalInfo>::Fuzz());
    break;
  case 13:
    LOG(("Calling SendDeletingChannel()"));
    callRet = SendDeletingChannel();
    break;
  case 14:
    LOG(("Calling Send__delete__()"));
    //callRet = Send__delete__(this);
    break;
  case 15:
    LOG(("Calling SendFinishInterceptedRedirect()"));
    callRet = SendFinishInterceptedRedirect();
    break;
  case 16:
    LOG(("Calling SendSetPriority()"));
    callRet = SendSetPriority(FuzzTraits<int16_t>::Fuzz());
    break;
  default:
    LOG(("Unknown call index: %d", callIndex));
    break;
  }

  if (!callRet) {
    LOG(("Failed to call %d", callIndex));
  }

  return NS_OK;
}

} // namespace net
} // namespace mozilla
