#include "Vehicle.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

std::vector<std::shared_ptr<VehicleType>> Vehicle::_vehicleType;

/// @brief Create a new Vehicle object.
/// @param type The index of the vehicle type in _vehicleType.
Vehicle::Vehicle(uint8_t type) {
  if (type < static_cast<int>(_vehicleType.size())) {
    _index = type;
    _position = Vehicle::getVehicleType(this->getType())->getSource();
  } else {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Vehicle type index out of range.\n";
    throw std::invalid_argument(msg);
  }
}
/// @brief Add a vehicle type in _vehicleType.
/// @param src The source node.
/// @param dst The destination node.
void Vehicle::addVehicleType(uint16_t src, uint16_t dst) {
  _vehicleType.push_back(std::make_shared<VehicleType>(VehicleType(src, dst)));
}
void Vehicle::addVehicleType(std::string fName) {
  std::fstream data;
  data.open(fName);
  uint16_t src, dst, n = 0;
  if (!data) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Vehicle type file not found in the given path: \'" +
                      fName + " \'\n";
    throw std::runtime_error(msg);
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
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Vehicle type file: \'" + fName + " \' is empty.\n";
    throw std::runtime_error(msg);
  }
  data.close();
}
/// @brief Get the vehicle type.
/// @return A pointer to the vehicle type object.
std::shared_ptr<VehicleType> Vehicle::getVehicleType(int8_t const index) {
  if (index < 0 || index > getNVehicleType() - 1) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Index out of range.\n";
    throw std::invalid_argument(msg);
  }
  return _vehicleType[index];
}
uint8_t Vehicle::getType() const noexcept { return _index; }
uint8_t Vehicle::getNVehicleType() {
  return static_cast<int>(_vehicleType.size());
}

void Vehicle::setPosition(int16_t pos) {
  if (pos < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Position must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _previousPosition = _position;
  _position = pos;
}
uint16_t Vehicle::getPosition() const { return _position; }
int16_t Vehicle::getPreviousPosition() const { return _previousPosition; }
void Vehicle::setStreet(int street) { _street = street; }
int Vehicle::getStreet() const { return _street; }
void Vehicle::setTimePenalty(int time) {
  if (time < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Time must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _timePenalty = time;
}
uint16_t Vehicle::getDestination() const {
  return Vehicle::getVehicleType(this->getType())->getDestination();
}
int Vehicle::getTimePenalty() const { return _timePenalty; }
void Vehicle::setVelocity(double vel) {
  if (vel < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Velocity must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _velocity = vel;
}
double Vehicle::getVelocity() const { return _velocity; }
/// @brief Increment the time traveled by the vehicle.
void Vehicle::incrementTimeTraveled() { ++_timeTraveled; }
int Vehicle::getTimeTraveled() const { return _timeTraveled; }
void Vehicle::resetTimeTraveled() { _timeTraveled = 0; }
