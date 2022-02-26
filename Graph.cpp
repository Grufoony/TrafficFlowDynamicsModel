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
    std::cout << i << "-->";
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