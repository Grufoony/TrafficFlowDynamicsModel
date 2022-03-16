#include "Graph.hpp"
#include "Vehicle.hpp"

#include <chrono>
#include <iostream>

int main() {
  auto start = std::chrono::high_resolution_clock::now();

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

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "--------------------------------------" << '\n';
  std::cout << "Execution time: " << duration.count() << " ms" << '\n';
  std::cout << "--------------------------------------" << '\n';
  return 0;
}