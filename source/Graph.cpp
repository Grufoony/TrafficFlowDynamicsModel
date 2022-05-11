#include "Graph.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

double constexpr TEMP_NORM = 273.15e-6;

// function for dijkstra
int minDistance(std::vector<int> const &dist, std::vector<bool> const &sptSet,
                int const _n) {
  // Initialize min value
  int min = std::numeric_limits<int>::max(), min_index = -1;

  for (int v = 0; v < _n; ++v)
    if (!sptSet.at(v) && dist.at(v) <= min)
      min = dist.at(v), min_index = v;

  return min_index;
}

void normalizeVec(std::vector<double> &vec) {
  double sum = 0.;
  for (auto it : vec)
    sum += it;
  if (static_cast<int>(sum) == 0)
    return;
  for (auto &it : vec)
    it = it / sum;
}

void normalizeMat(std::vector<std::vector<double>> &mat) {
  for (auto &it : mat)
    normalizeVec(it);
}

// using Dijkstra to calculate distance
int Graph::_minDistance(int const src, int const dst) const {
  std::vector<int> dist; // The output array. dist.at(i) will hold the shortest
  // distance from src to i

  std::vector<bool>
      sptSet; // sptSet.at(i) will be true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized

  // Initialize all distances as INFINITE and stpSet as false
  for (int i = 0; i < _n; ++i)
    dist.push_back(std::numeric_limits<int>::max()), sptSet.push_back(false);

  // Distance of source vertex from itself is always 0
  dist.at(src) = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < _n - 1; ++count) {
    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in the first iteration.
    int u = minDistance(dist, sptSet, _n);

    // Mark the picked vertex as processed
    sptSet.at(u) = true;

    // Update dist value of the adjacent vertices of the picked vertex.
    for (int v = 0; v < _n; ++v) {

      // Update dist.at(v) only if is not in sptSet, there is an edge from
      // u to v, and total weight of path from src to v through u is
      // smaller than current value of dist.at(v)
      auto lenght = _adjMatrix.at(u).at(v);
      if (lenght > std::numeric_limits<double>::epsilon())
        lenght /= this->_getStreetMeanVelocity(_findStreet(u, v));
      auto time = static_cast<int>(lenght);
      if (!sptSet.at(v) && time &&
          dist.at(u) != std::numeric_limits<int>::max() &&
          dist.at(u) + time < dist.at(v))
        dist.at(v) = dist.at(u) + time;
    }
  }
  return dist.at(dst);
}

std::vector<int> Graph::_nextStep(int const src, int const dst) {
  auto const &row = _adjMatrix.at(src);
  auto const min = _minDistance(src, dst);
  std::vector<int> _nextStep;
  for (int i = 0; i < static_cast<int>(row.size()); ++i) {
    auto lenght = row.at(i);
    if (lenght > std::numeric_limits<double>::epsilon()) {
      auto time = static_cast<int>(
          lenght / this->_getStreetMeanVelocity(_findStreet(src, i)));
      if (_minDistance(i, dst) == (min - time))
        _nextStep.push_back(i);
    }
  }
  return _nextStep;
}

