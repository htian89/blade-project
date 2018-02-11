// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>

class StringTB: public ::testing::Test {
 protected:
  StringTB() {
  }
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
  bool Less() {
    return false == false;
  }
  std::string test_str_;
};

TEST_F(StringTB, EraseLastSpace) {
  test_str_ = "Hello World ";
  test_str_.erase(test_str_.find_last_not_of(" ") + 1);
  if (true)
    ASSERT_EQ("Hello World", test_str_);
    ASSERT_EQ("Hello World", test_str_);

  ASSERT_EQ(true, Less());
}

TEST_F(StringTB, Other) {
  uint32_t i = 100;
  uint32_t j = 1000;
  int32_t k = i - j;
  ASSERT_EQ(-900, k);
}

TEST_F(StringTB, Capacity) {
  std::string test_str;
  ASSERT_EQ(0, test_str.capacity());
  test_str.reserve(2);
  ASSERT_EQ(2, test_str.capacity());
  test_str.clear();
  ASSERT_EQ(2, test_str.capacity());
  std::string test_str2;
  test_str2.reserve(5);
  ASSERT_EQ(5, test_str2.capacity());
  test_str = test_str2;
  ASSERT_EQ(5, test_str.capacity());
  test_str.clear();
  ASSERT_EQ(0, test_str.capacity());
  test_str += "1234512345";
  ASSERT_EQ(10, test_str.capacity());
  test_str += "12345";
  ASSERT_EQ(20, test_str.capacity());
  test_str.clear();
  ASSERT_EQ(20, test_str.capacity());
}
