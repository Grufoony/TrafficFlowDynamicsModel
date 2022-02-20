#include "street.hpp"

Street::Street() { n_v = 0; }
int Street::getLenght() const { return lenght; }
int Street::getCapacity() const { return capacity; }
int Street::getNVehicles() { return n_v; }
double Street::getVelocity() { return (v_max - k * n_v); }
void Street::addVehicle() { ++n_v; }
void Street::remVehicle() { --n_v; }