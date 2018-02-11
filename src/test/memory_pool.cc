#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "toft/system/time/timestamp.h"

template <typename T>
class MemoryPool {
  typedef T value_type;
 public:
  MemoryPool(size_t block_size) {
    for (size_t i = 0; i < block_size; i++) {
      free_blocks_.push(new value_type);
    }
  }
  ~MemoryPool() {
    while (!free_blocks_.empty()) {
      delete free_blocks_.front();
      free_blocks_.pop();
    }
  }
  value_type* Alloc() {
    if (free_blocks_.empty()) {
      return NULL;
    }
    value_type* ret = free_blocks_.front();
    free_blocks_.pop();
    return ret;
  }
  void DeAlloc(value_type* p) {
    free_blocks_.push(p);
  }
 protected:
  std::queue<value_type*> free_blocks_;
};

template <typename T>
class MemoryPool2 {
  typedef T value_type;
 public:
  MemoryPool2(size_t block_size) {
    for (size_t i = 0; i < block_size; i++) {
      free_blocks_.push(new value_type);
    }
  }
  ~MemoryPool2() {
    while (!free_blocks_.empty()) {
      delete free_blocks_.top();
      free_blocks_.pop();
    }
  }
  value_type* Alloc() {
    if (free_blocks_.empty()) {
      return NULL;
    }
    value_type* ret = free_blocks_.top();
    free_blocks_.pop();
    return ret;
  }
  void DeAlloc(value_type* p) {
    free_blocks_.push(p);
  }
 protected:
  std::stack<value_type*> free_blocks_;
};

struct A {
  void Clear() {
    a = {0};
  }
  int a[100];
};

TEST(TEST, MemoryPoolTest) {
  MemoryPool<struct A> pool(200000);
  std::vector<struct A*> vec;
  vec.reserve(200000);
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(pool.Alloc());
  }
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    pool.DeAlloc(*it);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";

  MemoryPool2<struct A> pool2(200000);
  vec.clear();
  vec.reserve(200000);
  t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(pool2.Alloc());
  }
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    pool2.DeAlloc(*it);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";

  vec.clear();
  t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(new A);
  }
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    delete *it;
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}

TEST(TEST, VectorTest) {
  std::vector<uint64_t> vec;
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(0);
  }
  vec.resize(0);
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";

  t = 0;
  vec.reserve(200000);
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(0);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}

struct Ids {
  std::set<uint64_t> id2_;
  std::set<uint64_t> id3_;
};

TEST(TEST, MapTest) {
  std::map<uint64_t, std::map<uint64_t, std::set<uint64_t>>> result;
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 15000; i > 0; --i) {
    for (int k = 3; k > 0; --k) {
      auto& id2 = result[i];
      for (int j = 5; j > 0; --j) {
        id2[j].insert(k);
      }
    }
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}

TEST(TEST, MapTest2) {
  std::map<uint64_t, Ids> result;
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  for (int i = 15000; i > 0; --i) {
    for (int k = 3; k > 0; --k) {
      auto& ids = result[i];
      for (int j = 5; j > 0; --j) {
        ids.id2_.insert(j);
        ids.id3_.insert(k);
      }
    }
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}

TEST(TEST, UnorderedMapTest) {
  std::unordered_map<uint64_t, Ids> result;
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  Ids ids;
  for (int k = 3; k > 0; --k) {
    for (int i = 50000; i > 0; --i) {
      //auto& ids = result[i];
      for (int j = 5; j > 0; --j) {
        ids.id2_.insert(j);
        ids.id3_.insert(k);
      }
    }
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}
