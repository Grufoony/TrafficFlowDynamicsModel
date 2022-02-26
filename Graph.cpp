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

  //   importo la matrice di adiacenza
  data.open(fName);
  for (int u = 0; u < _n; ++u) {
    for (int v = 0; v < _n; ++v) {
      data >> x;
      if (x) {
        _adj[u].push_back(v);
      }
    }
  }
  data.close();
}

void Graph::addEdge(int u, int v, bool b) {
  if (b) {
    _adj[u].push_back(v);
    _adj[v].push_back(u);
  }
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