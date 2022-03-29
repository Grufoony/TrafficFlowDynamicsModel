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
  double _temperature = 0.; // temperature of the network

  int _minDistance(
      int const,
      int const) const; // algoritmo per la distanza minima tra due nodi
  std::vector<int>
  _nextStep(int const,
            int const); // next step for the destination (min. distance)
  void _evolve();       // evolve the vehicle's position

public:
  Graph(const char *);               // import from file (only matrix)
  Graph(const char *, const char *); // import from file (matrix + coordinates)
  ~Graph() = default;

  void addVehicle(int);              // add a vehicle of a type in _vehicles
  void setTemperature(double const); // set the temperature of the network
  void createTransMatrix(); // create all the transition matrices for all
                            // vehicle types
  void evolve(int const);   // evolve the vehicles' position

  void printMatrix() const noexcept;     // print adjency matrix on terminal
  void print(bool const) const noexcept; // print network on terminal

  // funzione da eliminare che verrà usata per accedere alle private (DEBUG)
  void test();
};

#endif