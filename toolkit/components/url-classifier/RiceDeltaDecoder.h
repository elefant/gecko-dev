/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef RICE_DELTA_DECODER_H
#define RICE_DELTA_DECODER_H

namespace mozilla {
namespace safebrowsing {

class RiceDeltaDecoder {
public:
  // The decoder would read bits in the network order. That is,
  // the bit reading order in one byte is from MSB to LSB.
  // If one wants the bit stream to be read from LSB, please
  // reverse it in advance.
  RiceDeltaDecoder(const uint8_t* aEncodedData, size_t aEncodedDataSize);

  // The bits would always be read from MSB of one byte but the
  // how the decoder interprets the remainder can be determined
  // by |aIsRemainderBigEndian|. By default, the remainder would
  // be interpreted in Big Endian order.
  bool Decode(uint32_t aRiceParameter,
              uint32_t aFirstValue,
              uint32_t aNumEntries,
              uint32_t* aDecodedData,
              bool aIsRemainderBigEndian = true);

private:
  const uint8_t* mEncodedData;
  size_t mEncodedDataSize;
};

} // namespace safebrowsing
} // namespace mozilla

#endif  // UPDATE_V4_DECODER_H
