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
  // temperatura
  static constexpr double _temperature = 0.;

public:
  Vehicle(int, int);
};

#endif