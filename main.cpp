#include "Graph.hpp"
#include "Vehicle.hpp"

#include <iostream>

int main() {
  Vehicle::addVehicleType(0, 3);

  // std::vector<double> a = {2, 0};
  // std::vector<double> b = {3, 1};
  // std::vector<std::vector<double>> c;
  // c.push_back(a);
  // c.push_back(b);
  // Vehicle::getVehicleType(0).setTransMatrix(c);
  // auto f = Vehicle::getVehicleType(0).getTransMatrix();
  // for (auto it1 : f) {
  //   for (auto it : it1)
  //     std::cout << it << '\n';
  // }
  auto g = Graph("matrix.dat");
  g.addVehicle(0);
  g.createTransMatrix();

  std::cout << Vehicle::getVehicleType(0)->getTransMatrix().size() << '\n';
  for (auto it1 : Vehicle::getVehicleType(0)->getTransMatrix()) {
    for (auto it : it1)
      std::cout << it << '\n';
  }

  // g.print();

  return 0;
}