#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "VehicleType.hpp"

class Vehicle {
private:
  static const int _maxNumVehicleType = 9;
  static int _nVehicleType;
  static VehicleType *_vehicleType[_maxNumVehicleType];
  int _index;
  int _timePenalty; // penalità di tempo da scontare
public:
  Vehicle(int);
  void addVehicleType(int, int);
  void setTimePenalty(int);
  int getTimePenalty();
};

#endif