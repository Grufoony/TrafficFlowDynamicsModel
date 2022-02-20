#include "network.hpp"

#include <cassert>

// Street class implementations

net::Street::Street() { n_v = 0; }
int net::Street::getLenght() const { return lenght; }
int net::Street::getCapacity() const { return capacity; }
int net::Street::getNVehicles() { return n_v; }
double net::Street::getVelocity() { return (v_max - k * n_v); }
void net::Street::addVehicle() { ++n_v; }
void net::Street::remVehicle() { --n_v; }

// Vehicle class implementation

net::Vehicle::Vehicle(int t) : type{t} { assert(type > -1 && type < 9); }

// Network class implementation

net::Network::Network(int x, int y) : lenght{x}, height{y} {
  assert(lenght > 1 && lenght % 2 != 0);
  assert(height > 3 && height % 2 != 0);
}