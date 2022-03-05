#include "VehicleType.hpp"

VehicleType::VehicleType(int src, int dst) {
  _trip[0] = src;
  _trip[1] = dst;
}

int VehicleType::getOrigin() const noexcept { return _trip[0]; }
int VehicleType::getDestination() const noexcept { return _trip[1]; }