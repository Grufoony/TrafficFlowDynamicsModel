#ifndef STREET_HPP
#define STREET_HPP

#include "vehicle.hpp"
#include <vector>

class Street {
private:
  static const int _lenght = 1;    // lunghezza della strada
  static const int _capacity = 10; // capacità massima di veicoli
  static const int _vMax = 50;     // velocità massima per ogni strada
  static const int _vMin = 7;      // velocità minima per ogni strada
  std::vector<Vehicle> _vehicles;  // veicoli presenti sulla strada
  double _k = 2.;                  // costante per la decrescita della velocità

public:
  int getLenght() const;
  int getCapacity() const;
  int getNVehicles();
  double getVelocity();
  void addVehicle(Vehicle);
  // void remVehicle();
};

#endif