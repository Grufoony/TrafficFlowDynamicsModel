#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

#include <vector>

class VehicleType {
private:
  int _trip[2];                                  // 0=source, 1=destination
  std::vector<std::vector<double>> _transMatrix; // transition matrix

public:
  VehicleType(int, int);
  int getOrigin() const noexcept;
  int getDestination() const noexcept;
};

#endif