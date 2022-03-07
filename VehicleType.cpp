#include "VehicleType.hpp"

VehicleType::VehicleType(int src, int dst) {
  _trip[0] = src;
  _trip[1] = dst;
}

int VehicleType::getOrigin() const noexcept { return _trip[0]; }
int VehicleType::getDestination() const noexcept { return _trip[1]; }
void VehicleType::setTransMatrix(std::vector<std::vector<double>> matrix) {
  int n = matrix.size();
  for (int i = 0; i < n; ++i) {
    std::vector<double> temp;
    for (int j = 0; j < n; ++j) {
      temp.push_back(matrix.at(i).at(j));
    }
    _transMatrix.push_back(temp);
  }
}
std::vector<std::vector<double>> VehicleType::getTransMatrix() {
  return _transMatrix;
}