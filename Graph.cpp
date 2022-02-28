#include "Graph.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

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
  for (auto row : _adjMatrix) {
    for (auto it : row) {
      std::cout << it;
    }
    std::cout << '\n';
  }
}

void Graph::print() {
  int i = 0;
  for (auto row : _adjMatrix) {
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