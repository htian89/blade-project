#include <string>
#include <iostream>
#include <ctime>
#include <cmath>

class HeapSort {
 public:
  HeapSort(size_t length) : length_(length) {
    //depth_ = 1;
    //for (size_t i = 1; i <= length_; i += 2 * depth_) {
    //  ++depth_;
    //}
    depth_ = log(length_) / log(2) + 1;
    heap_ = new int[length_];
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < length_; i++) {
      heap_[i] = rand() % length_;
    }
    std::cout << "Depth: " << depth_ << std::endl;
  }
  void Echo() {
    size_t depth = 1;
    size_t length = 1;
    size_t count = 1;
    for (size_t i = 0; i < length_; i++) {
      if (i >= count) {
        length *= 2;
        count += length;
        std::cout << std::endl;
        depth++;
      }
      for (size_t j = 0; j < 2 * (depth_ - depth + 1); j++) {
        std::cout << " ";
      }
      std::cout << heap_[i];
    }
    std::cout << std::endl;
  }
  void Heapify(size_t index, size_t node_num) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t min = index;
    if (left < node_num && heap_[left] < heap_[min]) {
      min = left;
    }
    if (right < node_num && heap_[right] < heap_[min]) {
      min = right;
    }
    if (min != index) {
      Swap(min, index);
      Heapify(min, node_num);
    }
  }
  void MakeHeap() {
    for (int i = length_ / 2 - 1; i >= 0; --i) {
      Heapify(i, length_);
    }
  }
  void Sort() {
    for (size_t i = 0; i < length_; i++) {
      Swap(0, length_ - i - 1);
      Heapify(0, length_ - i - 1);
      std::cout << heap_[length_ - i -1] << " ";
    }
    std::cout << std::endl;
  }
 private:
  void Swap(size_t a, size_t b) {
    int temp = heap_[b];
    heap_[b] = heap_[a];
    heap_[a] = temp;
    //std::cout << heap_[a] << ":" << heap_[b] << std::endl;
  }
  size_t length_;
  int *heap_;
  size_t depth_;
};
int main(int argc, char const* argv[])
{
  HeapSort hs(atoi(argv[1]));
  hs.Echo();
  hs.MakeHeap();
  hs.Echo();
  hs.Sort();
  hs.Echo();
  return 0;
}
