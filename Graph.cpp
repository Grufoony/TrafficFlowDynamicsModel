#include "Graph.hpp"

#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>

void printPath(std::vector<int> parent, int j) {

  // Base Case : If j is source
  if (parent.at(j) == -1)
    return;

  printPath(parent, parent[j]);

  std::cout << "->" << j;
}

int Graph::_minDistance(std::vector<int> dist, std::vector<bool> sptSet) {
  // Initialize min value
  int min = INT_MAX, min_index;

  for (int v = 0; v < _n; v++)
    if (sptSet.at(v) == false && dist.at(v) <= min)
      min = dist.at(v), min_index = v;

  return min_index;
}

int Graph::_dijkstra(int src, int dst) {
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
    int u = _minDistance(dist, sptSet);

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

  // print the constructed distance array (REMOVE)
  std::cout << "Vertex\tDistance from Source\tPath\n";
  for (int i = 0; i < _n; ++i) {
    std::cout << i << '\t' << '\t' << dist.at(i) << '\t' << '\t';
    printPath(parent, i);
    std::cout << '\n';
  }

  return dist.at(dst);
}

Graph::Graph(const char *fName) {
  _n = 0;
  std::fstream data;

  // setto la dimensione del database
  data.open(fName);
  bool x;
  while (data >> x) {
    ++_n;
  }
  data.close();
  _n = std::sqrt(_n);

  //   importo la matrice di adiacenza e creo il grafo
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
  // setto la dimensione del database
  data.open(fCoordinates);
  bool x;
  double y;
  while (data >> y) {
    ++_n;
  }
  data.close();
  _n = _n / 2;

  // importo le coordinate dei nodi
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

  // importo la matrice di adiacenza e creo il grafo
  data.open(fName);
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

void Graph::printMatrix() {
  for (auto &row : _adjMatrix) {
    for (auto &it : row) {
      std::cout << it;
    }
    std::cout << '\n';
  }
}

void Graph::print() {
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
void Graph::test(int x, int y) { _dijkstra(x, y); }