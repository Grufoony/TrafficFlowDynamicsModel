#ifndef STREET_HPP
#define STREET_HPP

#include "vehicle.hpp"
#include <vector>

class Street {
private:
  static const int lenght = 1;    // lunghezza della strada
  static const int capacity = 10; // capacità massima di veicoli
  static const int v_max = 50;    // velocità massima per ogni strada
  static const int v_min = 7;     // velocità minima per ogni strada
  std::vector<Vehicle> vehicles;  // veicoli presenti sulla strada
  double k = 2.;                  // costante per la decrescita della velocità

public:
  int getLenght() const;
  int getCapacity() const;
  int getNVehicles();
  double getVelocity();
  void addVehicle(Vehicle);
  // void remVehicle();
};

#endif