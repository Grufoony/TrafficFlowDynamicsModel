#include "Vehicle.hpp"

#include <stdexcept>

std::vector<VehicleType> Vehicle::_vehicleType;

Vehicle::Vehicle(int type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = _vehicleType[_index].getOrigin();
  } else {
    throw std::runtime_error("Vehicle type does not exist.\n");
  }
}

void Vehicle::addVehicleType(int src, int dst) {
  _vehicleType.push_back(VehicleType(src, dst));
}
int Vehicle::getNVehicleType() { return static_cast<int>(_vehicleType.size()); }
void Vehicle::setTimePenalty(int time) { _timePenalty = time; }
int Vehicle::getTimePenalty() { return _timePenalty; }