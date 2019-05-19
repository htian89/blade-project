#include <iostream>

#include "thirdparty/gtest/gtest.h"
#include "toft/system/threading/thread_pool.h"
#include "toft/system/time/posix_time.h"
#include "toft/system/atomic/type.h"

const uint8_t PTHREAD_NUM = 20;

using namespace std;
using namespace toft;
class ThreadDemo : public ::testing::Test {
 public:
  ThreadDemo() {
    Init();
  }
  ~ThreadDemo() {
    delete thread_pool_;
  }
  void CallBack(uint8_t& thread_num) {
    uint8_t counter = AtomicIncrement(&counter_);
    cout << "thread: " << (unsigned)counter << endl;
    sleep(1);
    if (counter == thread_num) {
      cout << "set: " << (u_int64_t)thread_num << endl;
      wait_event_.Set();
    }
  }
  void Init() {
    thread_pool_ = new ThreadPool(PTHREAD_NUM);
  }
  void Run() {
    for (int i = 0; i < PTHREAD_NUM; i++) {
      Closure<void()>* task =
          NewClosure(this, &ThreadDemo::CallBack, PTHREAD_NUM);
      thread_pool_->AddTask(task);
    }
    timespec time_begin;
    RelativeMilliSecondsToAbsolute(0, &time_begin);
    wait_event_.Wait();
    timespec time_end;
    RelativeMilliSecondsToAbsolute(0, &time_end);
    cout << "Wait: " << (time_end.tv_sec - time_begin.tv_sec) << "s"<< endl;
  }

 public:
  ThreadPool* thread_pool_;
  AutoResetEvent wait_event_;
  uint8_t counter_;
};

TEST_F(ThreadDemo, First) {
  timespec time_begin;
  RelativeMilliSecondsToAbsolute(0, &time_begin);
  Run();
  timespec time_end;
  RelativeMilliSecondsToAbsolute(0, &time_end);
  cout << "Completed: " << (time_end.tv_sec - time_begin.tv_sec) << "s"<< endl;
}