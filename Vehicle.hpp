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
  int _timePenalty; // penalità di tempo da scontare
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
  void setTimePenalty(int);
  int getTimePenalty();
};

#endif