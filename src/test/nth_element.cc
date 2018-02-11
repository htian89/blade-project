#include <algorithm>
#include <vector>
#include <list>
#include <iterator>
#include <iostream>

template<class T, class Compare>
void NthElement(T first, T nth, T last, Compare *comp) {
  size_t num = nth - first;
  size_t len = last - first;
  for (auto it = first; it != nth; ++it) {
    size_t counter = 0;
    do {
      counter = 0;
      for (auto it_cmp = it + 1; it_cmp != first + len; ++it_cmp) {
        if (comp(*it_cmp, *it)) {
          ++counter;
          if (counter == num) {
            --len;
            std::iter_swap(it, first + len);
            break;
          }
        }
      }
    } while (counter == num);
    --num;
  }
}

bool SortDecision(uint64_t first, uint64_t second) {
  return first < second;
}

int main(int argc, char const* argv[])
{
  std::vector<uint64_t> tv = {8,2,10,3,7,6,1};
  for (auto it = tv.begin(); it != tv.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  NthElement(tv.begin(), tv.begin() + 4, tv.end(), SortDecision);
  for (auto it = tv.begin(); it != tv.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  return 0;
}
