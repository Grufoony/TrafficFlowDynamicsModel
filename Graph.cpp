#include "Graph.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

Graph::Graph(int v) : _n{v} { _adj = new std::list<int>[_n]; }
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
  _adj = new std::list<int>[_n];

  //   importo la matrice di adiacenza e creo il grafo
  data.open(fName);
  for (int u = 0; u < _n; ++u) {
    std::vector<int> temp;
    for (int v = 0; v < _n; ++v) {
      data >> x;
      temp.push_back(static_cast<int>(x));
      if (x) {
        _adj[u].push_back(v);
      }
    }
    _adjMatrix.push_back(temp);
  }
  data.close();
}

void Graph::addEdge(int u, int v, bool b) {
  if (b) {
    _adj[u].push_back(v);
    _adj[v].push_back(u);
  }
}

void Graph::printMatrix() {
  for (int i = 0; i < _n; ++i) {
    for (int j = 0; j < _n; ++j) {
      std::cout << _adjMatrix.at(i).at(j);
    }
    std::cout << '\n';
  }
  //   for (auto itu : _adjMatrix) {
  //     for (auto itv : itu) {
  //       std::cout << itv;
  //     }
  //     std::cout << '\n';
  //   }
}

void Graph::print() {
  for (int i = 0; i < _n; ++i) {
    std::cout << i << "-->";
    for (auto it : _adj[i]) {
      std::cout << it << " ";
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}