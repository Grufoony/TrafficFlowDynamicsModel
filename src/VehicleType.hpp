#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

#include "../utils/SparseMatrix.hpp"
#include <cstdint>
#include <vector>

//! @brief VehicleType class

//! This class is used to represent a vehicle type. It contains the source and
//! destination nodes, and the transition matrix.

class VehicleType {
private:
  uint16_t _trip[2];                 // 0=source, 1=destination
  SparseMatrix<double> _transMatrix; // transition matrix

public:
  VehicleType(uint16_t, uint16_t);
  ~VehicleType() = default;

  uint16_t getSource() const noexcept;
  uint16_t getDestination() const noexcept;
  void setTransMatrix(SparseMatrix<double> &); //!< set the transition matrix
  SparseMatrix<double> const &getTransMatrix() const;
};

#endif
