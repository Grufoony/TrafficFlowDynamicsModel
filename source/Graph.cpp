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
  std::vector<int> dist; // The output array.  dist[i] will hold the shortest
  // distance from src to i

  std::vector<bool>
      sptSet; // sptSet[i] will be true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized

  // Initialize all distances as INFINITE and stpSet[] as false
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
    for (int v = 0; v < _n; ++v)

      // Update dist[v] only if is not in sptSet, there is an edge from
      // u to v, and total weight of path from src to  v through u is
      // smaller than current value of dist[v]
      if (!sptSet.at(v) && _adjMatrix.at(u).at(v) &&
          dist.at(u) != std::numeric_limits<int>::max() &&
          dist.at(u) + _adjMatrix.at(u).at(v) < dist.at(v))
        dist.at(v) = dist.at(u) + _adjMatrix.at(u).at(v);
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
      if (_minDistance(i, dst) == (min - lenght))
        _nextStep.push_back(i);
    }
  }
  return _nextStep;
}

void Graph::_evolve() {
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
      vehicle->setTimePenalty(timePenalty - 1);
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
              vehicle->setStreet(streetIndex);
              vehicle->setVelocity(_streets.at(streetIndex)->getVelocity());
              _streets.at(streetIndex)->addVehicle();
              vehicle->setTimePenalty(
                  static_cast<int>(_streets.at(streetIndex)->getLenght() /
                                   vehicle->getVelocity()));
              vehicle->setPosition(i);
              break;
            }
          }
        }
      }
    }
  }
  // erase vehicles that have reached their destination
  _vehicles.erase(std::remove_if(_vehicles.begin(), _vehicles.end(),
                                 [](std::shared_ptr<Vehicle> const &vehicle) {
                                   return vehicle->getPosition() ==
                                              vehicle->getDestination() &&
                                          vehicle->getStreet() == -1;
                                 }),
                  _vehicles.end());
  ++_time;
}

int Graph::_findStreet(int const src, int const dst) {
  int i = 0;
  for (auto const &street : _streets) {
    if (street->getOrigin() == src && street->getDestination() == dst)
      return i;
    ++i;
  }
  throw std::runtime_error("Street not found");
}

Graph::Graph(const char *fName) {
  _n = 0;
  std::fstream data;

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
  data.open(fName);
  for (int u = 0; u < _n; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(x);
      if (b) {
        _streets.push_back(std::make_shared<Street>(Street(u, v, x)));
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
}

Graph::Graph(const char *fName, const char *fCoordinates) {
  _n = 0;
  std::fstream data;
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
  data.open(fName);
  if (!data) {
    throw std::runtime_error("Matrix file does not exist.\n");
  }
  for (int u = 0; u < _n; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(x);
      if (b) {
        _streets.push_back(std::make_shared<Street>(Street(u, v, x)));
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
}

void Graph::addVehicle(int type) {
  _vehicles.push_back(std::make_shared<Vehicle>(Vehicle(type)));
}

void Graph::addRndmVehicles(int nVehicles) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<> dist(0, Vehicle::getNVehicleType() - 1);
  for (int i = 0; i < nVehicles; ++i) {
    int index = dist(rng);
    this->addVehicle(index);
  }
}

void Graph::setTemperature(double const temperature) {
  if (temperature < 0)
    throw std::runtime_error("Temperature must be positive.\n");
  _temperature = temperature;
}

void Graph::createTransMatrix() {
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

void Graph::evolve(int const nVehicles) {
  if (nVehicles > 0)
    this->addRndmVehicles(nVehicles);
  this->_evolve();
}
void Graph::evolve() { this->_evolve(); }

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

void Graph::fprintDistribution(int const nBins) const noexcept {
  std::ofstream fOut;
  auto out = "./data/" + std::to_string(_time - 1) + ".dat";
  fOut.open(out);
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  int n;
  for (int i = 0; i < nBins + 1; ++i) {
    n = std::count_if(
        _streets.begin(), _streets.end(),
        [i, nBins](std::shared_ptr<Street> const &street) {
          return street->getVehicleDensity() >= i * (1. / nBins) &&
                 street->getVehicleDensity() < (i + 1) * (1. / nBins);
        });
    std::cout << std::setprecision(3) << i * (1. / nBins) << '\t' << n << '\n';
  }
  // for (int i = 0; i < nBins; ++i) {
  //   // get n vehicles based on velocity
  //   n = std::count_if(
  //       _vehicles.begin(), _vehicles.end(),
  //       [i, nBins, this](std::shared_ptr<Vehicle> const &vehicle) {
  //         auto index = vehicle->getStreet();
  //         if (index < 0)
  //           return false;
  //         return (vehicle->getVelocity() / _streets.at(index)->getVMax()) >=
  //                    i * (1. / nBins) &&
  //                (vehicle->getVelocity() / _streets.at(index)->getVMax()) <
  //                    (i + 1) * (1. / nBins);
  //       });
  //   std::cout << i * (1. / nBins) << '\t'
  //             << static_cast<double>(n) /
  //             static_cast<double>(_vehicles.size())
  //             << '\n';
  // }
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}

// funzione da eliminare (DEBUG)
void Graph::test() {
  for (auto const &street : _streets) {
    std::cout << street->getOrigin() << '\t' << street->getDestination() << '\t'
              << street->getNVehicles() << '\t' << std::setprecision(3)
              << street->getVelocity() << '\n';
  }
  // int const nBins = 33;
  // int n;
  // for (int i = 0; i < nBins; ++i) {
  //   n = std::count_if(
  //       _streets.begin(), _streets.end(),
  //       [i, nBins](std::shared_ptr<Street> const &street) {
  //         return street->getVehicleDensity() >= i * (1. / nBins) &&
  //                street->getVehicleDensity() < (i + 1) * (1. / nBins);
  //       });
  //   std::cout << i * (1. / nBins) << '\t' << n << '\n';
  // }
  // std::cout << '\n';
}