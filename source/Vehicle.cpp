#include "Vehicle.hpp"
#include <fstream>
#include <stdexcept>

std::vector<std::shared_ptr<VehicleType>> Vehicle::_vehicleType;

/// \brief Create a new Vehicle object.
/// \param type The index of the vehicle type in _vehicleType.
Vehicle::Vehicle(int type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = getVehicleType()->getSource();
  } else {
    throw std::invalid_argument("Vehicle::Vehicle: type out of range.\n");
  }
}
/// \brief Add a vehicle type in _vehicleType.
/// \param src The source node.
/// \param dst The destination node.
void Vehicle::addVehicleType(int src, int dst) {
  _vehicleType.push_back(std::make_shared<VehicleType>(VehicleType(src, dst)));
}
void Vehicle::addVehicleType(const char *fName) {
  std::fstream data;
  data.open(fName);
  int src, dst, n = 0;
  if (!data) {
    throw std::runtime_error("Vehicle::addVehicleType: file not found.\n");
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
    throw std::runtime_error("Vehicle::addVehicleType: file empty.\n");
  }
  data.close();
}
/// \brief Get the vehicle type.
/// \return A pointer to the vehicle type object.
std::shared_ptr<VehicleType> Vehicle::getVehicleType(int const index) {
  if (index < 0 || index > getNVehicleType() - 1)
    throw std::invalid_argument("Vehicle::getVehicleType: index out of range.\n");
  return _vehicleType.at(index);
}
/// \brief Get the vehicle type.
/// \return A pointer to the vehicle type object of this istance.
std::shared_ptr<VehicleType> Vehicle::getVehicleType() const noexcept {
  return _vehicleType.at(_index);
}
int Vehicle::getType() const noexcept { return _index; }
int Vehicle::getNVehicleType() { return static_cast<int>(_vehicleType.size()); }

void Vehicle::setPosition(int pos) {
  if (pos < 0)
    throw std::invalid_argument("Vehicle::setPosition: pos out of range.\n");
  _previousPosition = _position;
  _position = pos;
}
int Vehicle::getPosition() const { return _position; }
int Vehicle::getPreviousPosition() const { return _previousPosition; }
void Vehicle::setStreet(int street) { _street = street; }
int Vehicle::getStreet() const { return _street; }
void Vehicle::setTimePenalty(int time) {
  if (time < 0)
    throw std::invalid_argument("Vehicle::setTimePenalty: time must be greater than 0.\n");
  _timePenalty = time;
}
int Vehicle::getDestination() const {
  return getVehicleType()->getDestination();
}
int Vehicle::getTimePenalty() const { return _timePenalty; }
void Vehicle::setVelocity(double vel) {
  if (vel < 0)
    throw std::invalid_argument("Vehicle ::setVelocity: vel must be greater than 0.\n");
  _velocity = vel;
}
double Vehicle::getVelocity() const { return _velocity; }
/// \brief Increment the time traveled by the vehicle.
void Vehicle::incrementTimeTraveled() { ++_timeTraveled; }
int Vehicle::getTimeTraveled() const { return _timeTraveled; }
void Vehicle::resetTimeTraveled() { _timeTraveled = 0; }