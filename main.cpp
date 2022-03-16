#include "Graph.hpp"
#include "Vehicle.hpp"

#include <iostream>

int main() {
  Vehicle::addVehicleType(0, 3);

  auto g = Graph("matrix.dat");
  g.addVehicle(0);
  std::cout << "Trip from 0 to 3\n";
  g.print();
  g.createTransMatrix();

  return 0;
}