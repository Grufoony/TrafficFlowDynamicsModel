#include "Graph.hpp"
#include "../utils/utils.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <stdexcept>
#include <string>

double constexpr TEMP_NORM = 273.15e-6;

// using Dijkstra to calculate distance
int Graph::_minDistance(int const src, int const dst) const {
  std::vector<int> dist; // The output array. dist.at(i) will hold the shortest
  // distance from src to i
  dist.reserve(_n);

  std::vector<bool>
      sptSet; // sptSet.at(i) will be true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized
  sptSet.reserve(_n);

  // Initialize all distances as INFINITE and stpSet as false
  for (int i = 0; i < _n; ++i)
    dist.push_back(std::numeric_limits<int>::max()), sptSet.push_back(false);

  // Distance of source vertex from itself is always 0
  dist[src] = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < _n - 1; ++count) {
    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in the first iteration.
    int u = minDistance(dist, sptSet, _n);

    // Mark the picked vertex as processed
    sptSet[u] = true;

    // Update dist value of the adjacent vertices of the picked vertex.
    for (int v = 0; v < _n; ++v) {

      // Update dist.at(v) only if is not in sptSet, there is an edge from
      // u to v, and total weight of path from src to v through u is
      // smaller than current value of dist.at(v)

      // auto length = _adjMatrix.at(u).at(v);
      int time = 0;
      if (_adjMatrix.contains(u, v)) {
        auto weight = _streets[_findStreet(u, v)]->getLength();
        weight /= this->_getStreetMeanVelocity(_findStreet(u, v));
        time = static_cast<int>(weight);
      }
      if (!sptSet[v] && time && dist[u] != std::numeric_limits<int>::max() &&
          dist[u] + time < dist[v])
        dist[v] = dist[u] + time;
    }
  }
  return dist[dst];
}

// function to calculate the next step of a vehicle given the current position
// based on the minimum distance
std::vector<int> Graph::_nextStep(int const src, int const dst) {
  auto const min = _minDistance(src, dst);
  std::vector<int> _nextStep;
  for (auto const &el : _adjMatrix.getRow(src)) {
    auto weight = _streets[_findStreet(src, el.first)]->getLength();
    weight /= this->_getStreetMeanVelocity(_findStreet(src, el.first));
    auto time = static_cast<int>(weight);
    if (_minDistance(el.first, dst) == (min - time))
      _nextStep.push_back(el.first);
  }
  return _nextStep;
}

// remove vehicles that have reached their destination and add new vehicles if
// asked.
void Graph::_removeVehicles(bool reinsert) {
  ++_time;
  if (_time % _timeScale == 0) {
    int nVehicles_old = static_cast<int>(_vehicles.size());
    // erase vehicles that have reached their destination
    _vehicles.erase(
        std::remove_if(_vehicles.begin(), _vehicles.end(),
                       [this](std::shared_ptr<Vehicle> const &vehicle) {
                         bool condition = vehicle->getPosition() ==
                                              vehicle->getDestination() &&
                                          vehicle->getStreet() == -1;
                         if (condition) {
                           _meanTimeTraveled += vehicle->getTimeTraveled();
                           ++_nVehiclesToDst;
                         }
                         return condition;
                       }),
        _vehicles.end());
    int dVehicles = nVehicles_old - static_cast<int>(_vehicles.size());
    // add new vehicles
    if (reinsert) {
      this->addRndmVehicles(dVehicles);
    }
  }
  // keep in memory the previous state of the streets
  int i = 0;
  for (auto const &street : _streets) {
    _vehiclesOnStreet[i] = street->getNVehicles();
    i++;
  }
}

// get the time difference in a street in two different times
int Graph::_getTimeDifference(std::shared_ptr<Vehicle> vehicle) const {
  auto street = _streets[vehicle->getStreet()];
  auto streetLength = street->getLength();
  // time of travel if it keeps the same velocity
  auto oldTime = static_cast<int>(streetLength / vehicle->getVelocity());
  // time of travel if it changes velocity
  auto newTime = static_cast<int>(streetLength / street->getInputVelocity());
  return newTime - oldTime;
}

