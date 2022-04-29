#ifndef STREET_HPP
#define STREET_HPP

#include "Vehicle.hpp"
#include <memory>

class Street {
private:
  double _lenght;   // lenght of the street
  int _maxCapacity; // maximum capacity of vehicles
  int _nVehicles;   // number of vehicles on the street
  int _src;         // source node
  int _dst;         // destination node
  int _nLanes;      // number of lanes
  double _vMax;     // max velocity for that street
  int _index;       // index of the street (it's used to identify the street)

public:
  Street(int, int, double, int);
  ~Street() = default;

  int getOrigin() const noexcept;
  int getDestination() const noexcept;
  double getLenght() const noexcept;
  bool isFull() const noexcept;
  void setNLanes(int) noexcept;
  int getNVehicles() const noexcept;
  void setVMax(double);
  double getVelocity() const noexcept;
  double getVMax() const noexcept;
  double getVehicleDensity() const noexcept;
  void addVehicle(std::shared_ptr<Vehicle>);
  void remVehicle();
};

#endif