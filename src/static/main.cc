#include <iostream>
#include "a.h"
void bf();
void cf();

int main(void) {
  bf();
  std::cout << &A::a << ":" << A::a << std::endl;
  cf();
  std::cout << &A::a << ":" << A::a << std::endl;
  return 0;
}
