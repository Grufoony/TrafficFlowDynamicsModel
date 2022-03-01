#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Street.hpp"
#include <vector>

class Graph {
private:
  std::vector<std::vector<int>> _adjMatrix; // matrice di adiacenza
  std::vector<std::vector<double>>
      _nodesCoordinates; // coordinate cartesiane dei nodi
  std::vector<Street> _streets;
  int _n; // dimension

  int _minDistance(std::vector<int>, std::vector<bool>);
  int _dijkstra(int, int); // algoritmo per la distanza minima tra due nodi
  // double _attractionModule(int);

public:
  Graph(const char *);               // import from file (only matrix)
  Graph(const char *, const char *); // import from file (matrix + coordinates)
  void addEdge(int, int, bool);

  void printMatrix();
  void print();

  // funzione da eliminare che verrà usata per accedere alle private (REMOVE)
  void test(int, int);
};

#endif