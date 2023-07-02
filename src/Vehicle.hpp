#ifndef VEHICLE_HPP
#define VEHICLE_HPP

//! @brief Vehicle class

//! This class is used to represent a vehicle. It contains the vehicle type,
//! the position, the street, the time penalty, the velocity and the time
//! traveled.

#include "VehicleType.hpp"
#include <memory>
#include <string>
#include <vector>

class Vehicle {
private:
  static std::vector<std::shared_ptr<VehicleType>> _vehicleType;
  uint8_t _index;
  uint16_t _position;
  int16_t _previousPosition = -1;
  int _street = -1;
  int _timePenalty = 0;
  double _velocity = 0.;
  int _timeTraveled = 0;

public:
  Vehicle(uint8_t); //!< create a vehicle of a type in _vehicleType
  Vehicle(const Vehicle &v)
      : _index(v._index), _position(v._position),
        _previousPosition(v._previousPosition), _street(v._street),
        _timePenalty(v._timePenalty), _velocity(v._velocity),
        _timeTraveled(v._timeTraveled) {}
  ~Vehicle() = default;

  static void addVehicleType(uint16_t,
                             uint16_t); //!< add a vehicle type in _vehicleType
  static void addVehicleType(
      std::string); //!< add many vehicle types in _vehicleType from file
  static std::shared_ptr<VehicleType>
  getVehicleType(int8_t const); //!< get a vehicle type from _vehicleType
  static uint8_t
  getNVehicleType(); //!< get the number of vehicle types in _vehicleType
  uint8_t getType()
      const noexcept; // return the index of the vehicle type in _vehicleType
  void setPosition(int16_t);
  uint16_t getPosition() const noexcept;
  int16_t getPreviousPosition() const noexcept;
  void setStreet(int);
  int getStreet() const;
  uint16_t getDestination() const noexcept;
  void setTimePenalty(int);
  int getTimePenalty() const noexcept;
  void setVelocity(double);
  double getVelocity() const noexcept;
  void incrementTimeTraveled() noexcept;
  int getTimeTraveled() const noexcept;
  void resetTimeTraveled() noexcept;
};

#endif
