#include "Graph.hpp"
#include "Vehicle.hpp"

#include <iostream>

int main() {
  Vehicle::addVehicleType(0, 3);
  // Vehicle::addVehicleType(5, 3);

  auto g = Graph("matrix.dat");
  // g.addVehicle(0);
  std::cout << "Trip from 0 to 3\n";
  g.print();
  g.createTransMatrix();
  std::cout << "---------------------------------------------------\n";
  for (auto temp : Vehicle::getVehicleType(0)->getTransMatrix()) {
    for (auto it : temp) {
      std::cout << it << '\t';
    }
    std::cout << '\n';
  }

  return 0;
}