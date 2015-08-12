/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsICacheStorage.h"
#include "nsICacheStorageService.h"
#include "../../cache2/CacheFileUtils.h"
#include "mozilla/Logging.h"
#include "mozilla/DebugOnly.h"
#include "nsThreadUtils.h"
#include "PackagedAppVerifier.h"

// Defined in PackagedAppService.cpp
extern PRLogModuleInfo *gPASLog;

#undef LOG
#define LOG(args) MOZ_LOG(gPASLog, mozilla::LogLevel::Debug, args)

#ifdef MOZ_WIDGET_GONK
  #undef LOG
  #define LOG(args) printf_stderr args
#endif

namespace {

nsCString UriToString(nsIURI* aURI);

}

namespace mozilla {
namespace net {

///////////////////////////////////////////////////////////////////////////////

PackagedAppVerifier::PackagedAppVerifier(PackagedAppVerifierListener* aListener,
                                         const nsACString& aPackageOrigin)
  : mListener(aListener)
  , mState(STATE_UNKNOWN)
  , mPackageOrigin(aPackageOrigin)
{
}

void
PackagedAppVerifier::SetSignature(const nsACString& aSignature)
{
  mSignature = aSignature;
}

void
PackagedAppVerifier::ProcessResourceCache(ResourceCacheInfo* aInfo)
{
  MOZ_RELEASE_ASSERT(NS_IsMainThread(), "OnResourceCached must be on main thread");

  LOG(("PackagedAppVerifier::ProcessResourceCache: %s. State: %d", UriToString(aInfo->mURI).get(), mState));

  QueueResource(aInfo);

  switch (mState) {
  case STATE_UNKNOWN:
    // The first resource has to be the manifest.
    VerifyManifest(aInfo);
    break;

  case STATE_MANIFEST_VERIFYING:
    // A resource is cached in the middle of manifest verification. Queue it and
    // verify it until we make sure the manifest is verified.
    break;

  case STATE_MANIFEST_VERIFIED_OK:
    VerifyResource(aInfo);
    break;

  case STATE_MANIFEST_VERIFIED_FAILED:
    OnResourceVerified(false);
    break;

  default:
    MOZ_CRASH("Unexpected PackagedAppVerifier state."); // Shouldn't get here.
    break;
  }
}

void
PackagedAppVerifier::VerifyManifest(ResourceCacheInfo* aInfo)
{
  MOZ_RELEASE_ASSERT(NS_IsMainThread(), "Manifest verification must be on main thread");

  LOG(("PackagedAppVerifier::VerifyManifest: %s", UriToString(aInfo->mURI).get()));

  mState = STATE_MANIFEST_VERIFYING;

  // FIXME: Fire a fake successful OnManifestVerified event.
  nsCOMPtr<nsIRunnable> r =
    NS_NewNonOwningRunnableMethodWithArgs<bool>(this, &PackagedAppVerifier::OnManifestVerified, true);
  NS_DispatchToMainThread(r);

  // TODO: Delegate to the actual verifier.
}

void
PackagedAppVerifier::VerifyResource(ResourceCacheInfo* aInfo)
{
  MOZ_RELEASE_ASSERT(NS_IsMainThread(), "Resource resource must be on main thread");

  LOG(("PackagedAppVerifier::VerifyResource: %s", UriToString(aInfo->mURI).get()));

  // FIXME: Fire a fake successful OnResourceVerified event.
  nsCOMPtr<nsIRunnable> r =
    NS_NewNonOwningRunnableMethodWithArgs<bool>(this, &PackagedAppVerifier::OnResourceVerified, true);
  NS_DispatchToMainThread(r);

  // TODO: Delegate to the actual verifier.
}

void
PackagedAppVerifier::OnManifestVerified(bool aSuccess)
{
  LOG(("PackagedAppVerifier::OnManifestVerified: %d", aSuccess));

  mState = aSuccess ? STATE_MANIFEST_VERIFIED_OK
                    : STATE_MANIFEST_VERIFIED_FAILED;

  ResourceCacheInfo* info = mPendingResourceCacheInfoList.popFirst();
  MOZ_ASSERT(info);

  mListener->OnManifestVerified(info, aSuccess);

  if (!aSuccess) {
    return;
  }

  LOG(("Ready to verify resources that were cached during verification"));
  // Verify the resources which were cached during verification accordingly.
  info = mPendingResourceCacheInfoList.getFirst();
  for (; info; info = info->getNext()) {
    VerifyResource(info);
  }
}

void
PackagedAppVerifier::OnResourceVerified(bool aSuccess)
{
  ResourceCacheInfo* info = mPendingResourceCacheInfoList.popFirst();
  MOZ_ASSERT(info);

  LOG(("PackagedAppVerifier::OnResourceVerified: %s has been verified: %d",
       UriToString(info->mURI).get(), aSuccess));

  // Must be called on main thread.
  mListener->OnResourceVerified(info, aSuccess);
}

void
PackagedAppVerifier::QueueResource(ResourceCacheInfo* aInfo)
{
  LOG(("PackagedAppVerifier::QueueResource: %s", UriToString(aInfo->mURI).get()));
  mPendingResourceCacheInfoList.insertBack(aInfo);
}

nsCString
PackagedAppVerifier::GetPackageOrigin() const
{
  return mPackageOrigin;
}

bool
PackagedAppVerifier::IsPackageSigned() const
{
  return HasSignature() && STATE_MANIFEST_VERIFIED_OK;
}

bool
PackagedAppVerifier::HasSignature() const
{
  return !mSignature.IsEmpty();
}

} // namespace net
} // namespace mozilla
