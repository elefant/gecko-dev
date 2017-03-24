/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_HttpChannelFuzzer_h
#define mozilla_net_HttpChannelFuzzer_h

/*
#include "mozilla/UniquePtr.h"
#include "mozilla/net/HttpBaseChannel.h"
*/
#include "mozilla/net/PHttpChannelChild.h"
#include "nsITimer.h"

 /*
#include "mozilla/net/ChannelEventQueue.h"

#include "nsIStreamListener.h"
#include "nsILoadGroup.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIProgressEventSink.h"
#include "nsICacheInfoChannel.h"
#include "nsIApplicationCache.h"
#include "nsIApplicationCacheChannel.h"
#include "nsIUploadChannel2.h"
#include "nsIResumableChannel.h"
#include "nsIProxiedChannel.h"
#include "nsIAsyncVerifyRedirectCallback.h"
#include "nsIAssociatedContentSecurity.h"
#include "nsIChildChannel.h"
#include "nsIHttpChannelChild.h"
#include "nsIDivertableChannel.h"
#include "mozilla/net/DNS.h"
*/

namespace mozilla {
namespace net {

class HttpChannelFuzzer final : public PHttpChannelChild
                              , public nsITimerCallback
{
public:
  HttpChannelFuzzer();

  NS_DECL_ISUPPORTS
  NS_DECL_NSITIMERCALLBACK

  nsresult Start();

private:
  virtual ~HttpChannelFuzzer();

  mozilla::ipc::IPCResult RecvNotifyTrackingProtectionDisabled() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvNotifyTrackingResource() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvSetClassifierMatchedInfo(const ClassifierInfo& aInfo) override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvOnStartRequest(const nsresult& channelStatus,
                                             const nsHttpResponseHead& responseHead,
                                             const bool& useResponseHead,
                                             const nsHttpHeaderArray& requestHeaders,
                                             const bool& isFromCache,
                                             const bool& cacheEntryAvailable,
                                             const uint32_t& cacheExpirationTime,
                                             const nsCString& cachedCharset,
                                             const nsCString& securityInfoSerialization,
                                             const NetAddr& selfAddr,
                                             const NetAddr& peerAddr,
                                             const int16_t& redirectCount,
                                             const uint32_t& cacheKey,
                                             const nsCString& altDataType,
                                             const int64_t& altDataLen) override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvOnTransportAndData(const nsresult& channelStatus,
                                                 const nsresult& status,
                                                 const uint64_t& offset,
                                                 const uint32_t& count,
                                                 const nsCString& data) override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvOnStopRequest(const nsresult& statusCode, const ResourceTimingStruct& timing) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvOnProgress(const int64_t& progress, const int64_t& progressMax) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvOnStatus(const nsresult& status) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvFailedAsyncOpen(const nsresult& status) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvRedirect1Begin(const uint32_t& registrarId,
                                             const URIParams& newURI,
                                             const uint32_t& redirectFlags,
                                             const nsHttpResponseHead& responseHead,
                                             const nsCString& securityInfoSerialization,
                                             const nsCString& channelId) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvRedirect3Complete() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvAssociateApplicationCache(const nsCString& groupID,
                                                        const nsCString& clientID) override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvFlushedForDiversion() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvDivertMessages() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvDeleteSelf() override { return IPC_OK(); }
  mozilla::ipc::IPCResult RecvFinishInterceptedRedirect() override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvReportSecurityMessage(const nsString& messageTag,
                                                    const nsString& messageCategory) override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvIssueDeprecationWarning(const uint32_t& warning,
                                                      const bool& asError) override { return IPC_OK(); }

  mozilla::ipc::IPCResult RecvSetPriority(const int16_t& aPriority) override { return IPC_OK(); }

  nsCOMPtr<nsITimer> mTimer;

  uint32_t mCallIndex;

  const uint32_t kParentMessageNum = 17;
};

}
}

#endif // mozilla_net_HttpChannelFuzzer_h
