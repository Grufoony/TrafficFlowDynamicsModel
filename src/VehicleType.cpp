#include "VehicleType.hpp"
#include <stdexcept>
#include <string>

/// @brief Create a new VehicleType object.
/// @param src The source node.
/// @param dst The destination node.
VehicleType::VehicleType(uint16_t src, uint16_t dst) {
  _trip[0] = src;
  _trip[1] = dst;
}
/// @brief Get the source node index.
/// @return The source node index.
uint16_t VehicleType::getSource() const noexcept { return _trip[0]; }
/// @brief Get the destination node index.
/// @return The destination node index.
uint16_t VehicleType::getDestination() const noexcept { return _trip[1]; }
/// @brief Set the transition matrix.
/// @param transMatrix The transition matrix.
void VehicleType::setTransMatrix(SparseMatrix<double> &matrix) {
  this->_transMatrix = matrix;
}
/// @brief Get the transition matrix.
/// @return The transition matrix.
/// @throw std::runtime_error If the transition matrix is empty.
SparseMatrix<double> const &VehicleType::getTransMatrix() const {
  if (_transMatrix.size() == 0) {
    std::string msg = "VehicleType.cpp:" + std::to_string(__LINE__) + '\t' +
                      "Transition matrix is empty.\n";
    throw std::runtime_error(msg);
  }
  return _transMatrix;
}
