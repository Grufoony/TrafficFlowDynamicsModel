#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "street.hpp"
#include "vehicle.hpp"
#include <valarray> // alloca meno memoria di un vector
#include <vector>

namespace net {

class Network {
private:
  int _lenght;                    // lunghezza = numero colonne
  int _height;                    // altezza = numero righe
  std::valarray<Street> _net;     // network di strade
  std::vector<Vehicle> _vehicles; // veicoli immessi nel sistema
  const double _temperature = 0.; // temperatura del network

  void updateProbability(Vehicle); // aggiorna il vettore probabilità di
                                   // spostamento del veicolo

public:
  Network(int, int);
  Street &operator()(int,
                     int) noexcept; // operatore per usare il network a matrice
  int getSize() const;
  void addVehicle(int);
  void test();
};

} // namespace net

#endif