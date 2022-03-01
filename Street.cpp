#include "Street.hpp"

Street::Street(int n_1, int n_2) {
  _nodes[0] = n_1;
  _nodes[1] = n_2;
}

int *Street::getNodes() { return _nodes; }
int Street::getLenght() const noexcept { return _lenght; }
int Street::getCapacity() const noexcept { return _capacity; }
int Street::getNVehicles() { return _nVehicles; }
double Street::getVelocity() {
  auto v = _vMax - _nVehicles / _lenght;
  if (v < _vMin) {
    return _vMin;
  } else {
    return v;
  }
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }