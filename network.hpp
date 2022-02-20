#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "street.hpp"
#include <valarray>

namespace net {

class Network {
private:
  int lenght;
  int height;
  std::valarray<Street> net;

public:
  Network(int, int);
  Street &operator()(int row, int column) {
    return net[row * lenght * height + column];
  }
  int getSize() const;
};

} // namespace net

#endif