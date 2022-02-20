#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "street.hpp"
#include "vehicle.hpp"
#include <valarray> // alloca meno memoria di un vector
#include <vector>

namespace net {

class Network {
private:
  int _lenght; // lunghezza = numero colonne
  int _height; // altezza = numero righe
  double _temperature;
  std::valarray<Street> _net;     // network di strade
  std::vector<Vehicle> _vehicles; // veicoli immessi nel sistema

public:
  Network(int, int);
  Street &operator()(int, int); // operatore per usare il network a matrice
  int getSize() const;
  void addVehicle(int);
};

} // namespace net

#endif