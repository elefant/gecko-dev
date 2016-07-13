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

bool runOneTest(const TestingData& aData);

TEST(RiceDeltaDecoder, Empty)
{
  static const std::vector<TestingData> TESTING_DATA = {
    // The first batch of testing data, which has the same delta values.
    {
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
      {0x55, 0x55, 0x55, 0x55, 0x57,},
      1
    },
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
    ASSERT_TRUE(runOneTest(d));
  }
}

bool runOneTest(const TestingData& aData)
{
  RiceDeltaDecoder decoder(&aData.mEncoded[0], aData.mEncoded.size());

  std::vector<uint32_t> decoded(aData.mExpectedDecoded.size());

  decoded[0] = aData.mExpectedDecoded[0];
  bool rv = decoder.Decode(aData.mRiceParameter,
                           decoded[0], // first value.
                           decoded.size() - 1, // # of entries (first value not included).
                           &decoded[1]);

  return rv && decoded == aData.mExpectedDecoded;
}
