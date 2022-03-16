#include "Vehicle.hpp"
#include <stdexcept>

std::vector<std::shared_ptr<VehicleType>> Vehicle::_vehicleType;

Vehicle::Vehicle(int type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = _vehicleType[_index]->getSource();
  } else {
    throw std::runtime_error("Error in Vehicle: VehicleType does not exist.\n");
  }
}
Vehicle::~Vehicle() {
  // for (auto it : _vehicleType) {
  //   delete it;
  // }
  // _vehicleType.clear();
}

void Vehicle::addVehicleType(int src, int dst) {
  _vehicleType.push_back(std::make_shared<VehicleType>(VehicleType(src, dst)));
}
std::shared_ptr<VehicleType> Vehicle::getVehicleType(int index) {
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