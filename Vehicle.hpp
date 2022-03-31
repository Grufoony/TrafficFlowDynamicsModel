#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "VehicleType.hpp"
#include <memory>
#include <vector>

class Vehicle {
private:
  static std::vector<std::shared_ptr<VehicleType>> _vehicleType;
  int _index;
  int _position;
  int _street = -1;
  int _timePenalty = 0; // penalità di tempo da scontare
public:
  Vehicle(int);
  ~Vehicle() = default;

  static void addVehicleType(int, int);
  static void addVehicleType(const char *);
  static std::shared_ptr<VehicleType> getVehicleType(int const);
  static int getNVehicleType();

  std::shared_ptr<VehicleType> getVehicleType() const;
  void setPosition(int);
  int getPosition() const;
  void setStreet(int);
  int getStreet() const;
  int getDestination() const;
  void setTimePenalty(int);
  int getTimePenalty();
};

#endif