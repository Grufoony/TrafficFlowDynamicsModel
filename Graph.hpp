#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>

class Graph {
private:
  std::list<int> *_adj; // adjlist
  int _n;               // dimension
public:
  Graph(int);
  Graph(const char *); // import from file
  void addEdge(int, int, bool);

  void print();
};

#endif