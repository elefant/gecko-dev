/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsSBUpdateResponse_h
#define nsSBUpdateResponse_h

#include "nsISBUpdateResponse.h"
#include "nsAutoPtr.h"
#include "safebrowsing.pb.h"

class nsSBUpdateResponse final : public nsISBUpdateResponse
{
public:
  typedef mozilla::safebrowsing::FetchThreatListUpdatesResponse_ListUpdateResponse
    ListUpdateResponse;

  typedef google::protobuf::RepeatedPtrField< ::mozilla::safebrowsing::ThreatEntrySet >
    ThreatEntrySetList;

public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISBUPDATERESPONSE

  nsSBUpdateResponse(const ListUpdateResponse& aResponse);

private:
  ~nsSBUpdateResponse();

  // Populate additions or removals to internal storage.
  // |aUpdate| could be additions or removals.
  void PopulateUpdate(const ThreatEntrySetList& aUpdate,
                      bool aIsAddition);

  // For additions.
  void PopulateRawAddition(const ThreatEntrySet& aAddition);
  void PopulateEncodedAddition(const ThreatEntrySet& aAddition);
  void PopulateVariableLengthAddition(int aPrefixSize,
                                      const std::string& aPrefixes);

  // For removals.
  void PopulateRawRemoval(const ThreatEntrySet& aRemoval);
  void PopulateEncodedRemoval(const ThreatEntrySet& aRemoval);

  nsCString mListName;
  bool mIsFullUpdate;
  nsXPIDLCString mNewState;
  nsXPIDLCString mNewChecksum;

  // Stores fixed-length (4) prefixes.
  nsXPIDLCString mFixedLengthAdditions;

  // Stores 4-byte indices for removal.
  nsXPIDLCString mRemovalIndices;
};

#endif
