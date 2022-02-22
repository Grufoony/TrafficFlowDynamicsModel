#ifndef VEHICLE_HPP
#define VEHICLE_HPP

class Vehicle {
private:
  int _type;
  // source e destination vengono definite come -1 0 +1 rispetto all'asse
  // centrale del reticolo
  int _source;
  int _destination;
  // coordinate nel network
  int _x = 0;
  int _y;

  double *_probability; // [0] -y || [1] +x || [2] +y || [3] -x

public:
  Vehicle(int, int);
  ~Vehicle();
  int getX();
  int getY();
};

#endif