// change street of a vehicle
void Graph::_changeStreet(std::shared_ptr<Vehicle> vehicle, double const &p) {
  double threshold = 0.;
  for (auto const &probMap : Vehicle::getVehicleType(vehicle->getType())
                                 ->getTransMatrix()
                                 .getRow(vehicle->getPosition())) {
    threshold += probMap.second;
    if (p < threshold) {
      // street update
      int streetIndex = _findStreet(vehicle->getPosition(),
                                    probMap.first); // next street index
      if (!(_streets.at(streetIndex)->isFull()) &&
          !(vehicle->getPreviousPosition() ==
            streetIndex)) { // check if i can move on (street not full)
        if (!(vehicle->getStreet() <
              0)) { // check if the vehicle is on a street
          _streets.at(vehicle->getStreet())->remVehicle();
        }
        _streets.at(streetIndex)->addVehicle(vehicle);
      } else {
        vehicle->setVelocity(0.);
      }
      break;
    }
  }
}

void Graph::_evolve(bool reinsert) {
  this->_removeVehicles(reinsert);
  std::uniform_real_distribution<> dist(0., 1.);
  // cicling through all the vehicles
  for (auto const &vehicle : _vehicles) {
    auto const p = dist(_rng);
    auto timePenalty = vehicle->getTimePenalty();
    vehicle->incrementTimeTraveled();
    if (timePenalty > 0 &&
        vehicle->getVelocity() >
            std::numeric_limits<double>::epsilon()) { // check if the vehicle
                                                      // can change street
      // if not, check if it can tune its velocity
      int dTime = this->_getTimeDifference(vehicle);
      // if the difference is negative, then many vehicles exited the street, so
      // the vehicle can move faster
      if (dTime < 0) {
        if ((timePenalty + dTime) > 0) {
          vehicle->setTimePenalty(timePenalty + dTime);
        } else {
          vehicle->setTimePenalty(0);
        }
        vehicle->setVelocity(
            _streets[vehicle->getStreet()]->getInputVelocity());
      } else {
        // if the difference is positive, then many vehicles entered the street
        // but they won't affect the vehicle
        vehicle->setTimePenalty(timePenalty - 1);
      }
    } else if (vehicle->getPosition() ==
               vehicle->getDestination()) { // check if the vehicle is at the
                                            // destination
      if (!(vehicle->getStreet() < 0)) {
        _streets.at(vehicle->getStreet())->remVehicle();
        vehicle->setStreet(-1);
      }
    } else { // the vehicle can change street
      this->_changeStreet(vehicle, p);
    }
  }
}

