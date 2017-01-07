// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>

class StringTB: public ::testing::Test {
  protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
    std::string test_str_;
};

TEST_F(StringTB, EraseLastSpace) {
  test_str_ = "Hello World ";
  test_str_.erase(test_str_.find_last_not_of(" ") + 1);
  ASSERT_EQ("Hello World", test_str_);
}