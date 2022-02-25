#include "VehicleType.hpp"

VehicleType::VehicleType(int origin, int destination) {
  _trip[0] = origin;
  _trip[1] = destination;
}

int VehicleType::getOrigin() const { return _trip[0]; }
int VehicleType::getDestination() const { return _trip[1]; }