#include "network.hpp"
// #include "street.hpp"

#include <iostream>

int main() {
  auto network = net::Network(3, 3);
  std::cout << network(3, 4).getVelocity() << '\n';
  return 0;
}