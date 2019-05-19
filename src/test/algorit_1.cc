#include <iostream>
#include <string>

void Count(int array[]) {
  int i = 0;
  int temp = 0;
  do {
    if (array[temp] <= 0) {
      --array[temp];
      ++i;
      temp = i;
    } else {
      temp = array[temp] - 1;
      array[temp] = 0;
    }
    std::cout << array[temp] << std::endl;
  } while (i < sizeof(array)/sizeof(*array));
}

int main(int argc, char const* argv[])
{
  int array[10] = {8, 4, 2, 8, 6, 6, 7, 3, 1, 8};
  Count(array);
  for (int i = 0; i < sizeof(array) / sizeof(*array); i++) {
    if (array[i] < 0 ) {
      continue;
    }
    std::cout << i + 1 << " : " << array[i] << std::endl;
  }
  return 0;
}

