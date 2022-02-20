#include "network.hpp"
#include "street.hpp"

#include <cassert>
#include <stdexcept>

net::Network::Network(int row, int column) : _lenght{row}, _height{column} {
  assert(_lenght > 0);
  assert(_height > 2 &&
         _height % 2 != 0); // ho 3 sorgenti quindi mi servono almeno 3 righe
  _net.resize(_lenght * _height);
}

Street &net::Network::operator()(int row, int column) {
  return _net[row * _lenght * _height + column];
}

int net::Network::getSize() const { return _net.size(); }

void net::Network::addVehicle(int type) {
  int y;
  switch (type) {
  case 0 ... 2:
    y = (_height - 1) / 2;
    break;
  case 3 ... 5:
    y = (_height - 1) / 2 + 1;
    break;
  case 6 ... 8:
    y = (_height - 1) / 2;
    break;
  default:
    throw std::runtime_error("Errore in addVehicle\n");
  }
  _vehicles.push_back(Vehicle(type, y));
}