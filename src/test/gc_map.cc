// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>
#include <glog/logging.h>
#include <iostream>
#include <sstream>
#include <map>
#include <string>

#include "joft/gc/hashmap_gc.h"
#include "toft/system/time/timestamp.h"

typedef std::string AttrType;

// 属性value到倒排链的映射
struct PredicatesInfo {
  joft::hashmap::HashmapGC<AttrType, void> predicate_map_;
};

// 中心属性下每个属性对应的PredicatesInfo
struct AttrPredicates {
  std::map<AttrType, PredicatesInfo> attr_predicate_map_;
};

class GCMapTestBench: public ::testing::Test {
  public:
    GCMapTestBench() {
    }
  protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
    AttrPredicates test_map_;
};

TEST_F(GCMapTestBench, memory) {
  return;
  int i,j;
  for (j = 10000; j > 0; j--) {
    PredicatesInfo info;
    for (i = 10000; i > 0 ; i--) {
      std::stringstream ss;
      ss << i;
      info.predicate_map_.Set(ss.str(), NULL);
    }
    std::stringstream ss2;
    ss2 << j;
    test_map_.attr_predicate_map_[ss2.str()] = info;
  }
  LOG(ERROR) << "completed." << j;
  while(1) {
    sleep(1);
  }
}

TEST_F(GCMapTestBench, time_cost) {
  std::vector<std::string> str_key;
  joft::hashmap::HashmapGC<std::string, void> str_map;
  std::vector<uint64_t> int_key;
  joft::hashmap::HashmapGC<uint64_t, void> int_map;
  for (uint64_t i = 0; i < 10000; ++i) {
    std::stringstream ss;
    ss << i;
    str_key.push_back(ss.str());
    str_map.Set(ss.str(), NULL);
    int_key.push_back(i);
    int_map.Set(i, NULL);
  }
  uint64_t t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (auto it = str_key.cbegin(); it != str_key.cend(); ++it) {
    str_map.Get(*it);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "str gc_map total cost: " << t * 1.0/1000 << "ms";
  t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (auto it = int_key.cbegin(); it != int_key.cend(); ++it) {
    int_map.Get(*it);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "int gc_map total cost: " << t * 1.0/1000 << "ms";
}
