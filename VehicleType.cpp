#include "VehicleType.hpp"
#include <stdexcept>

VehicleType::VehicleType(int src, int dst) {
  _trip[0] = src;
  _trip[1] = dst;
}

int VehicleType::getSource() const noexcept { return _trip[0]; }
int VehicleType::getDestination() const noexcept { return _trip[1]; }
void VehicleType::setTransMatrix(std::vector<std::vector<double>> &matrix) {
  int n = matrix.size();
  if (n == 0)
    throw std::runtime_error(
        "Error in setTransMatrix: matrix has dimension 0.\n");
  for (int i = 0; i < n; ++i) {
    std::vector<double> temp;
    for (int j = 0; j < n; ++j) {
      temp.push_back(matrix.at(i).at(j));
    }
    _transMatrix.push_back(temp);
  }
}
std::vector<std::vector<double>> &VehicleType::getTransMatrix() {
  if (_transMatrix.size() == 0)
    throw std::runtime_error(
        "Error in getTransMatrix: _transMatrix has dimension 0.\n");
  return _transMatrix;
}