#include <gtest/gtest.h>
#include "thirdparty/jsoncpp/value.h"
#include "thirdparty/jsoncpp/writer.h"
#include <glog/logging.h>
#include "toft/base/string/number.h"

TEST(TEST, json_test) {
  ::Json::Value j1;
  ::Json::Value j2;
  j1["b"] = 2;
  j1["a"] = 1;
  j2["c"] = 3;
  j2["d"] = ::toft::NumberToString(4);
  ::Json::FastWriter fastWriter;
  LOG(ERROR) << fastWriter.write(j1);
  LOG(ERROR) << j1.toStyledString();
  //j1.append(j2);
  //std::string c = j1.asString();
  LOG(ERROR) << j1.size();
  for (auto it = j2.begin(); it != j2.end(); ++it) {
    const std::string& key = it.key().asString();
    //j1[key] = j2[key].asInt();
  }
  for (auto it = j1.begin(); it != j1.end(); ++it) {
    LOG(ERROR) << it.key().asString();
    LOG(ERROR) << j1[it.key().asString()].asInt();
  }
}
