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

  // The following structure and testing data is copied from Chromium source code:
  //
  // http://goo.gl/VgFIAc
  //
  // and will be tranlated to our own testing format.

  struct RiceDecodingTestInfo {
    uint32_t mRiceParameter;
    std::vector<uint32_t> mDeltas;
    std::string mEncoded;

    RiceDecodingTestInfo(uint32_t aRiceParameter,
                         const std::vector<uint32_t>& aDeltas,
                         const std::string& aEncoded)
      : mRiceParameter(aRiceParameter)
      , mDeltas(aDeltas)
      , mEncoded(aEncoded)
    {
    }
  };

  const std::vector<RiceDecodingTestInfo> TESTING_DATA_CHROMIUM = {
      RiceDecodingTestInfo(2, {15, 9}, "\xf7\x2"),
      RiceDecodingTestInfo(
          28, {1777762129, 2093280223, 924369848},
          "\xbf\xa8\x3f\xfb\xfc\xfb\x5e\x27\xe6\xc3\x1d\xc6\x38"),
      RiceDecodingTestInfo(
          28, {62763050, 1046523781, 192522171, 1800511020, 4442775, 582142548},
          "\x54\x60\x7b\xe7\x0a\x5f\xc1\xdc\xee\x69\xde"
          "\xfe\x58\x3c\xa3\xd6\xa5\xf2\x10\x8c\x4a\x59"
          "\x56\x00"),
      RiceDecodingTestInfo(
          28, {26067715, 344823336, 8420095, 399843890, 95029378, 731622412,
               35811335, 1047558127, 1117722715, 78698892},
          "\x06\x86\x1b\x23\x14\xcb\x46\xf2\xaf\x07\x08\xc9\x88\x54\x1f\x41\x04"
          "\xd5\x1a\x03\xeb\xe6\x3a\x80\x13\x91\x7b\xbf\x83\xf3\xb7\x85\xf1\x29"
          "\x18\xb3\x61\x09"),
      RiceDecodingTestInfo(
          27, {225846818, 328287420, 166748623, 29117720, 552397365, 350353215,
               558267528, 4738273, 567093445, 28563065, 55077698, 73091685,
               339246010, 98242620, 38060941, 63917830, 206319759, 137700744},
          "\x89\x98\xd8\x75\xbc\x44\x91\xeb\x39\x0c\x3e\x30\x9a\x78\xf3\x6a\xd4"
          "\xd9\xb1\x9f\xfb\x70\x3e\x44\x3e\xa3\x08\x67\x42\xc2\x2b\x46\x69\x8e"
          "\x3c\xeb\xd9\x10\x5a\x43\x9a\x32\xa5\x2d\x4e\x77\x0f\x87\x78\x20\xb6"
          "\xab\x71\x98\x48\x0c\x9e\x9e\xd7\x23\x0c\x13\x43\x2c\xa9\x01"),
      RiceDecodingTestInfo(
          28, {339784008, 263128563, 63871877, 69723256, 826001074, 797300228,
               671166008, 207712688},
          std::string("\x21\xc5\x02\x91\xf9\x82\xd7\x57\xb8\xe9\x3c\xf0\xc8\x4f"
                      "\xe8\x64\x8d\x77\x62\x04\xd6\x85\x3f\x1c\x97\x00\x04\x1b"
                      "\x17\xc6",
                      30)),
      RiceDecodingTestInfo(
          28, {471820069, 196333855, 855579133, 122737976, 203433838, 85354544,
               1307949392, 165938578, 195134475, 553930435, 49231136},
          "\x95\x9c\x7d\xb0\x8f\xe8\xd9\xbd\xfe\x8c\x7f\x81\x53\x0d\x75\xdc\x4e"
          "\x40\x18\x0c\x9a\x45\x3d\xa8\xdc\xfa\x26\x59\x40\x9e\x16\x08\x43\x77"
          "\xc3\x4e\x04\x01\xa4\xe6\x5d\x00"),
      RiceDecodingTestInfo(
          27, {87336845, 129291033, 30906211, 433549264, 30899891, 53207875,
               11959529, 354827862, 82919275, 489637251, 53561020, 336722992,
               408117728, 204506246, 188216092, 9047110, 479817359, 230317256},
          "\x1a\x4f\x69\x2a\x63\x9a\xf6\xc6\x2e\xaf\x73\xd0\x6f\xd7\x31\xeb\x77"
          "\x1d\x43\xe3\x2b\x93\xce\x67\x8b\x59\xf9\x98\xd4\xda\x4f\x3c\x6f\xb0"
          "\xe8\xa5\x78\x8d\x62\x36\x18\xfe\x08\x1e\x78\xd8\x14\x32\x24\x84\x61"
          "\x1c\xf3\x37\x63\xc4\xa0\x88\x7b\x74\xcb\x64\xc8\x5c\xba\x05"),
      RiceDecodingTestInfo(
          28, {297968956, 19709657, 259702329, 76998112, 1023176123, 29296013,
               1602741145, 393745181, 177326295, 55225536, 75194472},
          "\xf1\x94\x0a\x87\x6c\x5f\x96\x90\xe3\xab\xf7\xc0\xcb\x2d\xe9\x76\xdb"
          "\xf8\x59\x63\xc1\x6f\x7c\x99\xe3\x87\x5f\xc7\x04\xde\xb9\x46\x8e\x54"
          "\xc0\xac\x4a\x03\x0d\x6c\x8f\x00"),
      RiceDecodingTestInfo(
          28, {532220688, 780594691, 436816483, 163436269, 573044456, 1069604,
               39629436, 211410997, 227714491, 381562898, 75610008, 196754597,
               40310339, 15204118, 99010842},
          "\x41\x2c\xe4\xfe\x06\xdc\x0d\xbd\x31\xa5\x04\xd5\x6e\xdd\x9b\x43\xb7"
          "\x3f\x11\x24\x52\x10\x80\x4f\x96\x4b\xd4\x80\x67\xb2\xdd\x52\xc9\x4e"
          "\x02\xc6\xd7\x60\xde\x06\x92\x52\x1e\xdd\x35\x64\x71\x26\x2c\xfe\xcf"
          "\x81\x46\xb2\x79\x01"),
      RiceDecodingTestInfo(
          28, {219354713, 389598618, 750263679, 554684211, 87381124, 4523497,
               287633354, 801308671, 424169435, 372520475, 277287849},
          "\xb2\x2c\x26\x3a\xcd\x66\x9c\xdb\x5f\x07\x2e\x6f\xe6\xf9\x21\x10\x52"
          "\xd5\x94\xf4\x82\x22\x48\xf9\x9d\x24\xf6\xff\x2f\xfc\x6d\x3f\x21\x65"
          "\x1b\x36\x34\x56\xea\xc4\x21\x00"),
  };

  for (auto tdc : TESTING_DATA_CHROMIUM) {
    // Populate chromium testing data to our native testing data struct.
    TestingData d;

    d.mRiceParameter = tdc.mRiceParameter; // Populate rice parameter.

    // Populate encoded data from std::string to vector<uint8>.
    d.mEncoded.resize(tdc.mEncoded.size());
    memcpy(&d.mEncoded[0], tdc.mEncoded.c_str(), tdc.mEncoded.size());

    // Populate deltas to expected decoded data. The first value would be just
    // set to an arbitrary value, say 7, to avoid any assumption to the
    // first value in the implementation.
    d.mExpectedDecoded.resize(tdc.mDeltas.size() + 1);
    for (size_t i = 0; i < d.mExpectedDecoded.size(); i++) {
      if (0 == i) {
        d.mExpectedDecoded[i] = 7;
      } else {
        d.mExpectedDecoded[i] = d.mExpectedDecoded[i - 1] + tdc.mDeltas[i - 1];
      }
    }

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
