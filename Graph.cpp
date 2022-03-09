#include "Graph.hpp"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

// DEBUG
void printPath(std::vector<int> parent, int j) {

  // Base Case : If j is source
  if (parent.at(j) == -1)
    return;

  printPath(parent, parent.at(j));

  std::cout << "->" << j;
}

void sortPath(std::vector<int> parent, std::vector<int> &path, int j) {

  // Base Case : If j is source
  if (parent.at(j) != -1) {
    sortPath(parent, path, parent.at(j));
    path.push_back(parent.at(j));
  }
}

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

// using Dijkstra to botain the shortest path
std::vector<int> Graph::_Path(int src, int dst) {
  std::vector<int> dist(
      _n); // The output array.  dist[i] will hold the shortest
  // distance from src to i

  std::vector<bool> sptSet(
      _n); // sptSet[i] will be true if vertex i is included in shortest
  // path tree or shortest distance from src to i is finalized

  std::vector<int> parent(_n); // Parent array to store shortest path tree

  // Initialize all distances as INFINITE and stpSet[] as false
  for (int i = 0; i < _n; ++i)
    dist.at(i) = INT_MAX, sptSet.at(i) = false, parent.at(i) = -1;

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
          dist.at(u) + _adjMatrix.at(u).at(v) < dist.at(v)) {
        parent.at(v) = u;
        dist.at(v) = dist.at(u) + _adjMatrix.at(u).at(v);
      }
  }

  std::vector<int> path;
  // print the constructed distance array DEBUG
  // std::cout << "Vertex\tDistance from Source\tPath\n";
  // for (int i = 0; i < _n; ++i) {
  //   std::cout << i << '\t' << '\t' << dist.at(i) << '\t' << '\t';
  //   printPath(parent, i);
  //   std::cout << '\n';
  // }
  // std::cout << '\n';

  sortPath(parent, path, dst);

  // for (auto &it : path) {
  //   std::cout << it << '\n';
  // }

  return path;
}

Graph::Graph(const char *fName) {
  _n = 0;
  std::fstream data;

  // set database's dimension
  data.open(fName);
  if (!data) {
    throw std::runtime_error("Matrix file does not exist.\n");
  }
  bool x;
  while (data >> x) {
    ++_n;
  }
  data.close();
  _n = std::sqrt(_n);

  // import adj matrix from file
  data.open(fName);
  for (int u = 0; u < _n; ++u) {
    std::vector<int> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      temp.push_back(static_cast<int>(x));
      if (x) {
        _streets.push_back(Street(u, v));
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
  bool x;
  double y;
  while (data >> y) {
    ++_n;
  }
  data.close();
  _n = _n / 2;

  // import coordinates
  data.open(fCoordinates);
  for (int u = 0; u < 2; ++u) {
    std::vector<double> temp;
    for (int v = 0; v < _n; ++v) {
      data >> y;
      temp.push_back(y);
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
    std::vector<int> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      temp.push_back(static_cast<int>(x));
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

void Graph::addVehicle(int type) { _vehicles.push_back(Vehicle(type)); }

void Graph::createTransMatrix() {
  for (int index = 0; index < Vehicle::getNVehicleType(); ++index) {
    auto vehicle = Vehicle::getVehicleType(index);
    int dst = vehicle->getDestination();
    int next;
    std::vector<std::vector<double>> matrix;
    for (int i = 0; i < _n; ++i) {

      // std::cout << "ITERATION NUMBER: " << i << '\n'; //DEBUG

      std::vector<double> temp;
      auto path = _Path(i, dst);
      if (path.size() > 1)
        next = path.at(1);
      else
        next = -1;
      for (int j = 0; j < _n; ++j) {

        // std::cout << "SUBITERATION NUMBER: " << j << '\n'; // DEBUG

        if (j == next)
          temp.push_back(42.);
        else
          temp.push_back(0.);
      }
      matrix.push_back(temp);
    }
    for (auto it1 : matrix) {
      for (auto it : it1)
        std::cout << it << " ";
      std::cout << '\n';
    }
    std::cout << "-------------------------------\n";

    vehicle->setTransMatrix(matrix);
  }
}

void Graph::printMatrix() noexcept {
  for (auto &row : _adjMatrix) {
    for (auto &it : row) {
      std::cout << it;
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
    for (auto &it : row) {
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
void Graph::test(int x, int y) { _Path(x, y); }