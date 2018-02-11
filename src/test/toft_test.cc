#include <gtest/gtest.h>
#include <glog/logging.h>
#include "toft/system/time/timestamp.h"

TEST(TEST, time_test) {
  int t = 0;
  for (int i = 1e4; i > 0; --i) {
    t -= ::toft::GetTimeStampInUs();
    t += ::toft::GetTimeStampInUs();
  }
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}
