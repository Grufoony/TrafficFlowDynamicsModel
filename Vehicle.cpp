#include "Vehicle.hpp"

#include <stdexcept>

std::vector<VehicleType> Vehicle::_vehicleType;

Vehicle::Vehicle(int type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = _vehicleType[_index].getOrigin();
  } else {
    throw std::runtime_error("Error in Vehicle: VehicleType does not exist.\n");
  }
}

void Vehicle::addVehicleType(int src, int dst) {
  _vehicleType.push_back(VehicleType(src, dst));
}
VehicleType &Vehicle::getVehicleType(int index) {
  if (index < 0 || index > getNVehicleType() - 1)
    throw std::invalid_argument("Error in getVehicleType.\n");
  return _vehicleType.at(index);
}
int Vehicle::getNVehicleType() { return static_cast<int>(_vehicleType.size()); }

void Vehicle::setPosition(int pos) {
  if (pos < 0)
    throw std::invalid_argument("Error in setPosition.\n");
  _position = pos;
}
void Vehicle::setTimePenalty(int time) {
  if (time < 0)
    throw std::invalid_argument("Error in setTimePenalty.\n");
  _timePenalty = time;
}
int Vehicle::getTimePenalty() { return _timePenalty; }