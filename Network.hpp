#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Street.hpp"
#include "Vehicle.hpp"
// #include <valarray> // alloca meno memoria di un vector
#include <vector>

namespace net {

class Network {
private:
  int _lenght; // lunghezza = numero colonne
  int _height; // altezza = numero righe
  int _nNodes; // numero totale di nodi nella rete
  std::vector<bool> _nodeMatrix;
  // std::valarray<Street> _net;     // network di strade
  // std::vector<Vehicle> _vehicles; // veicoli immessi nel sistema
  // const double _temperature = 0.; // temperatura del network

public:
  // Network(int, int);
  Street &operator()(int,
                     int) noexcept; // operatore per usare il network a matrice
  void importNodeMatrix(const char *);
  void importMatrix();
  int getSize() const;
  void addVehicle(int);
};

} // namespace net

#endif