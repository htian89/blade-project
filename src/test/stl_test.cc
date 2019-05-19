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

typedef struct test_struct {
  int64_t a;
  static int64_t b;
} test_struct;
int64_t test_struct::b = 2;

TEST_F(StlTestBench, OtherTest) {
}

bool Compare1(const int& a, const int& b) {
  return (a > b);
}

bool Compare2(const int& a, const int& b) {
  return (a < b);
}

TEST_F(StlTestBench, ListTest) {
  std::list<int> l1 = {2,1,3};
  std::list<int> l2 = {2,1,3};
  ASSERT_EQ(l1, l2);
  std::copy(l1.begin(),l1.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  l1.sort(Compare1);
  std::copy(l1.begin(),l1.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  for (auto it = l1.begin();
      it != l1.end();) {
    auto it_temp = it;
    l1.erase(it_temp++);
    it = it_temp;
  }
  ASSERT_EQ(true, l1.empty());
}

TEST_F(StlTestBench, TimeTest) {
  return;
  std::list<int> l1;
  std::vector<int> v1;
  v1.clear();
  v1.reserve(0);
  for (int i = 0; i < 10000; i++) {
    l1.push_back(i);
    v1.push_back(i);
  }
  int times = 10000;
  clock_t time_list = clock();
  for (int i = 0; i < times; i++) {
    l1.sort(Compare1);
    l1.sort(Compare2);
    //l1.size();
  }
  time_list = clock() - time_list;
  std::cout << "list: " << (double)time_list/CLOCKS_PER_SEC << std::endl;
  clock_t time_vector = clock();
  for (int i = 0; i < times; i++) {
    std::stable_sort(v1.begin(),v1.end(), Compare1);
    std::stable_sort(v1.begin(),v1.end(), Compare2);
    //v1.size();
  }
  time_vector = clock() - time_vector;
  std::cout << "vector: " << (double)time_vector/CLOCKS_PER_SEC << std::endl;
}

TEST_F(StlTestBench, SetTest) {
  std::set<int> a = {3 ,2, 1};
  for (auto it = a.begin(); it != a.end(); ++it) {
    LOG(ERROR) << *it;
  }
  LOG(ERROR) << sizeof(a);
  LOG(ERROR) << sizeof(int);
  LOG(ERROR) << sizeof(int *);
  struct test_struct test_a;
  struct test_struct test_b;
  LOG(ERROR) << sizeof(test_a);
  LOG(ERROR) << sizeof(test_b);
  LOG(ERROR) << test_a.b;
  LOG(ERROR) << test_b.b;
  std::set<int> b = {};
  LOG(ERROR) << sizeof(b);
  std::set<int> c = {4};
  a.insert(b.begin(), b.end());
  ASSERT_EQ(std::set<int>({1,2,3}), a);
  LOG(ERROR) << sizeof(a);
  a.insert(c.begin(), c.end());
  ASSERT_EQ(std::set<int>({1,2,3,4}), a);
  LOG(ERROR) << sizeof(a);
}

TEST_F(StlTestBench, VectorTest) {
  using namespace std;
  vector<int> vec;
  LOG(ERROR) << sizeof(vec);
  int i;
  ASSERT_EQ(0, vec.size());
  for (i = 0; i < 5; i++) {
    vec.push_back(i);
  }
  ASSERT_EQ(5, vec.size());
  for (i = 0; i < 5; i++) {
    ASSERT_EQ(i, vec[i]);
  }
  ASSERT_EQ(4, vec.back());
  ASSERT_EQ(4, vec.back());
  vector<int>::iterator v = vec.begin();
  while (v != vec.end()) {
    ASSERT_EQ(v - vec.begin(), *v);
    v++;
  }
  vector<int> v1 = {1,2,3};
  vector<int> v2 = {2,3,4};
  vector<int> v3;
  v3.reserve(v1.size() + v2.size());
  v3.insert(v3.end(), v1.begin(), v1.end());
  v3.insert(v3.end(), v2.begin(), v2.end());
  ASSERT_EQ(6, v3.size());

  vector<int> v4;
  v4.insert(v4.end(), v1.begin(), v1.begin());
  ASSERT_EQ(0, v4.size());
  v4.clear();
  v4.insert(v4.end(), v1.begin(), v1.begin()+1);
  ASSERT_EQ(1, v4.size());
  vector<int> v5_cap;
  ASSERT_EQ(0, v5_cap.capacity());
  v5_cap.resize(10);
  ASSERT_EQ(10, v5_cap.capacity());
  v5_cap.clear();
  ASSERT_EQ(10, v5_cap.capacity());
  v5_cap.reserve(100);
  v5_cap.clear();
  v5_cap.reserve(10);
  ASSERT_EQ(100, v5_cap.capacity());
}

TEST_F(StlTestBench, VectorTest2) {
  std::vector<int> vec = {1,2,3};
  EXPECT_EQ(vec.begin() + 3, vec.end());
}

TEST_F(StlTestBench, Reference) {
  std::vector<int> a;
  std::vector<int>& b = a;
  b.push_back(101);
  ASSERT_EQ(101, a[0]);
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
  test_tmp[2] = {};
  test_tmp[3] = {};
  for (auto it = test_tmp.begin(); it != test_tmp.end(); ++it) {
    const auto& a = it->first;
    LOG(ERROR) << a;
    //std::cout << it->first << std::endl;
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

struct A {
  std::string a_;
  int b_;
};

TEST_F(StlTestBench, Struct) {
  std::vector<A> a_list;
  a_list.clear();
  a_list.resize(3);
  ASSERT_EQ(0, a_list[0].b_);
}

TEST_F(StlTestBench, Uint) {
  uint32_t i = 0;
  i -= 1;
  ASSERT_EQ(0xffffffff, i);
  i += 2;
  ASSERT_EQ(1, i);
}
