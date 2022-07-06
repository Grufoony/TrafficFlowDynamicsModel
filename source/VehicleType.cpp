#include "VehicleType.hpp"
#include <stdexcept>

/// \brief Create a new VehicleType object.
/// \param src The source node.
/// \param dst The destination node.
VehicleType::VehicleType(int src, int dst) {
  _trip[0] = src;
  _trip[1] = dst;
}
int VehicleType::getSource() const noexcept { return _trip[0]; }
int VehicleType::getDestination() const noexcept { return _trip[1]; }
/// \brief Set the transition matrix.
/// \param transMatrix The transition matrix.
void VehicleType::setTransMatrix(std::vector<std::vector<double>> &matrix) {
  int n = matrix.size();
  if (n == 0)
    throw std::runtime_error("VehicleType::setTransMatrix: matrix is empty.\n");
  for (int i = 0; i < n; ++i) {
    std::vector<double> temp;
    for (int j = 0; j < n; ++j) {
      temp.push_back(matrix.at(i).at(j));
    }
    _transMatrix.push_back(temp);
  }
}
std::vector<std::vector<double>> const &VehicleType::getTransMatrix() const {
  if (_transMatrix.size() == 0)
    throw std::runtime_error("VehicleType::getTransMatrix: matrix is empty");
  return _transMatrix;
}