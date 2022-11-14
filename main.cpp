#include "./TrafficModel/Graph.hpp"
#include "./TrafficModel/Vehicle.hpp"

#include <chrono>
#include <cmath>
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
    Vehicle::getVehicleType(i)->getTransMatrix().print();
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
  std::string const DATA_FOLDER = "./temp_data/";
  std::string const IMG_FOLDER = "./temp_img/data/";

  typedef std::chrono::high_resolution_clock Clock;
  auto start = Clock::now();
  // clock has started

  auto g = Graph(argv[1]);
  // std::ofstream fOut;
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
    // argv = matrix_file vehicle_type_file temperature n_vehicles time_limit
    std::cout << "Setting up the simulation..." << '\n';
    Vehicle::addVehicleType(argv[2]);
    g.setTemperature(std::stod(argv[3]));
    g.updateTransMatrix();
    dVehicle = std::stoi(argv[4]);
    std::cout << "Done." << '\n';
    g.fprint(true);
    clearDir(DATA_FOLDER);
    clearDir(IMG_FOLDER);
    // TEST
    // printTransMatrices();
    // adiabatic analysis
    for (int t = 0; t < std::stoi(argv[5]); ++t) {
      printLoadingBar(t, std::stoi(argv[5]));
      if (t % 100 == 0) {
        // g.fprintHistogram(DATA_FOLDER, "density", 10, "latex");
        // g.fprintDistribution(DATA_FOLDER, "u/q");
        // g.fprintDistribution(DATA_FOLDER, "q/k");
        // g.fprintDistribution(DATA_FOLDER, "u/k");
        // g.fprintActualState(DATA_FOLDER, "q/k");
        // g.fprintActualState(DATA_FOLDER, "u/k");
        // g.fprintTimeDistribution(DATA_FOLDER, "k", 0.);
        // g.fprintTimeDistribution(DATA_FOLDER, "q", 0.);
        // g.fprintTimeDistribution(DATA_FOLDER, "u", 0.);
        g.fprintHistogram(DATA_FOLDER, "traveltime", 20, "latex");
        g.fprintHistogram(DATA_FOLDER, "traveltime", 20, "root");
      }
      // if (t == 3000 || t == 5200 || t == 11400) {
      //   g.fprintVisual("./temp_img/data/");
      // }
      // constant analysis
      if (t < 12e3 && t % 60 == 0 && t != 0) {
        g.addVehiclesUniformly(dVehicle / 1.125);
        g.addRndmVehicles(dVehicle);
      }
      // peaked analysis
      // if (t < 500 && t % 50 == 0 && t != 0) {
      //   g.addVehiclesUniformly(dVehicle / 10);
      // }
      // if (t == 0) {
      //   g.fprintVisual(IMG_FOLDER);
      // }
      // if (t % 240 == 0 && t != 0) {
      //   g.test();
      // }
      // constant analysis
      // if (t < 8e3) {
      //   g.evolve();
      // } else {
      //   g.evolve(false);
      // }
      // peaked analysis
      // if (t < 0.4e4) {
      //   g.evolve();
      // } else {
      //   g.evolve(false);
      // }
      g.evolve(false);
    }
    break;

  case 7:
    // argv = matrix_file vehicle_type_file temperature amplitude period
    // time_limit
    std::cout << "Setting up the simulation..." << '\n';
    Vehicle::addVehicleType(argv[2]);
    g.setTemperature(std::stod(argv[3]));
    g.updateTransMatrix();
    dVehicle = std::stoi(argv[4]);
    std::cout << "Done." << '\n';
    g.fprint(true);
    clearDir(DATA_FOLDER);
    clearDir(IMG_FOLDER);
    for (int t = 0; t < std::stoi(argv[6]); ++t) {
      printLoadingBar(t, std::stoi(argv[6]));
      if ((t % 300 == 0) && (t < (std::stoi(argv[5]) * 3 / 2))) {
        dVehicle = std::abs(std::stoi(argv[4]) *
                            std::sin(2 * M_PI * t / std::stoi(argv[5])));
        if (t > 1.62e4 && t < 2.43e4) {
          dVehicle *= 1.125;
        }
        g.addVehiclesUniformly(dVehicle);
        // g.addRndmVehicles(dVehicle / 2);
      }
      if (t % 900 == 0) {
        // g.fprintHistogram(DATA_FOLDER, "density", 15);
        // g.fprintDistribution(DATA_FOLDER, "u/q");
        // g.fprintDistribution(DATA_FOLDER, "q/k");
        // g.fprintDistribution(DATA_FOLDER, "u/k");
        // g.fprintActualState(DATA_FOLDER, "q/k");
        // g.fprintActualState(DATA_FOLDER, "u/k");
        g.fprintTimeDistribution(DATA_FOLDER, "k", 4.);
        g.fprintTimeDistribution(DATA_FOLDER, "q", 4.);
        g.fprintTimeDistribution(DATA_FOLDER, "u", 4.);
      }
      if (t % 100 == 0 && t != 0) {
        g.fprintHistogram(DATA_FOLDER, "traveltime", 100, "latex");
        g.fprintHistogram(DATA_FOLDER, "traveltime", 100, "root");
      }
      g.evolve(false);
    }
    break;

  default:
    return EXIT_FAILURE;
    break;
  }
  // ending clock and terminate
  auto stop = Clock::now();
  printExeTime(stop - start);
  return 0;
}