int Graph::_findStreet(int const src, int const dst) const {
  int i = 0;
  for (auto const &street : _streets) {
    if (street->getOrigin() == src && street->getDestination() == dst)
      return i;
    ++i;
  }
  return -1;
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
    return _streets[streetIndex]->getVMax();
  }
  return vCum / i;
}
// return density counts of all streets
std::vector<double> Graph::_getDensityCounts(int const nBins) const {
  std::vector<double> densityCounts(nBins + 1, 0.);
  for (auto const &street : _streets) {
    auto const density = street->getDensity();
    int i = 0;
    while (density > (i + 1) * (1. / nBins)) {
      ++i;
    }
    ++densityCounts[i];
  }
  return densityCounts;
}
// return travel time counts of all vehicles, in minutes
std::vector<double> Graph::_getTravelTimeCounts(int const nBins) const {
  std::vector<double> travelTimeCounts;
  auto binSize = 6e3 / nBins;
  for (int i = 0; i < nBins + 1; ++i) {
    double count = std::count_if(
        _vehicles.begin(), _vehicles.end(),
        [i, binSize](std::shared_ptr<Vehicle> const &vehicle) {
          if (vehicle->getPosition() == vehicle->getDestination()) {
            return vehicle->getTimeTraveled() >= i * binSize &&
                   vehicle->getTimeTraveled() < (i + 1) * binSize;
          } else
            return false;
        });
    travelTimeCounts.push_back(count);
  }
  return travelTimeCounts;
}
/// @brief Generate the graph from the matrix
/// @param fName matrix file path
Graph::Graph(std::string fName) {
  _n = 0;
  std::ifstream data;

  // set database's dimension
  data.open(fName);
  if (!data) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Matrix file not found in the given path: \'" + fName +
                      " \'\n";
    throw std::runtime_error(msg);
  }
  double x;
  bool b;
  while (data >> x) {
    ++_n;
  }
  data.close();
  _n = std::sqrt(_n);
  _adjMatrix = SparseMatrix<bool>(_n, _n);

  // import adj matrix from file
  std::cout << "Importing adjacency matrix from file..." << '\n';
  data.open(fName);
  int streetIndex = 0;
  for (int u = 0; u < _n; ++u) {
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      if (b) {
        _adjMatrix.insert(u, v, true);
        _streets.push_back(
            std::make_shared<Street>(Street(u, v, x, streetIndex)));
        ++streetIndex;
      }
    }
  }
  data.close();
  std::cout << "Done." << '\n';
  for (int i = 0; i < static_cast<int>(_streets.size()); ++i) {
    _vehiclesOnStreet.push_back(0);
  }
  // sort streets by index
  std::sort(
      _streets.begin(), _streets.end(),
      [](std::shared_ptr<Street> const &a, std::shared_ptr<Street> const &b) {
        return a->getIndex() < b->getIndex();
      });
}
/// @brief Set the seed for the random number generator
/// @param seed seed for the random number generator
void Graph::setSeed(int const seed) { _rng.seed(seed); }
/// @brief Set the time scale for the simulation. Default value is 100
/// @param timeScale time scale
/// @throw std::invalid_argument if timeScale < 1
void Graph::setTimeScale(int const timeScale) {
  if (timeScale < 1) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Invalid time scale.\n";
    throw std::invalid_argument(msg);
  }
  _timeScale = timeScale;
}
/// @brief Get the time scale of the simulation
/// @return time scale
int Graph::getTimeScale() const noexcept { return _timeScale; }
/// @brief Adds a vehicle with a given type
/// @param type vehicle type
/// @throw std::invalid_argument if type is invalid
void Graph::addVehicle(int type) {
  if (type < 0 || !(type < Vehicle::getNVehicleType())) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Invalid vehicle type.\n";
    throw std::invalid_argument(msg);
  }
  _vehicles.push_back(std::make_shared<Vehicle>(Vehicle(type)));
}
/// @brief Adds a fixed number of vehicles with random types
/// Adds a fixed number of vehicles with random types starting at their origin
/// @param nVehicles number of vehiclesù
/// @throw std::invalid_argument if nVehicles < 0
void Graph::addRndmVehicles(int nVehicles) {
  if (nVehicles < 0) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Number of vehicles must be positive.\n";
    throw std::invalid_argument(msg);
  }
  std::uniform_int_distribution<> dist(0, Vehicle::getNVehicleType() - 1);
  for (int i = 0; i < nVehicles; ++i) {
    int index = dist(_rng);
    this->addVehicle(index);
  }
}
/// @brief Adds a fixed number of vehicles with random types
/// Adds a fixed number of vehicles with random types uniformply distributed on
/// the graph @param nVehicles number of vehicles
/// @throw std::invalid_argument if nVehicles < 0
void Graph::addVehiclesUniformly(int nVehicles) {
  if (nVehicles < 0) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Number of vehicles must be positive.\n";
    throw std::invalid_argument(msg);
  }
  std::uniform_int_distribution<> dist(0,
                                       static_cast<int>(_streets.size() - 1));
  for (int i = 0; i < nVehicles; ++i) {
    this->addRndmVehicles(1);
    int index = dist(_rng);
    while (_streets[index]->isFull()) {
      index = dist(_rng);
    }
    _streets[index]->addVehicle(_vehicles.back());
  }
}

// void Graph::loadVehicles(const char *fName) {
//   std::fstream data;
//   data.open(fName);
//   if (!data) {
//     throw std::runtime_error("Vehicles file does not exist.\n");
//   }
//   // TODO: add vehicles on _streets from file
// }

