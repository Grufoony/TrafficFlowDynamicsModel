#include "Graph.hpp"
#include "Vehicle.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

void printExeTime(std::chrono::high_resolution_clock::duration interval) {
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(interval);
  std::cout << "--------------------------------------" << '\n';
  std::cout << "Execution time: " << duration.count() << " ms" << '\n';
  std::cout << "--------------------------------------" << '\n';
}

int main(int argc, char **argv) {
  typedef std::chrono::high_resolution_clock Clock;
  auto start = Clock::now();
  // clock has started

  switch (argc) {
  case 2:
    Vehicle::addVehicleType(0, 8);
    // Vehicle::addVehicleType(5, 2);
    break;

  case 3:
    Vehicle::addVehicleType(argv[2]);
    break;

  default:
    return EXIT_FAILURE;
    break;
  }
  auto g = Graph(argv[1]);
  g.setTemperature(1e5);
  g.createTransMatrix();
  // g.print();
  // for (int i = 0; i < Vehicle::getNVehicleType(); ++i) {
  //   std::cout <<
  //   "-------------------------------------------------------------"
  //                "--------";
  //   std::cout << "From " << Vehicle::getVehicleType(i)->getSource() << " to "
  //             << Vehicle::getVehicleType(i)->getDestination() << '\n';
  //   for (auto temp : Vehicle::getVehicleType(i)->getTransMatrix()) {
  //     for (auto it : temp) {
  //       std::cout << std::setprecision(2) << it << '\t';
  //     }
  //     std::cout << '\n';
  //   }
  // }
  g.addVehicle(0);
  // g.addVehicle(1);
  g.test();
  for (int i = 0; i < 10; ++i) {
    g.evolve(1);
    g.test();
  }
  g.evolve(1);
  g.test();

  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}
