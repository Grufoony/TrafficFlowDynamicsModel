#ifndef STREET_HPP
#define STREET_HPP

class Street {
private:
  static const int _lenght = 1;    // lunghezza della strada
  static const int _capacity = 10; // capacità massima di veicoli
  static const int _vMax = 50;     // velocità massima per ogni strada
  static const int _vMin = 7;      // velocità minima per ogni strada
  int _nVehicles;                  // veicoli presenti sulla strada
  double _k = 2.;                  // costante per la decrescita della velocità

public:
  int getLenght() const;
  int getCapacity() const;
  int getNVehicles();
  double getVelocity();
  void addVehicle();
  void remVehicle();
};

#endif