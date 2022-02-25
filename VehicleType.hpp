#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

class VehicleType {
private:
  int _trip[2]; // 0=source, 1=destination

public:
  VehicleType(int, int);
  int getOrigin() const;
  int getDestination() const;
};

#endif