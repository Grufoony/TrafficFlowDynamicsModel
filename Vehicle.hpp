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
  static std::shared_ptr<VehicleType> getVehicleType(int const);
  static int getNVehicleType();

  void setPosition(int);
  void setTimePenalty(int);
  int getTimePenalty();
};

#endif