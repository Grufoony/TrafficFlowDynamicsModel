#include "Street.hpp"

#include <iostream>

int main() {
  Street s(1, 0);
  auto x = s.getNodes();
  std::cout << x[0] << '\n';
  std::cout << x[1] << '\n';
  return 0;
}