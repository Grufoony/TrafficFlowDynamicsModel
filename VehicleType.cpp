#include "VehicleType.hpp"

VehicleType::VehicleType(int src, int dst) {
  _trip[0] = src;
  _trip[1] = dst;
}

int VehicleType::getOrigin() const { return _trip[0]; }
int VehicleType::getDestination() const { return _trip[1]; }