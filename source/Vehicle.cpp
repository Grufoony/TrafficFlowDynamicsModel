#include "Vehicle.hpp"
#include <fstream>
#include <stdexcept>

std::vector<std::shared_ptr<VehicleType>> Vehicle::_vehicleType;

Vehicle::Vehicle(int type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = getVehicleType()->getSource();
  } else {
    throw std::runtime_error("Error in Vehicle: VehicleType does not exist.\n");
  }
}

void Vehicle::addVehicleType(int src, int dst) {
  _vehicleType.push_back(std::make_shared<VehicleType>(VehicleType(src, dst)));
}
void Vehicle::addVehicleType(const char *fName) {
  std::fstream data;
  data.open(fName);
  int src, dst, n = 0;
  if (!data) {
    throw std::runtime_error("VehicleType file does not exist.\n");
  }
  while (data >> src) {
    ++n;
  }
  data.close();
  n = n / 2;
  data.open(fName);
  if (n > 0) {
    for (int i = 0; i < n; ++i) {
      data >> src >> dst;
      _vehicleType.push_back(
          std::make_shared<VehicleType>(VehicleType(src, dst)));
    }
  } else {
    throw std::runtime_error("VehicleType file is empty.\n");
  }
  data.close();
}
std::shared_ptr<VehicleType> Vehicle::getVehicleType(int const index) {
  if (index < 0 || index > getNVehicleType() - 1)
    throw std::invalid_argument("Error in getVehicleType.\n");
  return _vehicleType.at(index);
}
std::shared_ptr<VehicleType> Vehicle::getVehicleType() const noexcept {
  return _vehicleType.at(_index);
}
int Vehicle::getType() const noexcept { return _index; }
int Vehicle::getNVehicleType() { return static_cast<int>(_vehicleType.size()); }

void Vehicle::setPosition(int pos) {
  if (pos < 0)
    throw std::invalid_argument("Error in setPosition.\n");
  _position = pos;
}
int Vehicle::getPosition() const { return _position; }
void Vehicle::setStreet(int street) { _street = street; }
int Vehicle::getStreet() const { return _street; }
void Vehicle::setTimePenalty(int time) {
  if (time < 0)
    throw std::invalid_argument("Error in setTimePenalty.\n");
  _timePenalty = time;
}
int Vehicle::getDestination() const {
  return getVehicleType()->getDestination();
}
int Vehicle::getTimePenalty() const { return _timePenalty; }
void Vehicle::setVelocity(double vel) {
  if (vel < 0)
    throw std::invalid_argument("Error in setVelocity.\n");
  _velocity = vel;
}
double Vehicle::getVelocity() const { return _velocity; }
void Vehicle::incrementTimeTraveled() { ++_timeTraveled; }
int Vehicle::getTimeTraveled() const { return _timeTraveled; }
void Vehicle::resetTimeTraveled() { _timeTraveled = 0; }