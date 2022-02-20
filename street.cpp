#include "street.hpp"

int Street::getLenght() const { return _lenght; }
int Street::getCapacity() const { return _capacity; }
int Street::getNVehicles() { return _vehicles.size(); }
double Street::getVelocity() {
  auto v = _vMax - _k * _vehicles.size();
  if (v < _vMin) {
    return _vMin;
  } else {
    return v;
  }
}
void Street::addVehicle(Vehicle v) { _vehicles.push_back(v); }
// void Street::remVehicle() { --n_v; }