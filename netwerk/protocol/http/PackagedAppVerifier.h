/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_PackagedAppVerifier_h
#define mozilla_net_PackagedAppVerifier_h

#include "mozilla/LinkedList.h"
#include "nsICacheEntry.h"
#include "nsIURI.h"

namespace mozilla {
namespace net {

class PackagedAppVerifierListener;

class PackagedAppVerifier final
{
public:
  enum EState {
    // The initial state.
	STATE_UNKNOWN,

    // When we are notified to process the first resource, we will start to
    // verify the manifest and go to this state no matter the package has 
    // signature or not.
    STATE_MANIFEST_VERIFYING,
  
    // Either the package has no signature or the manifest is verified 
    // successfully will we be in this state.
    STATE_MANIFEST_VERIFIED_OK,
  
    // iff the package has signature but the manifest is not well signed.
    STATE_MANIFEST_VERIFIED_FAILED,
  
    // The manifest is well signed but the resource integrity check failed.
    STATE_RESOURCE_VERIFIED_FAILED,
  };

public:
  struct ResourceCacheInfo : public mozilla::LinkedListElement<ResourceCacheInfo>
  {
    nsCOMPtr<nsIURI> mURI;
    nsCOMPtr<nsICacheEntry> mCacheEntry;
    nsresult mStatusCode;
    bool mIsLastPart;

    ResourceCacheInfo(nsIURI* aURI, 
                      nsICacheEntry* aCacheEntry, 
                      nsresult aStatusCode,
                      bool aIsLastPart)
      : mURI(aURI)
      , mCacheEntry(aCacheEntry)
      , mStatusCode(aStatusCode)
      , mIsLastPart(aIsLastPart)
    {
    }
  };

public:
  PackagedAppVerifier(PackagedAppVerifierListener* aListener,
                      const nsACString& aPackageOrigin);

  ~PackagedAppVerifier() { }

  // Called when a resource is already fully written in the cache. This resource 
  // will be processed and is guaranteed to be called back in either:
  //
  // 1) PackagedAppVerifierListener::OnManifestVerified:
  //    ------------------------------------------------------------------------
  //    If the resource is the first one in the package, it will be called 
  //    back in OnManifestVerified no matter this package has signature or not.
  //
  // 2) PackagedAppVerifierListener::OnResourceVerified.
  //    ------------------------------------------------------------------------
  //    Otherwise, the resource will be called back here.
  //
  void ProcessResourceCache(ResourceCacheInfo* aInfo);

  nsCString GetPackageOrigin() const;
  bool IsPackageSigned() const;
  bool HasSignature() const;

  void SetSignature(const nsACString& aSignature);

private:
  void QueueResource(ResourceCacheInfo* aInfo);

  // This two functions would call the actual verifier.
  void VerifyManifest(ResourceCacheInfo* aInfo);
  void VerifyResource(ResourceCacheInfo* aInfo);

  // FIXME: Remove until using the actual verifier. These two functions are 
  //        very likely to be wrapped around for the interal verifier callback.
  void OnManifestVerified(bool aSuccess);
  void OnResourceVerified(bool aSuccess);

  PackagedAppVerifierListener* mListener;
  mozilla::LinkedList<ResourceCacheInfo> mPendingResourceCacheInfoList;

  EState mState;
  nsCString mSignature;
  nsCString mPackageOrigin;

}; // class PackagedAppVerifier

class PackagedAppVerifierListener
{
public:
	typedef PackagedAppVerifier::ResourceCacheInfo ResourceCacheInfo;

public:
	virtual void OnManifestVerified(ResourceCacheInfo* aInfo, bool aSuccess) = 0;
	virtual void OnResourceVerified(ResourceCacheInfo* aInfo, bool aSuccess) = 0;
};

} // namespace net
} // namespace mozilla

#endif // mozilla_net_PackagedAppVerifier_h