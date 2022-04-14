#include "Graph.hpp"
#include "Vehicle.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

void printExeTime(std::chrono::high_resolution_clock::duration interval) {
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(interval);
  std::cout << "-------------------------" << '\n';
  std::cout << '|' << "Execution time: " << duration.count()
            << std::setprecision(4) << " ms" << '|' << '\n';
  std::cout << "-------------------------" << '\n';
}

void printTransMatrices() {
  for (int i = 0; i < Vehicle::getNVehicleType(); ++i) {
    std::cout << "-------------------------------------------------------------"
                 "--------";
    std::cout << "From " << Vehicle::getVehicleType(i)->getSource() << " to "
              << Vehicle::getVehicleType(i)->getDestination() << '\n';
    for (auto temp : Vehicle::getVehicleType(i)->getTransMatrix()) {
      for (auto it : temp) {
        std::cout << std::setprecision(2) << it << '\t';
      }
      std::cout << '\n';
    }
  }
}

int main(int argc, char **argv) {
  std::string const OUT_FORMAT = ".dat";
  std::string const OUT_FOLDER = "./img/data/";

  typedef std::chrono::high_resolution_clock Clock;
  auto start = Clock::now();
  // clock has started

  auto g = Graph(argv[1]);
  std::ofstream fOut;
  auto const rdbufBackup = std::cout.rdbuf();
  int dVehicle;

  switch (argc) {
  case 2:
    Vehicle::addVehicleType(0, 8);
    Vehicle::addVehicleType(5, 2);
    break;

  case 3:
    Vehicle::addVehicleType(argv[2]);
    break;

  case 4:
    Vehicle::addVehicleType(argv[2]);
    g.addRndmVehicles(std::stoi(argv[3]));
    break;

  case 5:
    Vehicle::addVehicleType(argv[2]);
    g.setTemperature(std::stod(argv[3]));
    g.addRndmVehicles(std::stoi(argv[4]));
    break;

  case 6:
    Vehicle::addVehicleType(argv[2]);
    g.setTemperature(std::stod(argv[3]));
    dVehicle = std::stoi(argv[4]);
    g.createTransMatrix();
    g.fprint(true);

    // g.evolve(100);
    // g.test();
    for (int t = 0; t < std::stoi(argv[5]); ++t) {
      auto out = OUT_FOLDER + std::to_string(t) + OUT_FORMAT;
      fOut.open(out);
      std::cout.rdbuf(fOut.rdbuf());
      std::cout << "source" << '\t' << "target" << '\t' << "load" << '\t' << "x"
                << '\n';
      if (t < 50) {
        g.evolve(dVehicle);
      } else {
        g.evolve();
      }
      g.fprintDistribution(33);
      g.test();
      fOut.close();
    }
    std::cout.rdbuf(rdbufBackup);
    break;

  default:
    fOut.close();
    return EXIT_FAILURE;
    break;
  }

  // fOut.close();

  // g.setTemperature(3000);
  // g.createTransMatrix();
  // g.fprint(true);
  // for (int t = 0; t < 1e3; ++t) {
  //   std::cout << "Time: " << t << '\n';
  //   g.evolve(1);
  //   g.printStreets();
  // }
  // g.evolve(1);
  // g.test();

  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}

// TODO:
// - implement time penalty for the vehicles
