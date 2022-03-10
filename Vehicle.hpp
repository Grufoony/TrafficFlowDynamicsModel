#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "VehicleType.hpp"
#include <vector>

class Vehicle {
private:
  static std::vector<VehicleType *> _vehicleType;
  int _index;
  int _position;
  int _timePenalty; // penalità di tempo da scontare
public:
  Vehicle(int);
  ~Vehicle();

  static void addVehicleType(int, int);
  static VehicleType *getVehicleType(int);
  static int getNVehicleType();

  void setPosition(int);
  void setTimePenalty(int);
  int getTimePenalty();
};

#endif