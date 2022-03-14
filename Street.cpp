#include "Street.hpp"

#include <stdexcept>

double constexpr AVG_LENGHT = 4.;
double constexpr V_MAX = 50.; // max velocity for each street
double constexpr V_MIN = 7.;  // min velocity for each street

Street::Street(int n_1, int n_2, double l) {
  if (!(l > 0))
    throw std::invalid_argument("Error in Street.\n");
  _nodes[0] = n_1;
  _nodes[1] = n_2;
  _lenght = l;
  _capacity = static_cast<int>(_lenght / AVG_LENGHT);
}

int *Street::getNodes() { return _nodes; }
int Street::getLenght() const noexcept { return _lenght; }
int Street::getCapacity() const noexcept { return _capacity; }
int Street::getNVehicles() { return _nVehicles; }
double Street::getVelocity() {
  auto v = V_MAX - _nVehicles / _lenght;
  if (v < V_MIN) {
    return V_MIN;
  } else {
    return v;
  }
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }