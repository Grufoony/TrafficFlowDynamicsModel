#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

namespace net {

class Street {
private:
  static const int lenght = 1;    // lunghezza della strada
  static const int capacity = 10; // capacità massima di veicoli
  static const int v_max = 50;    // velocità massima per ogni strada
  int n_v;                        // numero di veicoli presenti
  double k = 2.;                  // costante per la decrescita della velocità

public:
  Street();
  int getLenght() const;
  int getCapacity() const;
  int getNVehicles();
  double getVelocity();
  void addVehicle();
  void remVehicle();
};

class Vehicle {
private:
  int type;
  int source;
  int destination;

public:
  Vehicle(int t);
};

class Network {
private:
  int lenght;
  int height;
  std::vector<std::vector<Street>> net;

public:
  Network(int, int);
};

} // namespace net

#endif