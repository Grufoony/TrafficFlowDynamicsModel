#include "Graph.hpp"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

// function for dijkstra
int minDistance(std::vector<int> const &dist, std::vector<bool> const &sptSet,
                int const _n) {
  // Initialize min value
  int min = INT_MAX, min_index = -1;

  for (int v = 0; v < _n; v++)
    if (sptSet.at(v) == false && dist.at(v) <= min)
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
  std::vector<int> dist(
      _n); // The output array.  dist[i] will hold the shortest
  // distance from src to i

  std::vector<bool> sptSet(
      _n); // sptSet[i] will be true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized

  // Initialize all distances as INFINITE and stpSet[] as false
  for (int i = 0; i < _n; ++i)
    dist.at(i) = INT_MAX, sptSet.at(i) = false;

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
      if (!sptSet.at(v) && _adjMatrix.at(u).at(v) && dist.at(u) != INT_MAX &&
          dist.at(u) + _adjMatrix.at(u).at(v) < dist.at(v))
        dist.at(v) = dist.at(u) + _adjMatrix.at(u).at(v);
  }

  // print the constructed distance array DEBUG
  // std::cout << "Vertex   Distance from Source\n";
  // for (int i = 0; i < _n; ++i)
  //   std::cout << i << '\t' << '\t' << dist.at(i) << '\n';

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
  for (auto const &vehicle : _vehicles) {
    auto const &trans_vec = vehicle->getVehicleType()->getTransMatrix().at(
        vehicle->getPosition()); // obtain the line with trans probabilities
    auto threshold = 0.;
    auto const p = dist(rng);
    for (int i = 0; i < _n; ++i) {
      auto prob = trans_vec.at(i);
      // std::cout << trans_vec.size() << '\n';
      if (prob > std::numeric_limits<double>::epsilon()) {
        threshold += prob;
        if (p < threshold) {
          vehicle->setPosition(i);
          break;
        }
      }
    }
  }
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

void Graph::createTransMatrix() {
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

    for (int i = 0; i < _n; ++i) {

      // std::cout << "ITERATION NUMBER: " << i << '\n'; // DEBUG

      std::vector<double> temp;
      auto path = _nextStep(i, dst);
      if (path.size() > 0) {
        for (auto &it : path)
          matrix.at(i).at(it) = 42.;
      }
    }
    normalizeMat(matrix);
    vehicle->setTransMatrix(matrix);
  }
}

void Graph::evolve(int const time) {
  for (int dt = 0; dt < time; ++dt) {
    _evolve();
  }
}

void Graph::printMatrix() const noexcept {
  for (auto const &row : _adjMatrix) {
    for (auto const it : row) {
      std::cout << it << '\t';
    }
    std::cout << '\n';
  }
}

void Graph::print() const noexcept {
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

// funzione da eliminare (DEBUG)
void Graph::test() { std::cout << _vehicles.at(0)->getPosition() << '\n'; }