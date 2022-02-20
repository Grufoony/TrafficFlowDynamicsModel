#include "street.hpp"

int Street::getLenght() const { return lenght; }
int Street::getCapacity() const { return capacity; }
int Street::getNVehicles() { return vehicles.size(); }
double Street::getVelocity() {
  auto v = v_max - k * vehicles.size();
  if (v < v_min) {
    return v_min;
  } else {
    return v;
  }
}
void Street::addVehicle(Vehicle v) { vehicles.push_back(v); }
// void Street::remVehicle() { --n_v; }