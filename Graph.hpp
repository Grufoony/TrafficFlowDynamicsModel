#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <vector>

class Graph {
private:
  std::vector<std::vector<int>> _adjMatrix; // matrice di adiacenza
  std::vector<std::vector<double>>
      _nodesCoordinates; // coordinate cartesiane dei nodi
  int _n;                // dimension
public:
  Graph(const char *); // import from file
  Graph(const char *, const char *);
  void addEdge(int, int, bool);

  void printMatrix();
  void print();
};

#endif