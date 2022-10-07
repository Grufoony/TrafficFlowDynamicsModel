#ifndef VEHICLE_HPP
#define VEHICLE_HPP

//! Vehicle class

//! This class is used to represent a vehicle.

#include "VehicleType.hpp"
#include <memory>
#include <vector>

class Vehicle {
private:
  static std::vector<std::shared_ptr<VehicleType>> _vehicleType;
  int _index;
  int _position;
  int _previousPosition = -1;
  int _street = -1;
  int _timePenalty = 0;
  double _velocity = 0.;
  int _timeTraveled = 0;

public:
  Vehicle(int); //!< create a vehicle of a type in _vehicleType
  ~Vehicle() = default;

  static void addVehicleType(int, int); //!< add a vehicle type in _vehicleType
  static void addVehicleType(const char *); //!< add many vehicle types in _vehicleType from file
  static std::shared_ptr<VehicleType> getVehicleType(int const); //!< get a vehicle type from _vehicleType
  static int getNVehicleType(); //!< get the number of vehicle types in _vehicleType
  std::shared_ptr<VehicleType> getVehicleType() const noexcept;
  int getType() const noexcept; // return the index of the vehicle type in _vehicleType
  void setPosition(int);
  int getPosition() const;
  int getPreviousPosition() const;
  void setStreet(int);
  int getStreet() const;
  int getDestination() const;
  void setTimePenalty(int);
  int getTimePenalty() const;
  void setVelocity(double);
  double getVelocity() const;
  void incrementTimeTraveled();
  int getTimeTraveled() const;
  void resetTimeTraveled();
};

#endif
