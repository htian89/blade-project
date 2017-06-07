#include "a.h"
#include<iostream>

int A::a = 0;

void cf() {
  A::a = 2;
  std::cout << &A::a << ":" << A::a << std::endl;
}
