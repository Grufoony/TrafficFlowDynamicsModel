#include "street.hpp"

int Street::getLenght() const noexcept { return _lenght; }
int Street::getCapacity() const noexcept { return _capacity; }
int Street::getNVehicles() { return _nVehicles; }
double Street::getVelocity() {
  auto v = _vMax - _k * _nVehicles;
  if (v < _vMin) {
    return _vMin;
  } else {
    return v;
  }
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }