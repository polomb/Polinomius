#include <iostream>
#include "UnsortedMap.h"

int main() {
  UnsortedMap<int, std::string> map;

  map.insert(10, "ten");
  map.insert(2, "two");
  map[7] = "seven";

  std::cout << "UnsortedMap contents (in insertion order):\n";
  for (auto it = map.begin(); it != map.end(); ++it) {
    std::cout << it->first << " -> " << it->second << '\n';
  }

  std::cout << "\nfind(2): " << map.find(2)->second << '\n';
  std::cout << "size: " << map.size() << '\n';
  return 0;
}
