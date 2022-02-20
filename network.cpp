#include "network.hpp"
#include "street.hpp"

#include <cassert>

net::Network::Network(int column, int row) : _lenght{column}, _height{row} {
  assert(_lenght > 0);
  assert(_height > 2 &&
         _height % 2 != 0); // ho 3 sorgenti quindi mi servono almeno 3 righe
  _net.resize(_height * _lenght);
}

Street &net::Network::operator()(int row, int column) {
  return _net[row * _lenght * _height + column];
}

int net::Network::getSize() const { return _net.size(); }