#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

class TestBench {
  std::array<int, 5> array1_ {{0,1,2,3}};
  int int_a_ = 0;
  std::vector<int> vector1_ {0,1,2,3};
  std::map<int, int> map1_ {{0, 1},{1, 2},{2,3}};
 public:
  TestBench();
  void DefaultValueTest(int64_t& a = 1) {
      std::cout << a << std::endl;
  }
  void Print() {
    for (auto& i : array1_) {
      std::cout << i;
    }
    std::cout << std::endl;
    for (auto& i : vector1_) {
      std::cout << i;
    }
    std::cout << std::endl;
    for (auto& it : map1_) {
      std::cout << it.first << ":" << std::get<1>(it) << ", ";
    }
    std::cout << std::endl;
  }
};
TestBench::TestBench() {}

int main(int argc, char const* argv[])
{
  TestBench tb;
  //tb.Print();
  tb.DefaultValueTest(2);
  tb.DefaultValueTest();
  return 0;
}
