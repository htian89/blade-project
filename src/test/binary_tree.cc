#include <iostream>
#include <string>

typedef struct Btree {
  int data_;
  Btree *lchild_, *rchild_;
};

class BinaryTree {
  public:
   BinaryTree() {

   }
  private:
   Btree* tree_;
};

int main(int argc, char const* argv[])
{

  return 0;
}
