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
  } else {
    _maxCapacity = static_cast<int>(_length / _avgLength);
  }
}
int Street::getOrigin() const noexcept { return _src; }
int Street::getDestination() const noexcept { return _dst; }
int Street::getIndex() const noexcept { return _index; }
double Street::getLength() const noexcept { return _length; }
/// @brief Tells if the street is full.
/// @return True if the street is full, false otherwise.
bool Street::isFull() const noexcept {
  return (_nVehicles == (_nLanes * _maxCapacity));
}
/// @brief Set the number of lanes.
/// @param nLanes The number of lanes.
void Street::setNLanes(int n) noexcept {
  if (n > 0)
    _nLanes = n;
}
int Street::getNVehicles() const noexcept { return _nVehicles; }
/// @brief Set the maximum velocity.
/// @param vMax The maximum velocity.
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
void Street::remVehicle() {
  --_nVehicles;
  if (_nVehicles < 0) {
    std::string msg =
        "Street.cpp:" + std::to_string(__LINE__) + '\t' + "nVehicles < 0.\n";
    throw std::runtime_error(msg);
  }
}
