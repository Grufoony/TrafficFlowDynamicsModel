#include "network.hpp"

#include <iostream>

int main() {
  auto network = net::Network(3, 3);
  network.addVehicle(0);
  // std::cout << network(3, 2).getVelocity() << '\n';
  network.test();
  return 0;
}