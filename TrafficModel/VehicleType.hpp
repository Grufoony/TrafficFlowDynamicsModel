#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

//! VehicleType class

//! This class is used to represent a vehicle type.

#include <vector>

class VehicleType {
private:
  uint8_t _trip[2];                                  // 0=source, 1=destination
  std::vector<std::vector<double>> _transMatrix; // transition matrix

public:
  VehicleType(uint8_t, uint8_t);
  ~VehicleType() = default;

  uint8_t getSource() const noexcept;
  uint8_t getDestination() const noexcept;
  void setTransMatrix(
		  std::vector<std::vector<double>> &); //!< set the transition matrix
  std::vector<std::vector<double>> const &getTransMatrix() const;
};

#endif