/// @brief set the system temperature
/// @param temperature
/// @throw std::invalid_argument if temperature < 0
void Graph::setTemperature(double const temperature) {
  if (temperature < 0) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Temperature must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _temperature = temperature;
}
/// @brief get the system temperature
/// @return temperature
double Graph::getTemperature() const noexcept { return _temperature; }
/// @brief update the transition matrix
/// @details The transition matrix is updated according to the expected
/// traveltime using Dijsktra's algorithm. It uses also temperature to
/// simulate the effect of a noise.
void Graph::updateTransMatrix() {
  // function for noise using a kelvin-like temperature normalization
  auto const noise = std::tanh(_temperature * TEMP_NORM);

  for (int index = 0; index < Vehicle::getNVehicleType(); ++index) {
    auto const vehicle = Vehicle::getVehicleType(index);
    int const dst = vehicle->getDestination();
    SparseMatrix<double> matrix(this->_adjMatrix.getRowDim(),
                                this->_adjMatrix.getColDim());
    // setting to 1 the probabilities of correct movements
    for (int i = 0; i < _n; ++i) {
      auto path = _nextStep(i, dst);
      if (path.size() > 0) {
        for (auto &it : path)
          matrix.insert(i, it, 1.);
      }
    }
    // setting noise
    for (int i = 0; i < _n; ++i) {
      for (int j = 0; j < _n; ++j) {
        if (_adjMatrix.contains(i, j) && matrix(i, j) < 0.1) {
          matrix.insert_or_assign(i, j, noise);
        }
      }
    }
    matrix = matrix.getNormRows();
    vehicle->setTransMatrix(matrix);
  }
}
/// @brief Updates the state of the system
/// @details Updates the state of the system by moving vehicles. The evolution
/// algorithm, for all vehicles:
/// - checks if a vehicle is able to move from its actual position, i.e. checks
/// its time penalty. If it is not able to move then the penalty is decreased by
/// one. If it is able the next step is randocmly choosen depending on the
/// transition matrix.
/// - if the destination street is full, then a time step is lost (like a STOP
/// sign). Else, the vehicle enters the street with a velocity which depends on
/// the veicle density of the street.
/// - depending on the entering velocity, a new time penalty is assigned to the
/// vehicle, of the form \f$\frac{L}{v(t)}\f$, with \f$L\f$ length of the street
/// @param reinsert if true, vehicles are reinserted in the streets
/// from their origin
void Graph::evolve(bool reinsert) { this->_evolve(reinsert); }
/// @brief Updates the state of the system reinserting vehicles
/// Updates the state of the system by moving vehicles and reinserting them in
/// the streets from their origin
void Graph::evolve() { this->_evolve(true); }
/// @brief Print the transition matrix
void Graph::printMatrix() noexcept { _adjMatrix.print(); }
/// @brief Print informations about the system
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
    for (int i = 0; i < _adjMatrix.getRowDim(); ++i) {
      auto const &row = _adjMatrix.getRow(i);
      std::cout << i;
      if (!(_nodesCoordinates.empty())) {
        std::cout << " (" << _nodesCoordinates[0][i] << ','
                  << _nodesCoordinates[1][i] << ") ";
      }
      std::cout << "-->";
      row.print();
      std::cout << '\n';
    }
  }
  std::cout << "-------------------------" << '\n';
}
/// @brief Print information of every street.
/// Print information of every street like index, origin, destination, number of
/// vehicles and input velocity.
void Graph::printStreets() const noexcept {
  int i = 0;
  for (auto const &street : _streets) {
    if (street->getNVehicles() > 0) {
      std::cout << i << '(' << street->getOrigin() << ','
                << street->getDestination() << ')' << ": "
                << street->getNVehicles() << '\t' << std::setprecision(3)
                << street->getInputVelocity() << '\n';
    }
    ++i;
  }
}

