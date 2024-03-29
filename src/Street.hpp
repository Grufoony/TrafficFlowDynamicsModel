#ifndef STREET_HPP
#define STREET_HPP

//! @brief Street class

//! This class is used to represent a street. It contains the source and
//! destination nodes, the length, the maximum capacity, the number of vehicles
//! on the street, the number of lanes, the maximum velocity and the index of
//! the street.

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
  static double _avgLength; // average length of a vehicle, if < 0 then there's
                            // no limit on the capacity
  static double _vMin;      // minimum velocity

public:
  Street(int, int, double, int); // src, dst, length, index
  ~Street() = default;

  int getOrigin() const noexcept;
  int getDestination() const noexcept;
  int getIndex() const noexcept;
  double getLength() const noexcept;
  bool isFull() const noexcept;
  void setNLanes(int);
  int getNLanes() const noexcept;
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
