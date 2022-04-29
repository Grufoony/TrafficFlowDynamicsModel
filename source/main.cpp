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
      std::chrono::duration_cast<std::chrono::milliseconds>(interval).count();
  std::string unit = " ms";
  if (duration > 1e4) {
    duration =
        std::chrono::duration_cast<std::chrono::seconds>(interval).count();
    unit = " s";
  }
  std::cout << "┌─────────────────────────┐" << '\n';
  std::cout << "│ " << std::setw(10) << "Execution time: " << duration
            << std::setprecision(4) << unit << " │" << '\n';
  std::cout << "└─────────────────────────┘" << '\n';
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

void printLoadingBar(int const i, int const n) {
  std::cout << "Loading: " << std::setprecision(3) << std::fixed
            << (i * 100. / n) << "%" << '\r';
  std::cout.flush();
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
    g.addVehiclesUniformly(dVehicle);
    for (int t = 0; t < std::stoi(argv[5]); ++t) {
      printLoadingBar(t, std::stoi(argv[5]));
      g.evolve();
      // if (t < 1000) {
      //   auto out = OUT_FOLDER + std::to_string(t) + OUT_FORMAT;
      //   fOut.open(out);
      //   std::cout.rdbuf(fOut.rdbuf());
      //   std::cout << "source" << '\t' << "target" << '\t' << "load" << '\t'
      //             << "x" << '\n';
      //   g.test();
      //   fOut.close();
      //   std::cout.rdbuf(rdbufBackup);
      // }
      if (t % 250 == 0) {
        g.fprintDistribution(15);
      }
    }
    break;

  default:
    fOut.close();
    return EXIT_FAILURE;
    break;
  }

  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}
