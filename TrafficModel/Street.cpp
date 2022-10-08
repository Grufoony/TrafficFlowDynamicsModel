#include "Street.hpp"

#include <limits>
#include <stdexcept>

double constexpr AVG_LENGHT =
    8.;                         // if < 0 then there's no limit on the capacity
double constexpr V_MIN = 75e-2; // minimum velocity

/// \brief Create a new Street object.
/// \param src The source node.
/// \param dst The destination node.
/// \param lenght The lenght of the street.
/// \param index The index of the street.
Street::Street(int src, int dst, double lenght, int index) {
  if (!(lenght > 0))
    throw std::invalid_argument("Street::Street: lenght must be positive.\n");
  if (index < 0)
    throw std::invalid_argument("Street::Street: index must be positive.\n");
  _src = src;
  _dst = dst;
  _lenght = lenght;
  _nVehicles = 0;
  _nLanes = 1;
  _vMax = 13.9; // 13.9 m/s = 50 km/h
  _index = index;
  if (AVG_LENGHT < 0) {
    _maxCapacity = std::numeric_limits<int>::max();
  } else {
    _maxCapacity = static_cast<int>(_lenght / AVG_LENGHT);
  }
}
int Street::getOrigin() const noexcept { return _src; }
int Street::getDestination() const noexcept { return _dst; }
int Street::getIndex() const noexcept { return _index; }
double Street::getLenght() const noexcept { return _lenght; }
/// \brief Tells if the street is full.
/// \return True if the street is full, false otherwise.
bool Street::isFull() const noexcept {
  return (_nVehicles == (_nLanes * _maxCapacity));
}
/// \brief Set the number of lanes.
/// \param nLanes The number of lanes.
void Street::setNLanes(int n) noexcept {
  if (n > 0)
    _nLanes = n;
}
int Street::getNVehicles() const noexcept { return _nVehicles; }
/// \brief Set the maximum velocity.
/// \param vMax The maximum velocity.
void Street::setVMax(double v) {
  if (v < 0)
    throw std::invalid_argument("Street::setVMax: vMax must be positive.\n");
  _vMax = v;
}
/// \brief Get the input velocity.
/// \return The input velocity of a vehicle immediately after its arrival on the
/// street.
double Street::getInputVelocity() const noexcept { // linear decay
  return (_vMax *
          (1 - (V_MIN * (static_cast<double>(_nVehicles) /
                         static_cast<double>((_nLanes * _maxCapacity))))));
}
double Street::getVMax() const noexcept { return _vMax; }
/// \brief Get the density of the street.
double Street::getDensity() const noexcept {
  return (static_cast<double>(_nVehicles) /
          static_cast<double>((_nLanes * _maxCapacity)));
}
/// \brief Get the vehicle density of the street.
double Street::getVehicleDensity() const noexcept {
  return (_nVehicles / _lenght);
}
/// \brief Add a vehicle to the street.
/// Adds a vehicle to the street and changes the vehicle's parameters.
/// \param vehicle The vehicle to add.
void Street::addVehicle(std::shared_ptr<Vehicle> vehicle) {
  vehicle->setStreet(_index);
  vehicle->setVelocity(this->getInputVelocity());
  vehicle->setTimePenalty(static_cast<int>(_lenght / vehicle->getVelocity()));
  vehicle->setPosition(_dst);
  ++_nVehicles;
}
void Street::remVehicle() {
  --_nVehicles;
  if (_nVehicles < 0)
    throw std::runtime_error("Street::remVehicle: nVehicles < 0.\n");
}