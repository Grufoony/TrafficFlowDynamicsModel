#include "network.hpp"
#include "street.hpp"

#include <cassert>

net::Network::Network(int x, int y) : lenght{x}, height{y} {
  assert(lenght > 1);
  assert(height > 3 && height % 2 != 0);
  net.resize(height * lenght);
}

Street &net::Network::operator()(int row, int column) {
  return net[row * lenght * height + column];
}

int net::Network::getSize() const { return net.size(); }