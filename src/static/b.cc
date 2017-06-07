#include "a.h"
#include <iostream>

int A::a = 0;

void bf() {
  A::a = 1;
  std::cout << &A::a << ":" << A::a << std::endl;
}