void Graph::fprintMatrix(const char *fName) { _adjMatrix.fprint(fName); }
/// @brief Print information of the network.
/// @param printGraph if true, prints the graph.
/// Print information of the network like number of nodes, number of streets and
/// the graph.
void Graph::fprint(const bool printGraph) const noexcept {
  std::ofstream fOut;
  fOut.open("network_info.txt");
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  this->print(printGraph);
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}
/// @brief Print information of every street on a file.
/// @param fName name of the file where the information will be saved.
/// Print information of every street like index, origin, destination, number of
/// vehicles and input velocity.
void Graph::fprintStreets(std::string const &fName) const noexcept {
  std::ofstream fOut;
  fOut.open(fName);
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  this->printStreets();
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}
/// @brief Print network's data in a format readable by the script visual.py.
/// @param outFolder folder where the data file will be saved.
void Graph::fprintVisual(std::string const &outFolder) const noexcept {
  std::ofstream fOut;
  auto out = outFolder + std::to_string(_time) + ".dat";
  fOut.open(out);
  auto const rdbufBackup = std::cout.rdbuf();
  std::cout.rdbuf(fOut.rdbuf());
  std::cout << "source" << '\t' << "target" << '\t' << "load" << '\t' << "x"
            << '\n';
  for (auto const &street : _streets) {
    std::cout << street->getOrigin() << '\t' << street->getDestination() << '\t'
              << street->getNVehicles() << '\t' << std::setprecision(3)
              << street->getInputVelocity() << '\n';
  }
  std::cout.rdbuf(rdbufBackup);
  fOut.close();
}
/// @brief Print some network's data in a elegible format.
/// @param outFolder folder where the data file will be saved.
/// @param opt is used to select wich data print. In particolar you can choose
/// between the following options:
/// - "density" to print the histogram of the vehicle density on the streets.
/// - "traveltime" to print the histogram of the travel time of the vehicles.
/// @param nBins is the number of bins used to create the histogram.
/// @param format is the format of the data. In particolar you can choose
/// between the following options:
/// - "latex" to print the data in a format readable by latex.
/// - "root" to print the data in a format readable by root.
/// @throw std::invalid_argument if nBins < 1 or format/option is invalid.
void Graph::fprintHistogram(std::string const &outFolder,
                            std::string const &opt, int const nBins,
                            std::string const &format) const {
  if (nBins < 1) {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Number of bins must be greater than one.\n";
    throw std::invalid_argument(msg);
  }
  if (opt != "density" && opt != "traveltime") {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Option should be \"density\" or \"traveltime\".\n";
    throw std::invalid_argument(msg);
  }
  std::ofstream fOut;
  if (opt == "density") {
    auto out = outFolder + std::to_string(_time) + "_den.dat";
    fOut.open(out);
    auto const &densityCounts = this->_getDensityCounts(nBins);
    for (int i = 0; i < nBins + 1; ++i) {
      fOut << std::setprecision(3) << i * (1. / nBins) << '\t'
           << densityCounts[i] / _streets.size() << '\n';
    }

    fOut << (nBins + 1.) * (1. / nBins);
    fOut.close();
    return;
  }
  // traveltime
  if (format != "latex" && format != "root") {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Format should be \"latex\" or \"root\".\n";
    throw std::invalid_argument(msg);
  }
  auto out = outFolder + std::to_string(_time);
  if (format == "latex") {
    out += "_t.dat";
    fOut.open(out);
    auto travelTimeCounts = this->_getTravelTimeCounts(nBins);
    normalizeVec(travelTimeCounts);
    for (int j = 0; j < static_cast<int>(travelTimeCounts.size()); ++j) {
      fOut << std::setprecision(3) << j * 1e2 / nBins << '\t'
           << travelTimeCounts[j] << '\n';
    }
    fOut.close();
    return;
  }
  // root format
  out += "_root.dat";
  fOut.open(out);
  for (auto const &vehicle : _vehicles) {
    if (vehicle->getPosition() == vehicle->getDestination()) {
      fOut << vehicle->getTimeTraveled() / 60. << '\n';
    }
  }
  fOut.close();
  return;
}
/// @brief Print some network's data distributions in a format readable by
/// LaTeX. @param outFolder folder where the data file will be saved. @param opt
/// is used to select wich data print. In particolar you can choose between the
/// following options:
/// - "u/q" to print the velocity/flux distribution
/// - "q/k" to print the flow/capacity distribution
/// - "u/k" to print the velocity/capacity distribution
/// @throw std::invalid_argument if opt is not valid
void Graph::fprintDistribution(std::string const &outFolder,
                               std::string const &opt) const {
  if (opt != "u/q" && opt != "q/k" && opt != "u/k") {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Option should be \"u/q\", \"q/k\" or \"u/k\".\n";
    throw std::invalid_argument(msg);
  }
  std::ofstream fOut;
  std::vector<double> u, q, k, x, y;
  for (auto const &street : _streets) {
    auto meanV = this->_getStreetMeanVelocity(street->getIndex());
    if (!(meanV < 0)) {
      u.push_back(meanV * 3.6);
      q.push_back(meanV * street->getVehicleDensity() * 3.6e3);
      k.push_back(street->getVehicleDensity() * 1e3);
    }
  }
  if (opt == "u/q") {
    auto out = outFolder + std::to_string(_time) + "_u-q.dat";
    fOut.open(out);
    x = q;
    y = u;
  } else if (opt == "q/k") {
    auto out = outFolder + std::to_string(_time) + "_q-k.dat";
    fOut.open(out);
    x = k;
    y = q;
  } else { // u/k
    auto out = outFolder + std::to_string(_time) + "_u-k.dat";
    fOut.open(out);
    x = k;
    y = u;
  }
  // print on file the results
  for (int i = 0; i < static_cast<int>(x.size()); ++i) {
    fOut << x[i] << '\t' << y[i] << '\n';
  }
  fOut.close();
}
/// @brief Print some network's data in a format readable by LaTeX.
/// @param outFolder folder where the data file will be saved.
/// @param opt is used to select wich data print. In particolar you can choose
/// between the following options:
/// - "k" to append the mean capacity of the network to the file k-t.dat
/// - "q" to append the mean flow of the network to the file q-t.dat
/// - "u" to append the mean velocity of the network to the file u-t.dat
/// @param timeZero is the time at which the simulation starts
/// @throw std::invalid_argument if opt is not valid
void Graph::fprintTimeDistribution(std::string const &outFolder,
                                   std::string const &opt,
                                   double const timeZero) const {
  if (opt != "k" && opt != "q" && opt != "u") {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Option should be \"k\", \"q\" or \"u\".\n";
    throw std::invalid_argument(msg);
  }
  // timeZero in hours
  std::ofstream fOut;
  auto meanDensity = 0.;
  auto meanVelocity = 0.;
  for (auto const &street : _streets) {
    meanDensity += street->getVehicleDensity() * 1e3;
    meanVelocity += this->_getStreetMeanVelocity(street->getIndex()) * 3.6;
  }
  meanDensity /= _streets.size();
  meanVelocity /= _streets.size();
  std::string out;
  double y = 0.;
  if (opt == "k") {
    out = outFolder + "k-t.dat";
    y = meanDensity;
  } else if (opt == "q") {
    out = outFolder + "q-t.dat";
    y = meanDensity * meanVelocity;
  } else { // velocity (u)
    out = outFolder + "u-t.dat";
    y = meanVelocity;
  }
  fOut.open(out, std::ios_base::app);
  fOut << _time / 3.6e3 + timeZero << '\t' << y << '\n';
  fOut.close();
}
/// @brief Print the means of some network's data in a format readable by LaTeX.
/// @param outFolder folder where the data file will be saved.
/// @param opt is used to select wich data print. In particolar you can choose
/// between the following options:
/// - "q/k" to print the mean flow/capacity
/// - "u/k" to print the mean velocity/capacity
/// @throw std::invalid_argument if opt is not valid
void Graph::fprintActualState(std::string const &outFolder,
                              std::string const &opt) const {
  if (opt != "q/k" && opt != "u/k") {
    std::string msg = "Graph.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Option should be \"q/k\" or \"u/k\".\n";
    throw std::invalid_argument(msg);
  }
  std::ofstream fOut;
  // Compute mean density and velocity
  auto meanDensity = 0.;
  auto meanVelocity = 0.;
  for (auto const &street : _streets) {
    meanDensity += street->getVehicleDensity() * 1e3;
    meanVelocity += this->_getStreetMeanVelocity(street->getIndex()) * 3.6;
  }
  meanDensity /= _streets.size();
  meanVelocity /= _streets.size();
  std::string out;
  double y = 0.;
  if (opt == "q/k") { // Print mean flow/density
    out = outFolder + "q-k.dat";
    y = meanVelocity * meanDensity;
  } else { // Print mean velocity/density
    out = outFolder + "u-k.dat";
    y = meanVelocity;
  }
  fOut.open(out, std::ios_base::app);
  fOut << meanDensity << '\t' << y << '\n';
  fOut.close();
}

// void Graph::save(const char *fileName) const noexcept {
//   std::ofstream fOut;
//   fOut.open(fileName);
//   // TODO: save network state on file
//   fOut.close();
// }
