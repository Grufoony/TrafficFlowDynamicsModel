#include "Street.hpp"

#include <limits>
#include <stdexcept>

double constexpr AVG_LENGHT =
    4.; // if < 0 then there's no limit on the capacity

Street::Street(int n_1, int n_2, double l) {
  if (!(l > 0))
    throw std::invalid_argument("Error in Street.\n");
  _src = n_1;
  _dst = n_2;
  _lenght = l;
  _nVehicles = 0;
  _nLanes = 1;
  _vMax = 1.39; // 1.39 m/s = 50 km/h
  if (AVG_LENGHT < 0) {
    _maxCapacity = std::numeric_limits<int>::max();
  } else {
    _maxCapacity = static_cast<int>(_lenght / AVG_LENGHT);
  }
}

int Street::getOrigin() const noexcept { return _src; }
int Street::getDestination() const noexcept { return _dst; }
double Street::getLenght() const noexcept { return _lenght; }
bool Street::isFull() const noexcept {
  return (_nVehicles == (_nLanes * _maxCapacity));
}
void Street::setNLanes(int n) noexcept {
  if (n > 0)
    _nLanes = n;
}
int Street::getNVehicles() const noexcept { return _nVehicles; }
void Street::setVMax(double v) {
  if (v < 0)
    throw std::invalid_argument("Error in setVMax.\n");
  _vMax = v;
}
double Street::getVelocity() const noexcept { // linear decay
  return (_vMax *
          (1 - (75e-2 * (static_cast<double>(_nVehicles) /
                         static_cast<double>((_nLanes * _maxCapacity))))));
}
double Street::getVMax() const noexcept { return _vMax; }
double Street::getVehicleDensity() const noexcept {
  return (static_cast<double>(_nVehicles) /
          static_cast<double>((_nLanes * _maxCapacity)));
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }