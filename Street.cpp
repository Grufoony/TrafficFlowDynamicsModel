#include "Street.hpp"

#include <limits>
#include <stdexcept>

double constexpr AVG_LENGHT =
    1e-1;                      // if < 0 then there's no limit on the capacity
double constexpr V_MAX = 4e-1; // max velocity for each street

Street::Street(int n_1, int n_2, double l) {
  if (!(l > 0))
    throw std::invalid_argument("Error in Street.\n");
  _src = n_1;
  _dst = n_2;
  _lenght = l;
  if (AVG_LENGHT < 0) {
    _maxCapacity = std::numeric_limits<int>::max();
  } else {
    _maxCapacity = static_cast<int>(_lenght / AVG_LENGHT);
  }
}

int Street::getOrigin() const noexcept { return _src; }
int Street::getDestination() const noexcept { return _dst; }
double Street::getLenght() const noexcept { return _lenght; }
bool Street::isFull() const noexcept { return (_nVehicles == _maxCapacity); }
int Street::getNVehicles() const noexcept { return _nVehicles; }
double Street::getVelocity() const noexcept { // linear decay
  // double test = _nVehicles / _maxCapacity;
  return (V_MAX * (1 - (75e-2 * (static_cast<double>(_nVehicles) /
                                 static_cast<double>(_maxCapacity)))));
  // return test;
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }