#include <vector>
#include <thread>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "toft/system/time/timestamp.h"
#include "toft/system/threading/rwlock.h"

TEST(TEST, time_test) {
  int t = 0;
  for (int i = 1e4; i > 0; --i) {
    t -= ::toft::GetTimeStampInUs();
    t += ::toft::GetTimeStampInUs();
  }
  LOG(ERROR) << "total cost: " << t * 1.0/1000 << "ms";
}

static toft::RwLock g_rw_lock;
static int global_counter = 0;
static void FuncAdd() {
  toft::RwLock::ReaderLocker lock(&g_rw_lock);
  ++global_counter;
}
static void FuncThread() {
  for (int i = 0; i < 10000; ++i) {
    FuncAdd();
  }
}

TEST(TEST, rwlock_test) {
  int time_cost;
  time_cost -= ::toft::GetTimeStampInUs();
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.push_back(std::thread(FuncThread));
  }
  for (auto&& thread : threads) {
    thread.join();
  }
  time_cost += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << time_cost << "us";
}

static void FuncAdd2() {
  toft::RwLock::WriterLocker lock(&g_rw_lock);
  ++global_counter;
}
static void FuncThread2() {
  for (int i = 0; i < 10000; ++i) {
    FuncAdd2();
  }
}

TEST(TEST, nolock_test) {
  int time_cost;
  time_cost -= ::toft::GetTimeStampInUs();
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.push_back(std::thread(FuncThread2));
  }
  for (auto&& thread : threads) {
    thread.join();
  }
  time_cost += ::toft::GetTimeStampInUs();
  LOG(ERROR) << "total cost: " << time_cost << "us";
}
