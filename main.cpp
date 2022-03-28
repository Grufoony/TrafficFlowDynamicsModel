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
    Vehicle::addVehicleType(0, 8);
    Vehicle::addVehicleType(5, 2);
    break;

  case 3:
    Vehicle::addVehicleType(argv[2]);
    break;

  default:
    return EXIT_FAILURE;
    break;
  }
  auto g = Graph(argv[1]);
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
  //       std::cout << it << '\t';
  //     }
  //     std::cout << '\n';
  //   }
  // }
  g.addVehicle(0);
  g.addVehicle(1);
  g.test();
  for (int i = 0; i < 4; ++i) {
    g.evolve(1);
    g.test();
  }
  // g.evolve(1);
  // g.test();

  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}

// dikjstra algorithm
// int Graph::dijkstra(int const src, int const dst) {
//   std::vector<int> dist;
//   std::vector<int> prev;
//   std::vector<bool> visited;
//   for (int i = 0; i < _n; ++i) {
//     dist.push_back(std::numeric_limits<int>::max());
//     prev.push_back(-1);
//     visited.push_back(false);
//   }
//   dist.at(src) = 0;
//   prev.at(src) = src;
//   visited.at(src) = true;
//   for (int i = 0; i < _n - 1; ++i) {
//     int u = minDist(dist, visited);
//     visited.at(u) = true;
//     for (int v = 0; v < _n; ++v) {
//       if (!visited.at(v) && _adjMatrix.at(u).at(v) > 0) {
//         if (dist.at(u) + _adjMatrix.at(u).at(v) < dist.at(v)) {
//           dist.at(v) = dist.at(u) + _adjMatrix.at(u).at(v);
//           prev.at(v) = u;
//         }
//       }
//     }
//   }
//   return dist.at(dst);
// }

// minDist
// int minDist(std::vector<int> const &dist, std::vector<bool> const &visited) {
//   int min = std::numeric_limits<int>::max();
//   int min_index = -1;
//   for (int i = 0; i < dist.size(); ++i) {
//     if (!visited.at(i) && dist.at(i) < min) {
//       min = dist.at(i);
//       min_index = i;
//     }
//   }
//   return min_index;
// }
