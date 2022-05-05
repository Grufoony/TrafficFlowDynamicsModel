#include "Graph.hpp"
#include "Vehicle.hpp"

#include <chrono>
#include <filesystem>
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
  std::cout << "│ " << std::setw(10) << "Execution time: " << std::setw(5)
            << duration << std::setprecision(4) << unit << " │" << '\n';
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
  std::cout << "Loading: " << std::setprecision(2) << std::fixed
            << (i * 100. / n) << "%" << '\r';
  std::cout.flush();
}

void clearDir(std::string const &dir) {
  std::cout << "Cleaning directory: " << dir << '\n';
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);
  std::cout << "Directory cleaned." << '\n';
}

int main(int argc, char **argv) {
  std::string const OUT_FORMAT = ".dat";
  std::string const DATA_FOLDER = "./data/";
  std::string const OUT_FOLDER = "./img/data/";

  typedef std::chrono::high_resolution_clock Clock;
  auto start = Clock::now();
  // clock has started

  auto g = Graph(argv[1]);
  std::ofstream fOut;
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
    std::cout << "Setting up the simulation..." << '\n';
    Vehicle::addVehicleType(argv[2]);
    g.setTemperature(std::stod(argv[3]));
    dVehicle = std::stoi(argv[4]);
    g.createTransMatrix();
    std::cout << "Done." << '\n';
    g.fprint(true);
    clearDir(DATA_FOLDER);
    clearDir(OUT_FOLDER);
    fOut.open("./prova.dat");
    for (int t = 0; t < std::stoi(argv[5]); ++t) {
      printLoadingBar(t, std::stoi(argv[5]));
      if (t % 250 == 0) {
        if (t < 1250)
          g.addVehiclesUniformly(dVehicle / 5);
        g.fprintHistogram(DATA_FOLDER, 15);
        g.fprintDistribution(DATA_FOLDER, "u/q");
        g.fprintDistribution(DATA_FOLDER, "q/k");
        g.fprintDistribution(DATA_FOLDER, "u/k");
      }
      if (t % 50 == 0) {
        g.fprintActualState(fOut.rdbuf());
      }
      if (t < 5e3) {
        g.evolve();
      } else {
        g.evolve(false);
      }
    }
    break;

  default:
    return EXIT_FAILURE;
    break;
  }
  fOut.close();
  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}
