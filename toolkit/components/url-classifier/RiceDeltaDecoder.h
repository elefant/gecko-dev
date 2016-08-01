/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef RICE_DELTA_DECODER_H
#define RICE_DELTA_DECODER_H

namespace mozilla {
namespace safebrowsing {

class RiceDeltaDecoder {
public:
  RiceDeltaDecoder(uint8_t* aEncodedData, size_t aEncodedDataSize);

  bool Decode(uint32_t aRiceParameter,
              uint32_t aFirstValue,
              uint32_t aNumEntries,
              uint32_t* aDecodedData);

private:
  uint8_t* mEncodedData;
  size_t mEncodedDataSize;
};

} // namespace safebrowsing
} // namespace mozilla

#endif  // UPDATE_V4_DECODER_H
