#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

class Graph {
private:
  std::vector<std::vector<int>> _adjMatrix; // matrice di adiacenza
  std::vector<std::vector<double>>
      _nodesCoordinates; // coordinate cartesiane dei nodi
  int _n;                // dimension

  int _minDistance(std::vector<int>, std::vector<bool>);
  void _dijkstra(int);

public:
  Graph(const char *); // import from file
  Graph(const char *, const char *);
  void addEdge(int, int, bool);

  void printMatrix();
  void print();

  // funzione da eliminare che verrà usata per accedere alle private (REMOVE)
  void test(int);
};

#endif