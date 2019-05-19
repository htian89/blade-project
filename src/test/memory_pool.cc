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
  MemoryPool(size_t block_size) : pool_size_(block_size) {
    all_blocks_.reserve(block_size);
    for (size_t i = 0; i < block_size; i++) {
      all_blocks_.push_back(new value_type);
    }
  }
  ~MemoryPool() {
    while (!all_blocks_.empty()) {
      delete all_blocks_.back();
      all_blocks_.pop_back();
    }
  }
  void Init() {
    counter_ = 0;
  }
  value_type* Alloc() {
    if (counter_ >= pool_size_) {
      return NULL;
    }
    return all_blocks_[counter_++];
    //value_type* ret = free_blocks_.back();
    //free_blocks_.pop_back();
    //return ret;
  }
 protected:
  std::vector<value_type*> all_blocks_;
  size_t pool_size_ = 0;
  size_t counter_ = 0;
};

template <typename T>
class MemoryPool2 {
  typedef T value_type;
 public:
  MemoryPool2(size_t block_size) {
    for (size_t i = 0; i < block_size; i++) {
      all_blocks_.push(new value_type);
    }
  }
  ~MemoryPool2() {
    while (!all_blocks_.empty()) {
      delete all_blocks_.top();
      all_blocks_.pop();
    }
  }
  void Init() {
    free_blocks_ = all_blocks_;
  }
  value_type* Alloc() {
    if (free_blocks_.empty()) {
      return NULL;
    }
    value_type* ret = free_blocks_.top();
    free_blocks_.pop();
    return ret;
  }
 protected:
  std::stack<value_type*> free_blocks_;
  std::stack<value_type*> all_blocks_;
};

struct A {
  void Clear() {
    a = {0};
  }
  std::array<int, 100> a {{0}};
};

TEST(TEST, MemoryPoolTest) {
  MemoryPool<struct A> pool(200000);
  struct A* p = nullptr;
  int t = 0;
  t -= ::toft::GetTimeStampInUs();
  pool.Init();
  for (int i = 200000; i > 0; --i) {
    p = pool.Alloc();
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "vector pool total cost: " << t * 1.0/1000 << "ms";

  MemoryPool2<struct A> pool2(200000);
  t = 0;
  t -= ::toft::GetTimeStampInUs();
  pool2.Init();
  for (int i = 200000; i > 0; --i) {
    p = pool2.Alloc();
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "stack pool total cost: " << t * 1.0/1000 << "ms";
  LOG(ERROR) << "p: " << p;

  std::vector<struct A*> vec;
  vec.reserve(200000);
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
  LOG(ERROR) << "no pool total cost: " << t * 1.0/1000 << "ms";
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
  LOG(ERROR) << "Don't reserve total cost: " << t * 1.0/1000 << "ms";

  t = 0;
  vec.reserve(200000);
  t -= ::toft::GetTimeStampInUs();
  for (int i = 200000; i > 0; --i) {
    vec.push_back(0);
  }
  t += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "Reserve total cost: " << t * 1.0/1000 << "ms";
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
