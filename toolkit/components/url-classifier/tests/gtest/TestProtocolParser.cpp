/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

#include "gtest/gtest.h"
#include "ProtocolParser.h"

using namespace mozilla;
using namespace mozilla::safebrowsing;

TEST(ProtocolParser, UpdateWait)
{
  // Top level response which contains a list of update response
  // for different lists.
  FetchThreatListUpdatesResponse response;

  // Set min wait duration.
  auto minWaitDuration = response.mutable_minimum_wait_duration();
  minWaitDuration->set_seconds(8);
  minWaitDuration->set_nanos(1 * 1000000000);

  std::string s;
  response.SerializeToString(&s);

  ProtocolParser* p = new ProtocolParserProtobuf();
  p->AppendStream(nsCString(s.c_str(), s.length()));
  p->End();
  ASSERT_EQ(p->UpdateWait(), 9);
  delete p;
}
