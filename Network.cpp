#include "Network.hpp"

#include <cassert>
#include <cmath>
#include <fstream>

// debugging
#include <iostream>

// net::Network::Network(int row, int column) : _lenght{row}, _height{column} {
//   assert(_lenght > 0);
//   assert(_height > 2 &&
//          _height % 2 != 0); // ho 3 sorgenti quindi mi servono almeno 3 righe
//   _net.resize(_lenght * _height);
// }

// operatore per trattare il valarray come matrice
// Street &net::Network::operator()(int row, int column) noexcept {
//   auto index = row * _lenght * _height + column;
//   // if (index > static_cast<int>(_net.size())) {
//   //   return _net[index - _net.size()];
//   // } else if (index < 0) {
//   //   return _net[index + _net.size()];
//   // } else {
//   //   return _net[index];
//   // }
//   return _net[index];
// }

void net::Network::importNodeMatrix(const char *fName) {
  std::ifstream data;
  _nNodes = 0;
  bool x;
  data.open(fName);
  while (data >> x) {
    _nodeMatrix.push_back(x);
    ++_nNodes;
  }

  int endline = std::sqrt(_nNodes);
  std::cout << endline << '\n';
  for (int i = 0; i < endline; ++i) {
    for (int j = 0; j < endline; ++j) {
      std::cout << _nodeMatrix.at(j);
    }
    std::cout << '\n';
  }

  // something

  data.close();
}

// int net::Network::getSize() const { return _net.size(); }