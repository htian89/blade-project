// Author: Tian hao <tianhao3@jd.com>
//
// Description: Stl library test bench

#include <gtest/gtest.h>

struct a1 {
  char a;
  double b;
  char c;
};

struct a2 {
  char a;
  char c;
  int b;
};

struct a3 {
  int b;
  char a;
};

struct a4 {
  char a;
  bool b;
};

TEST(TestSizeOf, Struct) {
  EXPECT_EQ(sizeof(a1), 24);
  EXPECT_EQ(sizeof(a2), 8);
  EXPECT_EQ(sizeof(a3), 8);
  EXPECT_EQ(sizeof(a4), 1);
}
