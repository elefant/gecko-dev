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

extern bool
WillRedirect(nsHttpResponseHead * response);

//-----------------------------------------------------------------------------
// HttpChannelFuzzer
//-----------------------------------------------------------------------------

HttpChannelFuzzer::HttpChannelFuzzer()
{
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

  return NS_OK;
}

HttpChannelFuzzer::~HttpChannelFuzzer()
{
  LOG(("Destroying HttpChannelFuzzer @%p\n", this));
}

} // namespace net
} // namespace mozilla
