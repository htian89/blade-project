// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>

#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <iterator>
#include <algorithm>

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

TEST_F(StlTestBench, OtherTest) {
}

TEST_F(StlTestBench, SetTest) {
  std::set<int> a = {1 ,2, 3};
  std::set<int> b = {};
  std::set<int> c = {4};
  a.insert(b.begin(), b.end());
  ASSERT_EQ(std::set<int>({1,2,3}), a);
  a.insert(c.begin(), c.end());
  ASSERT_EQ(std::set<int>({1,2,3,4}), a);
}

TEST_F(StlTestBench, VectorTest) {
  using namespace std;
  vector<int> vec;
  int i;
  cout << "vector size = " << vec.size() << endl;
  for (i = 0; i < 5; i++) {
    vec.push_back(i);
  }
  cout << "extended vector size = " << vec.size() << endl;
  for (i = 0; i < 5; i++) {
    cout << "value of vec[" << i <<"] = " << vec[i] << endl;
  }
  cout << "value of vec.back() = " << vec.back() << endl;
  cout << "value of vec.back() = " << vec.back() << endl;
  vector<int>::iterator v = vec.begin();
  while (v != vec.end()) {
    cout << "value of v = " << *v << endl;
    v++;
  }
}

TEST_F(StlTestBench, UnorderedMapTest) {
  srand(time(NULL));
  for (uint64_t i = 10; i > 0; i--) {
    uint64_t key = rand() % 20;
    std::set<uint64_t> s;
    for (uint64_t j = 1; j <= key; j++) {
      s.insert(key + j);
    }
    test_unordered_map_[key] = s;
  }
  const auto& test_map_temp = test_unordered_map_;
  for (auto it_map = test_map_temp.begin();
      it_map != test_map_temp.end(); it_map++) {
    std::cout << it_map->first << " : ";
    std::set<uint64_t> sets = it_map->second;
    for(auto it_set = sets.begin(); it_set != sets.end();
        it_set++) {
      std::cout << *it_set << " ";
    }
    std::cout << std::endl;
  }
}

TEST_F(StlTestBench, MapTest) {
  uint64_t i = 0;
  while (i < 100) {
    uint64_t key = i;
    i++;
    std::set<uint64_t> s;
    for (uint64_t j = 1; j <= key; j++) {
      s.insert(key + j);
      i++;
    }
    test_map_[key].insert(1);
  }
  std::map<uint64_t, std::set<uint64_t>> test_tmp;
  test_tmp[1] = {};
  for (auto it = test_tmp.begin(); it != test_tmp.end(); ++it) {
    std::cout << it->first << std::endl;
  }
}

TEST_F(StlTestBench, SetWithVectorTest) {
  std::cout << "test_set_with_vector" << std::endl;
  std::vector<std::vector<uint64_t>> platform_tag_info(2);
  platform_tag_info[0].resize(3);
  platform_tag_info[0][0] = 1;
  platform_tag_info[0][1] = 2;
  platform_tag_info[0][2] = 3;
  platform_tag_info[1].resize(3);
  platform_tag_info[1][0] = 4;
  platform_tag_info[1][1] = 5;
  platform_tag_info[1][2] = 6;
  std::set<std::vector<uint64_t>> tag_list_set;
  tag_list_set.insert(std::vector<uint64_t>(2));
  for (auto tag_attr = platform_tag_info.begin();
      tag_attr != platform_tag_info.end(); ++tag_attr) {
    if (tag_attr - platform_tag_info.begin() == 0) {
      std::set<std::vector<uint64_t>> tmp_tag_list_set;
      for (auto it_tag = tag_attr->begin();
           it_tag != tag_attr->end(); ++it_tag) {
        for (auto it_list = tag_list_set.begin();
            it_list != tag_list_set.end(); ++it_list) {
          std::vector<uint64_t> tmp_list = *it_list;
          tmp_list[0] = *it_tag;
          tmp_tag_list_set.insert(*it_list);
        }
      }
      tag_list_set = tmp_tag_list_set;
    }
    if (tag_attr - platform_tag_info.begin() == 1) {
      std::set<std::vector<uint64_t>> tmp_tag_list_set;
      for (auto it_tag = tag_attr->begin();
           it_tag != tag_attr->end(); ++it_tag) {
        for (auto it_list = tag_list_set.begin();
            it_list != tag_list_set.end(); ++it_list) {
          std::vector<uint64_t> tmp_list = *it_list;
          tmp_list[1] = *it_tag;
          tmp_tag_list_set.insert(*it_list);
        }
      }
      tag_list_set = tmp_tag_list_set;
    }
  }

  for (auto it_list = tag_list_set.begin();
      it_list != tag_list_set.end(); ++it_list) {
    std::cout << ((*it_list)[0]) << ":" << ((*it_list)[1]) << std::endl;
  }
}
