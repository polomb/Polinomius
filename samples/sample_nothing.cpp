#include <iostream>

#include "AVLTree.h"

int main() {
  AVLTree<int> tree;
  for (int key : {10, 5, 15, 3, 7, 12, 20}) {
    tree.insert(key);
  }

  std::cout << "AVL in-order traversal:\n";
  for (int key : tree) {
    std::cout << key << ' ';
  }
  std::cout << "\nheight: " << tree.height() << '\n';

  tree.erase(10);
  std::cout << "after erase(10), find(10): " << tree.find(10) << '\n';
  return 0;
}
