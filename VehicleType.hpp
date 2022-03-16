#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

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
  void setTransMatrix(std::vector<std::vector<double>> &);
  std::vector<std::vector<double>> const &getTransMatrix() const;
};

#endif