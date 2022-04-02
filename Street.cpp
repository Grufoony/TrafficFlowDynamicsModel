#include "Street.hpp"

#include <stdexcept>

double constexpr AVG_LENGHT = 4.;
double constexpr V_MAX = 50.; // max velocity for each street
double constexpr V_MIN = 7.;  // min velocity for each street

Street::Street(int n_1, int n_2, double l) {
  if (!(l > 0))
    throw std::invalid_argument("Error in Street.\n");
  _src = n_1;
  _dst = n_2;
  _lenght = l;
  _capacity = static_cast<int>(_lenght / AVG_LENGHT);
}

int Street::getOrigin() const noexcept { return _src; }
int Street::getDestination() const noexcept { return _dst; }
double Street::getLenght() const noexcept { return _lenght; }
bool Street::isFull() const noexcept { return _nVehicles == _capacity; }
// int Street::getCapacity() const noexcept { return _capacity; }
int Street::getNVehicles() const noexcept { return _nVehicles; }
double Street::getVelocity() {
  auto v = V_MAX - _nVehicles * AVG_LENGHT / _lenght;
  if (v < V_MIN) {
    return V_MIN;
  } else {
    return v;
  }
}
void Street::addVehicle() { ++_nVehicles; }
void Street::remVehicle() { --_nVehicles; }