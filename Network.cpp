#include "network.hpp"
#include "street.hpp"

#include <cassert>
#include <cmath>
#include <stdexcept>

#include <iostream>

void net::Network::updateProbability(Vehicle v) {
  double deltaX = _lenght - v.getX();
  double deltaY = _height / 2 - v.getDestination() - v.getY();
  double tot = std::abs(deltaX) + std::abs(deltaY);
  deltaX = deltaX / tot;
  deltaY = deltaY / tot;

  std::vector<double> prob;
  if (deltaY < 0) {
    prob.push_back(std::abs(deltaY));
    prob.push_back(std::abs(deltaX));
    prob.push_back(0.);
    prob.push_back(0.);
  } else {
    prob.push_back(0.);
    prob.push_back(std::abs(deltaX));
    prob.push_back(std::abs(deltaY));
    prob.push_back(0.);
  }
  std::cout << "sos" << prob[1] << '\n';

  v.setProb(prob);
}

net::Network::Network(int row, int column) : _lenght{row}, _height{column} {
  assert(_lenght > 0);
  assert(_height > 2 &&
         _height % 2 != 0); // ho 3 sorgenti quindi mi servono almeno 3 righe
  _net.resize(_lenght * _height);
}

// operatore per trattare il valarray come matrice
Street &net::Network::operator()(int row, int column) noexcept {
  auto index = row * _lenght * _height + column;
  // if (index > static_cast<int>(_net.size())) {
  //   return _net[index - _net.size()];
  // } else if (index < 0) {
  //   return _net[index + _net.size()];
  // } else {
  //   return _net[index];
  // }
  return _net[index];
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
    throw std::runtime_error("Errore in Network::addVehicle\n");
  }
  _vehicles.push_back(Vehicle(type, y));
}

void net::Network::test() {
  updateProbability(_vehicles[0]);
  std::cout << _vehicles[0].getX() << '\n';
  std::cout << _vehicles[0].getDestination() << '\n';
  auto prob = _vehicles[0].getProbArray();
  for (int i = 0; i < 4; ++i) {
    std::cout << _vehicles[0].getProbArray().at(i) << '\n';
  }
}