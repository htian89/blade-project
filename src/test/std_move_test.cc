#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "toft/system/time/timestamp.h"

TEST(TEST, StdMoveTest) {
  std::map<int64_t, std::string> a, b;
  b = {{1, "a"}, {2, "b"}};
  std::cout << "b: ";
  for (auto&& elem : b) {
    std::cout << elem.first << " : " << elem.second;
  }
  std::cout << std::endl;
  a.insert({1, std::move(b[1])});
  std::cout << "a: ";
  for (auto&& elem : a) {
    std::cout << elem.first << " : " << elem.second;
  }
  std::cout << std::endl;
  std::cout << "b: ";
  for (auto&& elem : b) {
    std::cout << elem.first << " : " << elem.second;
  }
  std::cout << std::endl;
}

TEST(TEST, VectorStdMoveTest) {
  std::vector<uint64_t> vec;
  int t = 0;
  for (int i = 200000; i > 0; --i) {
    vec.push_back(0);
  }
  t -= ::toft::GetTimeStampInUs();
  auto des1 = vec;
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "Copy vector total cost: " << t * 1.0/1000 << "ms";

  t = 0;
  t -= ::toft::GetTimeStampInUs();
  auto des2 = std::move(vec);
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "Move vector total cost: " << t * 1.0/1000 << "ms";
}

TEST(TEST, StringMoveTest) {
  std::string a("abcd");
  std::cout << "a: " << a << std::endl;
  std::string b = std::move(a);
  std::cout << "a: " << a << std::endl;
  std::cout << "b: " << b << std::endl;
  std::string c = "a";
  std::cout << "c: " << c << std::endl;
  c = std::move(b);
  std::cout << "b: " << b << std::endl;
  std::cout << "c: " << c << std::endl;
}
