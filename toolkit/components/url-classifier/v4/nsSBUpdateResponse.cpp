/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsSBUpdateResponse.h"

using namespace mozilla::safebrowsing;

/////////////////////////////////////////////////////////////////////////////
// nsSafeBrowsingUpdateResponseV4

NS_IMPL_ISUPPORTS(nsSBUpdateResponse, nsISBUpdateResponse)

nsSBUpdateResponse::nsSBUpdateResponse(const ListUpdateResponse& aResponse)
  : mIsFullUpdate(false)
{
  nsCOMPtr<nsIUrlClassifierUtils> urlUtil =
    do_GetService(NS_URLCLASSIFIERUTILS_CONTRACTID);

  // Init list name.
  if (aResponse.has_threat_type()) {
    auto threatType = aResponse.threat_type();
    nsresult rv = urlUtil->ConvertThreatTypeToListName(threatType, mListName);
    NS_WARN_IF(NS_FAILED(rv));
  } else {
    NS_WARNING("Threat type not initialized.");
  }

  // Init "isFullUpdate".
  if (aResponse.has_response_type()) {
    mIsFullUpdate =
      aResponse.response_type() == ListUpdateResponse::FULL_UPDATE;
  } else {
    NS_WARNING("Response type not initialized.");
  }

  // Init "new state".
  if (aResponse.has_new_client_state()) {
      mNewState.Assign(aResponse.new_client_state().c_str(),
                       aResponse.new_client_state().size());
  } else {
    NS_WARNING("New state not initialized.");
  }

  PopulateUpdate(aResponse.additions(), true /*aIsAddition*/ );
  PopulateUpdate(aResponse.removals(), false);
}

nsSBUpdateResponse::~nsSBUpdateResponse()
{
}

NS_IMETHODIMP
nsSBUpdateResponse::GetListName(nsACString& aListName)
{
  aListName = mListName;
  return NS_OK;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetIsFullUpdate(bool *aIsFullUpdate)
{
  *aIsFullUpdate = mIsFullUpdate;
  return NS_OK;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetNewState(nsACString& aNewState)
{
  aNewState = mNewState;
  return NS_OK;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetNewChecksum(nsACString& aNewChecksum)
{
  aNewChecksum = mNewChecksum;
  return NS_OK;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetFixedLengthAdditions(nsACString& aFixedLengthAdditions)
{
  aFixedLengthAdditions = mFixedLengthAdditions;
  return NS_OK;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetVariableLengthAdditions(nsIArray** aVariableLengthAdditions)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsSBUpdateResponse::GetRemovalIndices(nsACString& aRemovalIndices)
{
  aRemovalIndices = mRemovalIndices;
  return NS_OK;
}

////////////////////////////////////////////////////////////////////
// Non-interface functions.

void
nsSBUpdateResponse::PopulateUpdate(const ThreatEntrySetList& aUpdate,
                                   bool aIsAddition)
{
  for (int i = 0; i < aUpdate.size(); i++) {
    auto update = aUpdate.Get(i);
    if (!update.has_compression_type()) {
      NS_WARNING("Removal with no compression type.");
      continue;
    }

    switch (update.compression_type()) {
    case COMPRESSION_TYPE_UNSPECIFIED:
      NS_WARNING("Unspecified compression type.");
      break;

    case RAW:
      aIsAddition ? PopulateRawAddition(update)
                  : PopulateRawRemoval(update);
      break;

    case RICE:
      aIsAddition ? PopulateEncodedAddition(update)
                  : PopulateEncodedRemoval(update);
      break;
    }
  }
}

void
nsSBUpdateResponse::PopulateRawAddition(const ThreatEntrySet& aAddition)
{
  if (!aAddition.has_raw_hashes()) {
    return;
  }

  auto rawHashes = aAddition.raw_hashes();
  if (!rawHashes.has_prefix_size()) {
    NS_WARNING("Raw hash has no prefix size");
    return;
  }

  auto prefixes = rawHashes.raw_hashes();
  if (4 == rawHashes.prefix_size()) {
    // Put fixed-length (4) prefixes to one single storage.
    mFixedLengthAdditions.Append(prefixes.c_str(), prefixes.size());
  } else {
    PopulateVariableLengthAddition(rawHashes.prefix_size(), prefixes);
  }
}

void
nsSBUpdateResponse::PopulateVariableLengthAddition(int aPrefixSize,
                                                   const std::string& aPrefixes)
{
  NS_WARNING("Variable-length addition not supported yet.");
}

void
nsSBUpdateResponse::PopulateEncodedAddition(const ThreatEntrySet& aAddition)
{
  NS_WARNING("Encoded additions not supported yet.");
}

void
nsSBUpdateResponse::PopulateRawRemoval(const ThreatEntrySet& aRemoval)
{
  if (!aRemoval.has_raw_indices()) {
    NS_WARNING("A removal has no indices.");
    return;
  }

  // The type of |indices| is an vector of int32.
  auto indices = aRemoval.raw_indices().indices();
  mRemovalIndices.Assign((char*)indices.data(), indices.size() * 4);
}

void
nsSBUpdateResponse::PopulateEncodedRemoval(const ThreatEntrySet& aRemoval)
{
  NS_WARNING("Encoded removal not supported yet.");
}