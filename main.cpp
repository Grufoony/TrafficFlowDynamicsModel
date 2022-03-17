#include "Graph.hpp"
#include "Vehicle.hpp"

#include <chrono>
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
    Vehicle::addVehicleType(0, 3);
    break;

  case 3:
    Vehicle::addVehicleType(argv[2]);
    break;

  default:
    return EXIT_FAILURE;
    break;
  }
  auto g = Graph(argv[1]);
  g.print();
  g.createTransMatrix();
  for (int i = 0; i < Vehicle::getNVehicleType(); ++i) {
    std::cout << "-------------------------------------------------------------"
                 "--------";
    std::cout << "From " << Vehicle::getVehicleType(i)->getSource() << " to "
              << Vehicle::getVehicleType(i)->getDestination() << '\n';
    for (auto temp : Vehicle::getVehicleType(i)->getTransMatrix()) {
      for (auto it : temp) {
        std::cout << it << '\t';
      }
      std::cout << '\n';
    }
  }

  // ending clock adn terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}