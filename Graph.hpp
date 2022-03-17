#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Street.hpp"
#include "Vehicle.hpp"
#include <memory>
#include <vector>

class Graph {
private:
  std::vector<std::vector<double>> _adjMatrix;        // adjacency matrix
  std::vector<std::vector<double>> _nodesCoordinates; // coordinates matrix
  std::vector<std::shared_ptr<Street>> _streets;      // streets in the network
  std::vector<std::shared_ptr<Vehicle>> _vehicles;    // vehicles in the network
  int _n;                                             // dimension

  int _minDistance(
      int const,
      int const) const; // algoritmo per la distanza minima tra due nodi
  std::vector<int>
  _nextStep(int const,
            int const); // next step for the destination (min. distance)

public:
  Graph(const char *);               // import from file (only matrix)
  Graph(const char *, const char *); // import from file (matrix + coordinates)
  ~Graph() = default;

  void addVehicle(int);
  void createTransMatrix();

  void printMatrix() const noexcept;
  void print() const noexcept;

  // funzione da eliminare che verrà usata per accedere alle private (REMOVE)
  void test(int, int);
};

#endif