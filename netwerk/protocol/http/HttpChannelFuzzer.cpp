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

bool FuzzyCall(bool aCallResult, const char* aMessageName)
{
  LOG(("Sending %s...", aMessageName));
  return aCallResult;
}

#define FT(P) FuzzTraits<P>::Fuzz()

#define FUZZY_CALL0(Message)\
  FuzzyCall(Send##Message(), #Message)
#define FUZZY_CALL1(Message, P1)\
  FuzzyCall(Send##Message(FT(P1)), #Message)
#define FUZZY_CALL2(Message, P1, P2)\
  FuzzyCall(Send##Message(FT(P1), FT(P2)), #Message)
#define FUZZY_CALL3(Message, P1, P2, P3)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3)), #Message)
#define FUZZY_CALL4(Message, P1, P2, P3, P4)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4)), #Message)
#define FUZZY_CALL5(Message, P1, P2, P3, P4, P5)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5)), #Message)
#define FUZZY_CALL6(Message, P1, P2, P3, P4, P5, P6)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6)), #Message)
#define FUZZY_CALL7(Message, P1, P2, P3, P4, P5, P6, P7)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7)), #Message)
#define FUZZY_CALL8(Message, P1, P2, P3, P4, P5, P6, P7, P8)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8)), #Message)
#define FUZZY_CALL9(Message, P1, P2, P3, P4, P5, P6, P7, P8, P9)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8), FT(P9)), #Message)
#define FUZZY_CALL10(Message, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4),FT(P5), FT(P6),FT(P7), FT(P8),FT(P9), FT(P10)), #Message)

///////////////////////////////////////////////////////////////////////////////
// nsITimerCallback implementation
NS_IMETHODIMP
HttpChannelFuzzer::Notify(nsITimer *timer)
{
  bool callRet = false;
  int callIndex = mCallIndex++ % kParentMessageNum;
  switch (callIndex) {
  case 0:  callRet = FUZZY_CALL1(SetClassOfService, uint32_t); break;
  case 1:  callRet = FUZZY_CALL1(SetCacheTokenCachedCharset, nsCString); break;
  case 2:  callRet = FUZZY_CALL2(UpdateAssociatedContentSecurity, int32_t, int32_t); break;
  case 3:  callRet = FUZZY_CALL0(Suspend); break;
  case 4:  callRet = FUZZY_CALL0(Resume); break;
  case 5:  callRet = FUZZY_CALL1(Cancel, nsresult); break;
  case 6:  callRet = FUZZY_CALL10(Redirect2Verify, nsresult, RequestHeaderTuples, uint32_t, uint32_t, OptionalURIParams, OptionalURIParams, OptionalCorsPreflightArgs, bool, bool, bool); break;
  case 7:  callRet = FUZZY_CALL0(DocumentChannelCleanup); break;
  case 8:  callRet = FUZZY_CALL0(MarkOfflineCacheEntryAsForeign); break;
  case 9:  callRet = FUZZY_CALL3(DivertOnDataAvailable, nsCString, uint64_t, uint32_t); break;
  case 10: callRet = FUZZY_CALL1(DivertOnStopRequest, nsresult); break;
  case 11: callRet = FUZZY_CALL0(DivertComplete); break;
  case 12: callRet = FUZZY_CALL2(RemoveCorsPreflightCacheEntry, URIParams, PrincipalInfo); break;
  case 13: callRet = FUZZY_CALL0(DeletingChannel); break;
  //case 14: callRet = FUZZY_CALL(__delete__, this); break;
  case 15: callRet = FUZZY_CALL0(FinishInterceptedRedirect); break;
  case 16: callRet = FUZZY_CALL1(SetPriority, int16_t); break;
  default: LOG(("Unknown call index: %d", callIndex)); break;
  }

  if (!callRet) {
    LOG(("Failed to call %d", callIndex));
  }

  return NS_OK;
}

} // namespace net
} // namespace mozilla
