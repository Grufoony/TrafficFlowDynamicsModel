#include "Graph.hpp"
#include "Vehicle.hpp"

#include <iostream>

int main() {
  Vehicle::addVehicleType(0, 4);
  Vehicle::addVehicleType(1, 4);
  Graph g("matrix.dat");
  g.addVehicle(1);
  g.test(0, 1);
  return 0;
}