void Graph::_evolve(bool reinsert) {
  // keep in memory the previous state of the streets
  int i = 0;
  for (auto const &street : _streets) {
    _vehiclesOnStreet.at(i) = street->getNVehicles();
    i++;
  }
  // random initializations
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<> dist(0., 1.);
  // cicling through all the vehicles
  for (auto const &vehicle : _vehicles) {
    auto const &trans_vec = vehicle->getVehicleType()->getTransMatrix().at(
        vehicle->getPosition()); // obtain the line with trans probabilities
    auto threshold = 0.;
    auto const p = dist(rng);
    auto timePenalty = vehicle->getTimePenalty();
    if (timePenalty > 0) { // check if the vehicle can move
      // if the vahicle cannot move checks if the vehicle could go faster
      auto streetLenght = _streets.at(vehicle->getStreet())->getLenght();
      auto oldTime = static_cast<int>(streetLenght / vehicle->getVelocity());
      auto newTime = static_cast<int>(
          streetLenght / _streets.at(vehicle->getStreet())->getVelocity());
      auto dTime = newTime - oldTime;
      if (dTime < 0) {
        if ((timePenalty + dTime) > 0) {
          vehicle->setTimePenalty(timePenalty + dTime);
        } else {
          vehicle->setTimePenalty(0);
        }
        vehicle->setVelocity(_streets.at(vehicle->getStreet())->getVelocity());
      } else {
        vehicle->setTimePenalty(timePenalty - 1);
      }
    } else {
      for (int i = 0; i < _n; ++i) {
        auto prob = trans_vec.at(i);
        if (vehicle->getPosition() ==
            vehicle->getDestination()) { // check if the vehicle is at the
                                         // destination
          if (!(vehicle->getStreet() < 0)) {
            _streets.at(vehicle->getStreet())->remVehicle();
            vehicle->setStreet(-1);
          }
        } else if (prob > std::numeric_limits<double>::epsilon()) {
          threshold += prob;
          if (p < threshold) {
            // street update
            int streetIndex =
                _findStreet(vehicle->getPosition(), i); // nex street index
            if (!(_streets.at(streetIndex)
                      ->isFull())) { // check if i can move on (street not full)
              if (!(vehicle->getStreet() <
                    0)) { // check if the vehicle is on a street
                _streets.at(vehicle->getStreet())->remVehicle();
              }
              _streets.at(streetIndex)->addVehicle(vehicle);
              break;
            } else {
              vehicle->setVelocity(0.);
            }
          }
        }
      }
    }
  }
  int nVehicles_old = static_cast<int>(_vehicles.size());
  // erase vehicles that have reached their destination
  _vehicles.erase(std::remove_if(_vehicles.begin(), _vehicles.end(),
                                 [](std::shared_ptr<Vehicle> const &vehicle) {
                                   return vehicle->getPosition() ==
                                              vehicle->getDestination() &&
                                          vehicle->getStreet() == -1;
                                 }),
                  _vehicles.end());
  ++_time;
  int dVehicles = nVehicles_old - static_cast<int>(_vehicles.size());
  // add new vehicles
  if (reinsert) {
    this->addRndmVehicles(dVehicles);
  }
}

int Graph::_findStreet(int const src, int const dst) const {
  int i = 0;
  for (auto const &street : _streets) {
    if (street->getOrigin() == src && street->getDestination() == dst)
      return i;
    ++i;
  }
  throw std::runtime_error("Street not found");
}

double Graph::_getStreetMeanVelocity(int const streetIndex) const {
  double vCum = 0;
  int i = 0;
  for (auto const &vehicle : _vehicles) {
    if (vehicle->getStreet() == streetIndex) {
      vCum += vehicle->getVelocity();
      ++i;
    }
  }
  if (i == 0) {
    return _streets.at(streetIndex)->getVMax();
  } else {
    return vCum / i;
  }
}

