#include "VehicleType.hpp"
#include <stdexcept>

/// \brief Create a new VehicleType object.
/// \param src The source node.
/// \param dst The destination node.
VehicleType::VehicleType(uint16_t src, uint16_t dst) {
  _trip[0] = src;
  _trip[1] = dst;
}
uint16_t VehicleType::getSource() const noexcept { return _trip[0]; }
uint16_t VehicleType::getDestination() const noexcept { return _trip[1]; }
/// \brief Set the transition matrix.
/// \param transMatrix The transition matrix.
void VehicleType::setTransMatrix(std::vector<std::vector<double>> &matrix) {
  int n = matrix.size();
  if (n == 0)
    throw std::runtime_error("VehicleType::setTransMatrix: matrix is empty.\n");
  _transMatrix.clear();
  _transMatrix.reserve(n);
  for (auto const &row : matrix) {
    std::vector<double> temp;
    temp.reserve(n);
    for (auto const &el : row) {
      temp.push_back(el);
    }
    _transMatrix.push_back(temp);
  }
  _transMatrix.shrink_to_fit();
}
std::vector<std::vector<double>> const &VehicleType::getTransMatrix() const {
  if (_transMatrix.size() == 0)
    throw std::runtime_error("VehicleType::getTransMatrix: matrix is empty");
  return _transMatrix;
}
