#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "street.hpp"
#include <valarray>

namespace net {

class Network {
private:
  int lenght; // lunghezza = numero colonne
  int height; // altezza = numero righe
  std::valarray<Street> net;

public:
  Network(int, int);
  Street &operator()(int, int); // operatore per usare il network a matrice
  int getSize() const;
};

} // namespace net

#endif