#include "network.hpp"

#include <iostream>

int main() {
  auto test = net::Street();
  test.addVehicle();
  test.addVehicle();
  test.addVehicle();
  std::cout << test.getVelocity() << '\n';
  auto network = net::Network(4, 5);
  return 0;
}