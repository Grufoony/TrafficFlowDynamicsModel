#include "Graph.hpp"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

// function for dijkstra
int minDistance(std::vector<int> dist, std::vector<bool> sptSet, int _n) {
  // Initialize min value
  int min = INT_MAX, min_index;

  for (int v = 0; v < _n; v++)
    if (sptSet.at(v) == false && dist.at(v) <= min)
      min = dist.at(v), min_index = v;

  return min_index;
}

// using Dijkstra to calculate distance
int Graph::_minDistance(int src, int dst) {
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

std::vector<int> Graph::_nextStep(int src, int dst) {
  auto const &row = _adjMatrix.at(src);
  auto min = _minDistance(src, dst);
  std::vector<int> _nextStep;
  for (int i = 0; i < static_cast<int>(row.size()); ++i) {
    if (row.at(i)) {
      // strade assunte tutte uguali
      if (_minDistance(i, dst) == (min - 1))
        _nextStep.push_back(i);
    }
  }
  return _nextStep;
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
    std::vector<bool> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(b);
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
    std::vector<bool> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      b = x > 0;
      temp.push_back(b);
      if (b) {
        _streets.push_back(std::make_shared<Street>(Street(u, v, x)));
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
}

void Graph::addEdge(int u, int v, bool b) {
  if (b) {
    _adjMatrix.at(u).at(v) = b;
    _adjMatrix.at(v).at(u) = b;
  }
}

void Graph::addVehicle(int type) {
  _vehicles.push_back(std::make_shared<Vehicle>(Vehicle(type)));
}

void Graph::createTransMatrix() {
  for (int index = 0; index < Vehicle::getNVehicleType(); ++index) {
    auto vehicle = Vehicle::getVehicleType(index);
    int dst = vehicle->getDestination();
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

    vehicle->setTransMatrix(matrix);
  }
}

void Graph::printMatrix() noexcept {
  for (auto &row : _adjMatrix) {
    for (auto it : row) {
      std::cout << it << '\t';
    }
    std::cout << '\n';
  }
}

void Graph::print() noexcept {
  int i = 0;
  for (auto &row : _adjMatrix) {
    std::cout << i;
    if (!(_nodesCoordinates.empty())) {
      std::cout << " (" << _nodesCoordinates.at(0).at(i) << ','
                << _nodesCoordinates.at(1).at(i) << ") ";
    }
    std::cout << "-->";
    int j = 0;
    for (auto it : row) {
      if (it) {
        std::cout << '\t' << j;
      }
      ++j;
    }
    std::cout << '\n';
    ++i;
  }
}

// funzione da eliminare (REMOVE)
void Graph::test(int x, int y) {
  x = y;
  y = x;
  return;
}