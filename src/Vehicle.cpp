#include "Vehicle.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

std::vector<std::shared_ptr<VehicleType>> Vehicle::_vehicleType;

/// @brief Create a new Vehicle object.
/// @param type The index of the vehicle type in _vehicleType.
/// @throw std::invalid_argument if the vehicle type index is out of range.
Vehicle::Vehicle(int type) {
  if (!(type < static_cast<int>(_vehicleType.size()))) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Vehicle type index out of range.\n";
    throw std::invalid_argument(msg);
  }
  _index = type;
  _position = Vehicle::getVehicleType(this->getType())->getSource();
}
/// @brief Add a vehicle type in _vehicleType.
/// @param src The source node.
/// @param dst The destination node.
/// @throw std::invalid_argument if the vehicle type already exists.
void Vehicle::addVehicleType(int src, int dst) {
  for (auto &v : _vehicleType) {
    if (v->getSource() == src && v->getDestination() == dst) {
      std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                        "Vehicle type already exists.\n";
      throw std::invalid_argument(msg);
    }
  }
  _vehicleType.push_back(std::make_shared<VehicleType>(VehicleType(src, dst)));
}
/// @brief Add many vehicle types in _vehicleType from file.
/// @param fName The file name.
/// @throw std::runtime_error if the file is not found or empty.
/// @throw std::invalid_argument if the vehicle type already exists.
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
  if (!(n > 0)) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Vehicle type file: \'" + fName + " \' is empty.\n";
    throw std::runtime_error(msg);
  }
  for (int i = 0; i < n; ++i) {
    data >> src >> dst;
    for (auto &v : _vehicleType) {
      if (v->getSource() == src && v->getDestination() == dst) {
        std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                          "Vehicle type already exists.\n";
        throw std::invalid_argument(msg);
      }
    }
    _vehicleType.push_back(
        std::make_shared<VehicleType>(VehicleType(src, dst)));
  }
  data.close();
}
/// @brief Get the vehicle type.
/// @return A pointer to the vehicle type object.
/// @throw std::invalid_argument if the vehicle type index is out of range.
std::shared_ptr<VehicleType> Vehicle::getVehicleType(int8_t const index) {
  if (index < 0 || index > getNVehicleType() - 1) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Index out of range.\n";
    throw std::invalid_argument(msg);
  }
  return _vehicleType[index];
}
/// @brief Get the vehicle type index.
/// @return The vehicle type index.
uint8_t Vehicle::getType() const noexcept { return _index; }
/// @brief Get the number of vehicle types.
/// @return The number of vehicle types.
uint8_t Vehicle::getNVehicleType() {
  return static_cast<int>(_vehicleType.size());
}
/// @brief Set the position of the vehicle.
/// @param pos The position of the vehicle, i.e. a positive integrer
/// representing the index of the node.
/// @throw std::invalid_argument if the position is negative.
void Vehicle::setPosition(int pos) {
  if (pos < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Position must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _previousPosition = _position;
  _position = pos;
}
/// @brief Get the position of the vehicle.
/// @return The position of the vehicle, i.e. a positive integrer representing
/// the index of the node.
uint16_t Vehicle::getPosition() const noexcept { return _position; }
/// @brief Get the previous position of the vehicle.
/// @return The previous position of the vehicle, i.e. a positive integrer
/// representing the index of the node. The previous position is -1 if the
/// vehicle has not moved yet.
int Vehicle::getPreviousPosition() const noexcept { return _previousPosition; }
/// @brief Set the street of the vehicle.
/// @param street The street of the vehicle, i.e.
/// a positive integrer representing the index of the street.
/// The -1 is a jolly.
void Vehicle::setStreet(int street) { _street = street; }
/// @brief Get the street of the vehicle.
/// @return The street of the vehicle, i.e.
/// a positive integrer representing the index of the street.
int Vehicle::getStreet() const { return _street; }
/// @brief Set the time penalty of the vehicle.
/// @param time The time penalty of the vehicle, i.e. a positive integrer.
/// @throw std::invalid_argument if the time is negative.
void Vehicle::setTimePenalty(int time) {
  if (time < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Time must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _timePenalty = time;
}
/// @brief Get the destination of the vehicle.
/// @return The destination of the vehicle.
uint16_t Vehicle::getDestination() const noexcept {
  return Vehicle::getVehicleType(this->getType())->getDestination();
}
/// @brief Get the time penalty of the vehicle.
/// @return The time penalty of the vehicle.
int Vehicle::getTimePenalty() const noexcept { return _timePenalty; }
/// @brief Set the velocity of the vehicle.
/// @param vel The velocity of the vehicle.
/// @throw std::invalid_argument if the velocity is negative.
void Vehicle::setVelocity(double vel) {
  if (vel < 0) {
    std::string msg = "Vehicle.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Velocity must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _velocity = vel;
}
/// @brief Get the velocity of the vehicle.
/// @return The velocity of the vehicle.
double Vehicle::getVelocity() const noexcept { return _velocity; }
/// @brief Increment the time traveled by the vehicle.
void Vehicle::incrementTimeTraveled() noexcept { ++_timeTraveled; }
/// @brief Get the time traveled by the vehicle.
/// @return The time traveled by the vehicle.
int Vehicle::getTimeTraveled() const noexcept { return _timeTraveled; }
/// @brief Reset the time traveled by the vehicle, setting it to 0.
void Vehicle::resetTimeTraveled() noexcept { _timeTraveled = 0; }
