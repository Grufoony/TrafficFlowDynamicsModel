#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

//! VehicleType class

//! This class is used to represent a vehicle type.

#include <vector>

class VehicleType {
private:
  int _trip[2];                                  // 0=source, 1=destination
  std::vector<std::vector<double>> _transMatrix; // transition matrix

public:
  VehicleType(int, int);
  ~VehicleType() = default;

  int getSource() const noexcept;
  int getDestination() const noexcept;
  void setTransMatrix(std::vector<std::vector<double>> &); //!< set the transition matrix
  std::vector<std::vector<double>> const &getTransMatrix() const;
};

#endif