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
