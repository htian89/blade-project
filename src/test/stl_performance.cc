// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>
#include <glog/logging.h>

#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <list>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <bitset>
#include "toft/system/time/timestamp.h"

#define TIME_START(x) (x -= ::toft::GetTimeStampInUs())
#define TIME_END(x) (x += ::toft::GetTimeStampInUs())

class StlTestBench: public ::testing::Test {
  public:
    StlTestBench() {
    }
  protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
    std::map<uint64_t, std::set<uint64_t>> test_map_;
    std::unordered_map<uint64_t, std::set<uint64_t>> test_unordered_map_;
};

TEST_F(StlTestBench, ListTest) {
  std::list<uint64_t> l;
  for (uint64_t i = 0; i < 100; ++i) {
    l.push_back(i);
  }
  uint64_t t;
  TIME_START(t);
  for (uint64_t i = 0; i < 1000000; ++i) {
    l.size();
  }
  TIME_END(t);
  LOG(ERROR) << "time cost: " << t/1000 << "ms";
}

TEST_F(StlTestBench, BitMapTest) {
  uint64_t t;
  TIME_START(t);
  std::bitset<64> bitmap;
  size_t counter = 0;
  for (uint64_t i = 0; i < 100000; ++i) {
    if (bitmap[i%64]) {
      continue;
    }
    bitmap.set(i%64);
    counter++;
  }
  TIME_END(t);
  LOG(ERROR) << "counter: " << counter;
  LOG(ERROR) << "time cost: " << t/1000 << "ms";
}
