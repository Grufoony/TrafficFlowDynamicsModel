#include "Street.hpp"
#include <limits>
#include <stdexcept>
#include <string>

double Street::_avgLength = 8.;
double Street::_vMin = 75e-2;

/// @brief Create a new Street object.
/// @param src The source node.
/// @param dst The destination node.
/// @param length The length of the street.
/// @param index The index of the street.
/// @throw std::invalid_argument If the length is not positive or if the index
/// is negative.
Street::Street(int src, int dst, double length, int index) {
  if (!(length > 0)) {
    std::string msg = "Street.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Street length must be positive.\n";
    throw std::invalid_argument(msg);
  }
  if (index < 0) {
    std::string msg = "Street.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Street index must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _src = src;
  _dst = dst;
  _length = length;
  _nVehicles = 0;
  _nLanes = 1;
  _vMax = 13.9; // 13.9 m/s = 50 km/h
  _index = index;
  if (_avgLength < 0) {
    _maxCapacity = std::numeric_limits<int>::max();
  }
  _maxCapacity = static_cast<int>(_length / _avgLength);
}
/// @brief Get the origin node index.
/// @return The origin node index.
int Street::getOrigin() const noexcept { return _src; }
/// @brief Get the destination node index.
/// @return The destination node index.
int Street::getDestination() const noexcept { return _dst; }
/// @brief Get the index of the street.
/// @return The index of the street.
int Street::getIndex() const noexcept { return _index; }
/// @brief Get the length of the street.
/// @return The length of the street.
double Street::getLength() const noexcept { return _length; }
/// @brief Tells if the street is full.
/// @return True if the street is full, false otherwise.
bool Street::isFull() const noexcept {
  return (_nVehicles == (_nLanes * _maxCapacity));
}
/// @brief Set the number of lanes.
/// @param nLanes The number of lanes.
/// @throw std::invalid_argument If the number of lanes is not positive.
void Street::setNLanes(int n) {
  std::string msg = "Street.cpp:" + std::to_string(__LINE__) + '\t' +
                    "Number of lanes must be positive.\n";
  n > 0 ? _nLanes = n : throw std::invalid_argument(msg);
}
int Street::getNLanes() const noexcept { return _nLanes; }
/// @brief Get the number of vehicles on the street.
/// @return The number of vehicles on the street.
int Street::getNVehicles() const noexcept { return _nVehicles; }
/// @brief Set the maximum velocity.
/// @param vMax The maximum velocity.
/// @throw std::invalid_argument If the maximum velocity is negative.
void Street::setVMax(double v) {
  if (v < 0) {
    std::string msg = "Street.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Max velocity must be positive.\n";
    throw std::invalid_argument(msg);
  }
  _vMax = v;
}
/// @brief Get the input velocity.
/// @details Get the entering velocity of a new vehicle based on the instant
/// vehicle density of this street. The used formula is \f$v(t) = v_{max}\left(
/// 1 - k\frac{\rho(t)}{\rho_{max}} \right)\f$
/// @return The input velocity of a vehicle immediately after its arrival on the
/// street.
double Street::getInputVelocity() const noexcept { // linear decay
  return (_vMax *
          (1 - (_vMin * (static_cast<double>(_nVehicles) /
                         static_cast<double>((_nLanes * _maxCapacity))))));
}
/// @brief Get the maximum velocity.
/// @return The maximum velocity.
double Street::getVMax() const noexcept { return _vMax; }
/// @brief Get the density of the street.
double Street::getDensity() const noexcept {
  return (static_cast<double>(_nVehicles) /
          static_cast<double>((_nLanes * _maxCapacity)));
}
/// @brief Get the vehicle density of the street.
double Street::getVehicleDensity() const noexcept {
  return (_nVehicles / _length);
}
/// @brief Add a vehicle to the street.
/// Adds a vehicle to the street and changes the vehicle's parameters.
/// @param vehicle The vehicle to add.
void Street::addVehicle(std::shared_ptr<Vehicle> vehicle) {
  vehicle->setStreet(_index);
  vehicle->setVelocity(this->getInputVelocity());
  vehicle->setTimePenalty(static_cast<int>(_length / vehicle->getVelocity()));
  vehicle->setPosition(_dst);
  ++_nVehicles;
}
/// @brief Remove a vehicle from the street.
/// Removes a vehicle from the street and changes the vehicle's parameters.
/// @throw std::runtime_error If the number of vehicles is negative.
void Street::remVehicle() {
  std::string msg =
      "Street.cpp:" + std::to_string(__LINE__) + '\t' + "nVehicles < 0.\n";
  _nVehicles > 0 ? --_nVehicles : throw std::runtime_error(msg);
}
