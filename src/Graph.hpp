#ifndef GRAPH_HPP
#define GRAPH_HPP

//! Graph class

//!  This class is used to represent a graph.

#include "../utils/SparseMatrix.hpp"
#include "Street.hpp"
#include "Vehicle.hpp"
#include <memory>
#include <string>
#include <vector>

class Graph {
private:
  SparseMatrix<bool> _adjMatrix;
  std::vector<std::vector<double>> _nodesCoordinates; // coordinates matrix
  std::vector<std::shared_ptr<Street>> _streets;      // streets in the network
  std::vector<std::shared_ptr<Vehicle>> _vehicles;    // vehicles in the network
  std::vector<int> _vehiclesOnStreet;                 // vehicles on the street
  int _n;                                             // dimension
  double _temperature = 0.;      // temperature of the network
  int _time = 0;                 // time of the simulation
  double _meanTimeTraveled = 0.; // mean time traveled by vehicles
  int _nVehiclesToDst = 0;       // number of vehicles to destination
  uint16_t _timeScale = 100;     // time scale for the simulation
  std::mt19937 _rng{std::random_device{}()}; // random number generator

  int _minDistance(int const, int const)
      const; // algorithm for min distance between two nodes
  std::vector<int>
  _nextStep(int const,
            int const); // next step for the destination (min. distance)
  void _evolve(bool);   // evolve the vehicle's position
  int _findStreet(int const,
                  int const) const; // find street with origin and destination
  double _getStreetMeanVelocity(int const) const; // get the mean velocity of a
                                                  // street

public:
  Graph(std::string); //!< import from file (only matrix)
  ~Graph() = default;

  void setSeed(int const);      //!< set the seed of the random number generator
  void setTimeScale(int const); //!< set the time scale
  int getTimeScale() const noexcept; //!< get the time scale
  void addVehicle(int);              //!< add a vehicle of a type in _vehicles
                                     //!< street
  void addRndmVehicles(int);      //!< add vehicles of random type in _vehicles
  void addVehiclesUniformly(int); //!< add vehicles uniformly on the streets
  void setTemperature(double const); //!< set the temperature of the network
  double getTemperature() const noexcept; //!< get the temperature of the
                                          //!< network
  void updateTransMatrix(); //!< create all the transition matrices for all
                            //!< vehicle types
  void evolve(bool);        //!< evolve the network
  void evolve();            //!< evolve the network with reinsertion (default)

  void printMatrix() noexcept;           //!< print adjency matrix on terminal
  void print(bool const) const noexcept; //!< print network info on terminal
  void printStreets() const
      noexcept; //!< print streets on terminal with nodes and number of vehicles

  void fprintMatrix(char const *);        //!< print adjency matrix on file
  void fprint(bool const) const noexcept; //!< print network info on file
  void fprintStreets(std::string const &) const
      noexcept; //!< print streets on file with nodes and number of vehicles
  void fprintVisual(
      std::string const &) const noexcept; //!< print file for visual.py
  void fprintHistogram(std::string const &, std::string const &, int const,
                       std::string const &) const; // print histograms
  void fprintDistribution(
      std::string const &,
      std::string const &) const; //!< print graphs whit format y \t x
  void fprintTimeDistribution(
      std::string const &, std::string const &,
      double const) const; //!< print graphs whit format y \t t
  void fprintActualState(std::string const &,
                         std::string const &) const; //!< print actual state

  //   void save(const char *) const noexcept; // TODO: save network in file

  // funzione da eliminare che verrà usata per accedere alle private (DEBUG)
  //   void test();
};

#endif
