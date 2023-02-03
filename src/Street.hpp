#ifndef STREET_HPP
#define STREET_HPP

//! Street class

//! This class is used to represent a street.

#include "Vehicle.hpp"
#include <memory>

class Street {
private:
  double _length;   // length of the street
  int _maxCapacity; // maximum capacity of vehicles
  int _nVehicles;   // number of vehicles on the street
  uint16_t _src;    // source node
  uint16_t _dst;    // destination node
  int _nLanes;      // number of lanes
  double _vMax;     // max velocity for that street
  int _index;       // index of the street (it's used to identify the street)

public:
  Street(int, int, double, int); // src, dst, length, index
  ~Street() = default;

  int getOrigin() const noexcept;
  int getDestination() const noexcept;
  int getIndex() const noexcept;
  double getLength() const noexcept;
  bool isFull() const noexcept;
  void setNLanes(int) noexcept;
  int getNVehicles() const noexcept;
  void setVMax(double);
  double getInputVelocity()
      const noexcept; //!< get instant input velocity for the street
  double getVMax() const noexcept;
  double getDensity()
      const noexcept; //!< get instant density for the street (in percentage)
  double getVehicleDensity()
      const noexcept; //!< get instant vehicle density for the stree
  void addVehicle(std::shared_ptr<Vehicle>); //!< add a vehicle to the street
  void remVehicle(); //!< remove a vehicle from the street
};

#endif