Graph::Graph(const char *fName) {
  _n = 0;
  std::ifstream data;

  // set database's dimension
  data.open(fName);
  if (!data) {
    throw std::runtime_error("Matrix file does not exist.\n");
  }
  double x;
  bool b;
  while (data >> x) {
    ++_n;
  }
  data.close();
  _n = std::sqrt(_n);

  // import adj matrix from file
  std::cout << "Importing adjacency matrix from file..." << '\n';
  data.open(fName);
  int streetIndex = 0;
  for (int u = 0; u < _n; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(x);
      if (b) {
        _streets.push_back(
            std::make_shared<Street>(Street(u, v, x, streetIndex)));
        ++streetIndex;
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
  std::cout << "Done." << '\n';
  for (int i = 0; i < static_cast<int>(_streets.size()); ++i) {
    _vehiclesOnStreet.push_back(0);
  }
}

Graph::Graph(const char *fName, const char *fCoordinates) {
  _n = 0;
  std::ifstream data;
  // set database's dimension
  data.open(fCoordinates);
  if (!data) {
    throw std::runtime_error("Coordinates file does not exist.\n");
  }
  double x;
  bool b;
  while (data >> x) {
    ++_n;
  }
  data.close();
  _n = _n / 2;

  // import coordinates
  data.open(fCoordinates);
  for (int u = 0; u < 2; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      temp.push_back(x);
    }
    _nodesCoordinates.push_back(temp);
  }
  data.close();

  // import adj matrix from file
  std::cout << "Importing adjacency matrix from file..." << '\n';
  data.open(fName);
  if (!data) {
    throw std::runtime_error("Matrix file does not exist.\n");
  }
  int streetIndex = 0;
  for (int u = 0; u < _n; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(x);
      if (b) {
        _streets.push_back(
            std::make_shared<Street>(Street(u, v, x, streetIndex)));
        ++streetIndex;
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
  std::cout << "Done." << '\n';
}

void Graph::addVehicle(int type) {
  if (type < 0 || !(type < Vehicle::getNVehicleType()))
    throw std::runtime_error("Invalid vehicle type.\n");
  _vehicles.push_back(std::make_shared<Vehicle>(Vehicle(type)));
}

void Graph::addRndmVehicles(int nVehicles) {
  if (nVehicles < 0)
    throw std::invalid_argument(
        "Number of vehicles randomly added must be positive.");
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<> dist(0, Vehicle::getNVehicleType() - 1);
  for (int i = 0; i < nVehicles; ++i) {
    int index = dist(rng);
    this->addVehicle(index);
  }
}

void Graph::addVehiclesUniformly(int n) {
  if (n < 0)
    throw std::invalid_argument(
        "Number of vehicles uniformly added must be positive.");
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<> dist(0,
                                       static_cast<int>(_streets.size() - 1));
  for (int i = 0; i < n; ++i) {
    this->addRndmVehicles(1);
    int index = dist(rng);
    while (_streets.at(index)->isFull()) {
      index = dist(rng);
    }
    _streets.at(index)->addVehicle(_vehicles.back());
  }
}

void Graph::loadVehicles(const char *fName) {
  std::fstream data;
  data.open(fName);
  if (!data) {
    throw std::runtime_error("Vehicles file does not exist.\n");
  }
  // TODO: add vehicles on _streets from file
}

void Graph::setTemperature(double const temperature) {
  if (temperature < 0)
    throw std::invalid_argument("Temperature must be positive.\n");
  _temperature = temperature;
}

void Graph::updateTransMatrix() {
  // function for noise using a kelvin-like temperature normalization
  auto const noise = std::tanh(_temperature * TEMP_NORM);

  for (int index = 0; index < Vehicle::getNVehicleType(); ++index) {
    auto const vehicle = Vehicle::getVehicleType(index);
    int const dst = vehicle->getDestination();
    std::vector<std::vector<double>> matrix;
    // initialize matrix at 0
    for (int i = 0; i < _n; ++i) {
      std::vector<double> temp;
      for (int j = 0; j < _n; ++j) {
        temp.push_back(0.);
      }
      matrix.push_back(temp);
    }
    // setting to 1 the probabilities of correct movements
    for (int i = 0; i < _n; ++i) {
      auto path = _nextStep(i, dst);
      if (path.size() > 0) {
        for (auto &it : path)
          matrix.at(i).at(it) = 1.;
      }
    }
    // setting noise
    for (int i = 0; i < _n; ++i) {
      for (int j = 0; j < _n; ++j) {
        if (_adjMatrix.at(i).at(j) > std::numeric_limits<double>::epsilon() &&
            matrix.at(i).at(j) < 0.1) {
          matrix.at(i).at(j) = noise;
        }
      }
    }
    normalizeMat(matrix);
    vehicle->setTransMatrix(matrix);
  }
}

void Graph::evolve(bool reinsert) { this->_evolve(reinsert); }
void Graph::evolve() { this->_evolve(true); }

void Graph::printMatrix() const noexcept {
  for (auto const &row : _adjMatrix) {
    for (auto const it : row) {
      std::cout << std::setprecision(2) << it << '\t';
    }
    std::cout << '\n';
  }
}

void Graph::print(bool const printGraph) const noexcept {
  std::cout << "-------------------------" << '\n';
  std::cout << "NETWORK INFORMATIONS\n";
  std::cout << "Nodes: " << _n << '\n';
  std::cout << "Temperature: " << _temperature << 'K' << '\n';
  std::cout << "Noise level: " << std::tanh(_temperature * TEMP_NORM) * 1e2
            << '%' << '\n';
  std::cout << "Vehicles: " << _vehicles.size() << '\n';
  for (int type = 0; type < static_cast<int>(Vehicle::getNVehicleType());
       ++type) {
    int nVehicles =
        std::count_if(_vehicles.begin(), _vehicles.end(),
                      [type](std::shared_ptr<Vehicle> const &vehicle) {
                        return vehicle->getType() == type;
                      });
    std::cout << '\t' << "Type " << type << ": " << nVehicles << '\n';
  }
  std::cout << "Streets: " << _streets.size() << '\n';
  if (printGraph) {
    std::cout << "Input graph:\n";
    int i = 0;
    for (auto const &row : _adjMatrix) {
      std::cout << i;
      if (!(_nodesCoordinates.empty())) {
        std::cout << " (" << _nodesCoordinates.at(0).at(i) << ','
                  << _nodesCoordinates.at(1).at(i) << ") ";
      }
      std::cout << "-->";
      int j = 0;
      for (auto const it : row) {
        if (it > std::numeric_limits<double>::epsilon()) {
          std::cout << '\t' << j;
        }
        ++j;
      }
      std::cout << '\n';
      ++i;
    }
  }
  std::cout << "-------------------------" << '\n';
}

void Graph::printStreets() const noexcept {
  int i = 0;
  for (auto const &street : _streets) {
    if (street->getNVehicles() > 0) {
      std::cout << i << '(' << street->getOrigin() << ','
                << street->getDestination() << ')' << ": "
                << street->getNVehicles() << '\t' << std::setprecision(3)
                << street->getVelocity() << '\n';
    }
    ++i;
  }
}

void Graph::fprint(const bool printGraph) const noexcept {
  std::ofstream fOut;
  fOut.open("network_info.txt");
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  this->print(printGraph);
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}

void Graph::fprintVisual(std::string const &out_folder) const noexcept {
  std::ofstream fOut;
  auto out = out_folder + std::to_string(_time) + ".dat";
  fOut.open("network_info.txt");
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  std::cout << "source" << '\t' << "target" << '\t' << "load" << '\t' << "x"
            << '\n';
  for (auto const &street : _streets) {
    std::cout << street->getOrigin() << '\t' << street->getDestination() << '\t'
              << street->getNVehicles() << '\t' << std::setprecision(3)
              << street->getVelocity() << '\n';
  }
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}

void Graph::fprintHistogram(std::string const &out_folder,
                            int const nBins) const noexcept {
  std::ofstream fOut;
  auto out = out_folder + std::to_string(_time) + ".dat";
  fOut.open(out);
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  int n;
  for (int i = 0; i < nBins + 1; ++i) {
    n = std::count_if(_streets.begin(), _streets.end(),
                      [i, nBins](std::shared_ptr<Street> const &street) {
                        return street->getDensity() >= i * (1. / nBins) &&
                               street->getDensity() < (i + 1) * (1. / nBins);
                      });
    std::cout << std::setprecision(3) << i * (1. / nBins) << '\t' << n << '\n';
  }
  std::cout << (nBins + 1.) * (1. / nBins);
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}

void Graph::fprintDistribution(std::string const &outFolder,
                               std::string const &opt) const {
  if (opt == "u/q") {
    std::ofstream fOut;
    auto out = outFolder + std::to_string(_time) + "_u-q.dat";
    fOut.open(out);
    auto const rdbufBackup = std::cout.rdbuf();
    std::cout.rdbuf(fOut.rdbuf());
    for (auto const &street : _streets) {
      auto meanV = this->_getStreetMeanVelocity(street->getIndex());
      if (!(meanV < 0))
        std::cout << meanV * street->getVehicleDensity() * 3.6e3 << '\t'
                  << meanV * 3.6 << '\n';
    }
    std::cout.rdbuf(rdbufBackup);
    fOut.close();
  } else if (opt == "q/k") {
    std::ofstream fOut;
    auto out = outFolder + std::to_string(_time) + "_q-k.dat";
    fOut.open(out);
    auto const rdbufBackup = std::cout.rdbuf();
    std::cout.rdbuf(fOut.rdbuf());
    for (auto const &street : _streets) {
      auto meanV = this->_getStreetMeanVelocity(street->getIndex());
      if (!(meanV < 0))
        std::cout << street->getVehicleDensity() * 1e3 << '\t'
                  << meanV * street->getVehicleDensity() * 3.6e3 << '\n';
    }
    std::cout.rdbuf(rdbufBackup);
    fOut.close();
  } else if (opt == "u/k") {
    std::ofstream fOut;
    auto out = outFolder + std::to_string(_time) + "_u-k.dat";
    fOut.open(out);
    auto const rdbufBackup = std::cout.rdbuf();
    std::cout.rdbuf(fOut.rdbuf());
    for (auto const &street : _streets) {
      auto meanV = this->_getStreetMeanVelocity(street->getIndex());
      if (!(meanV < 0))
        std::cout << street->getVehicleDensity() * 1e3 << '\t' << meanV * 3.6
                  << '\n';
    }
    std::cout.rdbuf(rdbufBackup);
    fOut.close();
  }
}

void Graph::fprintActualState(std::string const &outFolder,
                              std::string const &opt) const noexcept {
  auto const rdbufBackup = std::cout.rdbuf();
  if (opt == "q/k") {
    auto meanDensity = 0.;
    auto meanVelocity = 0.;
    for (auto const &street : _streets) {
      meanDensity += street->getVehicleDensity() * 1e3;
      meanVelocity += this->_getStreetMeanVelocity(street->getIndex()) * 3.6;
    }
    meanDensity /= _streets.size();
    meanVelocity /= _streets.size();
    // auto variance = 0.;
    // for (auto const &street : _streets) {
    //   variance += (street->getVehicleDensity() * 1e3 - meanDensity) *
    //               (street->getVehicleDensity() * 1e3 - meanDensity);
    // }
    // variance = std::sqrt(variance / _streets.size());
    auto out = outFolder + "q-k.dat";
    std::ofstream fOut;
    fOut.open(out, std::ios_base::app);
    std::cout.rdbuf(fOut.rdbuf());
    std::cout << meanDensity << '\t' << meanVelocity * meanDensity << '\n';
    // auto const &street = _streets.at(69);
    // std::cout << street->getVehicleDensity() * 1e3 << '\t'
    //           << this->_getStreetMeanVelocity(street->getIndex()) *
    //                  street->getVehicleDensity() * 3.6e3
    //           << '\n';
  } else if (opt == "u/k") {
    auto meanDensity = 0.;
    auto meanVelocity = 0.;
    for (auto const &street : _streets) {
      meanDensity += street->getVehicleDensity() * 1e3;
      meanVelocity += this->_getStreetMeanVelocity(street->getIndex()) * 3.6;
    }
    meanDensity /= _streets.size();
    meanVelocity /= _streets.size();
    auto out = outFolder + "u-k.dat";
    std::ofstream fOut;
    fOut.open(out, std::ios_base::app);
    std::cout.rdbuf(fOut.rdbuf());
    std::cout << meanDensity << '\t' << meanVelocity << '\n';
  }
  std::cout.rdbuf(rdbufBackup);
}

void Graph::save(const char *fileName) const noexcept {
  std::ofstream fOut;
  fOut.open(fileName);
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  // TODO: save network state on file
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}

// funzione da eliminare (DEBUG)
void Graph::test() {
  std::cout << _vehicles.size() << '\n';
  //   auto const &street = _streets.at(69);
  //   std::vector<double> v;
  //   v.push_back(street->getVehicleDensity() * 1e3);
  //   v.push_back(this->_getStreetMeanVelocity(street->getIndex()) *
  //               street->getVehicleDensity() * 3.6e3);
  //   return v;
}