#include "gtest/gtest.h"
#include "RiceDeltaDecoder.h"
#include "mozilla/ArrayUtils.h"

using namespace mozilla;
using namespace mozilla::safebrowsing;

struct TestingData {
  std::vector<uint32_t> mExpectedDecoded;
  std::vector<uint8_t> mEncoded;
  uint32_t mRiceParameter;
};

static void reverseByte(uint8_t& b);
static bool runOneTest(TestingData& aData, bool aSBV4Order);

TEST(RiceDeltaDecoder, NetworkOrder)
{
  static const std::vector<TestingData> TESTING_DATA = {
    // The first batch of testing data, which has the same delta values.
    {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x24, 0x92, 0x49, 0x24, 0x92, 0x49, 0x24, 0xff,},
      2
    },
    {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f},
      3
    },
    {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x08, 0x42, 0x10, 0x84, 0x21, 0x08, 0x42, 0x10, 0x84, 0x21, 0x08, 0x43},
      4
    },
    {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x04, 0x10, 0x41, 0x04, 0x10, 0x41, 0x04, 0x10, 0x41, 0x04, 0x10, 0x41, 0x04, 0x10, 0x7f},
      5
    },
        {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x81, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x81, 0x02, 0x04, 0x0f},
      6
    },
    {
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
      7
    },

    // The second batch of testing data, which has random delta values.
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0xff, 0xef, 0xff, 0xf7, 0xf1, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xef},
      1
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0xfb, 0xfe, 0xf8, 0x3f, 0xfc, 0x7f, 0xff, 0xf5, 0xff, 0xf3},
      2
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0xdf, 0xbd, 0x07, 0xe9, 0xff, 0x6f, 0xd7},
      3
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0x9e, 0xf6, 0x07, 0x67, 0xbb, 0x97},
      4
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0x5d, 0xf3, 0x01, 0x99, 0x9d, 0x57},
      5
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0x2e, 0x7c, 0x60, 0x15, 0x91, 0xcd, 0x7f},
      6
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      {0x17, 0x1f, 0x0c, 0x01, 0x2c, 0x4e, 0x35},
      7
    },
  };

  for (auto d : TESTING_DATA) {
    ASSERT_TRUE(runOneTest(d, false));
  }
}

// In this batch of tests, the encoded data would be like
// what we originally receive from the network. See comment
// in |runOneTest| for more detail.
TEST(RiceDeltaDecoder, SBV4Order) {
  static const std::vector<TestingData> TESTING_DATA = {
    {
      {5, 20, 29},
      {0xf7, 0x2,},
      2
    },
  };

  for (auto d : TESTING_DATA) {
    ASSERT_TRUE(runOneTest(d, true));
  }
}

// Make sure the shortage of encoded data wouldn't cause crash.
// The decoding should fail due to the input underflow.
TEST(RiceDeltaDecoder, InputUnderflow)
{
  // The testing data sets are copied and modified from the above tests.
  // The actual encoded data is annotated just for reference.
  static const std::vector<TestingData> TESTING_DATA = {
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0xff, 0xef, 0xff, 0xf7, 0xf1, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xef},
      {0xff, 0xef, 0xff, 0xf7, 0xf1, 0xff, 0xff, 0xfc, 0xff},
      1
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0xfb, 0xfe, 0xf8, 0x3f, 0xfc, 0x7f, 0xff, 0xf5, 0xff, 0xf3},
      {0xfb,},
      2
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0xdf, 0xbd, 0x07, 0xe9, 0xff, 0x6f, 0xd7},
      {},
      3
    },
  };

  for (auto d : TESTING_DATA) {
    ASSERT_FALSE(runOneTest(d, false));
  }
}

// Test if the trailing junk in the encoded data would cause crash.
// Note that the decoding would still succeed.
TEST(RiceDeltaDecoder, InputOverflow)
{
  // The testing data sets are copied and modified from the above tests.
  // The actual encoded data is annotated just for reference.
  static const std::vector<TestingData> TESTING_DATA = {
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0x9e, 0xf6, 0x07, 0x67, 0xbb, 0x97},
      {0x9e, 0xf6, 0x07, 0x67, 0xbb, 0x97, 0xca, 0xca, 0xca},
      4
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0x5d, 0xf3, 0x01, 0x99, 0x9d, 0x57},
      {0x5d, 0xf3, 0x01, 0x99, 0x9d, 0x57, 0xca, 0xca, 0xca},
      5
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0x2e, 0x7c, 0x60, 0x15, 0x91, 0xcd, 0x7f},
      {0x2e, 0x7c, 0x60, 0x15, 0x91, 0xcd, 0x7f, 0xca, 0xca, 0xca},
      6
    },
    {
      {12, 35, 66, 78, 79, 123, 201, 254},
      //{0x17, 0x1f, 0x0c, 0x01, 0x2c, 0x4e, 0x35},
      {0x17, 0x1f, 0x0c, 0x01, 0x2c, 0x4e, 0x35, 0xca, 0xca, 0xca},
      7
    },
  };

  for (auto d : TESTING_DATA) {
    ASSERT_TRUE(runOneTest(d, false));
  }
}


static void
reverseByte(uint8_t& b)
{
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
}

static bool
runOneTest(TestingData& aData, bool aSBV4Order)
{
  // By default, the decoder reads bits and interprets
  // the remainder from the MSB (network order). However, for
  // Safe Browsing V4, the data would be encoded and
  // "reversely" appended to the bit buffer on a byte basis [1].
  // That's why we need to reverse every single byte if the test
  // data is in the SBV4 order.
  //
  // Besides, no matter what order the bits are appended, the
  // remainder should be interpreted in big endian. Since we reverse
  // bits for SBV4 order, we need to specifically ask the decoder to
  // decode the remainder in "little endian" order.
  //
  // [1] https://developers.google.com/safe-browsing/v4/compression

  if (aSBV4Order) {
    for (auto& v : aData.mEncoded) {
      reverseByte(v);
    }
  }

  RiceDeltaDecoder decoder(&aData.mEncoded[0], aData.mEncoded.size());

  std::vector<uint32_t> decoded(aData.mExpectedDecoded.size());

  bool isRemainderBigEndian = !aSBV4Order;
  decoded[0] = aData.mExpectedDecoded[0];
  bool rv = decoder.Decode(aData.mRiceParameter,
                           decoded[0], // first value.
                           decoded.size() - 1, // # of entries (first value not included).
                           &decoded[1],
                           isRemainderBigEndian);

  return rv && decoded == aData.mExpectedDecoded;
}
