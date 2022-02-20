#include "vehicle.hpp"
#include <cassert>
#include <stdexcept>

Vehicle::Vehicle(int t) : _type{t} {
  assert(_type > -1 && _type < 9);
  switch (_type) {
  case 0:
    _source = 1;
    _destination = 1;
    break;
  case 1:
    _source = 1;
    _destination = 0;
    break;
  case 2:
    _source = 1;
    _destination = -1;
    break;
  case 3:
    _source = 0;
    _destination = 1;
    break;
  case 4:
    _source = 0;
    _destination = 0;
    break;
  case 5:
    _source = 0;
    _destination = -1;
    break;
  case 6:
    _source = -1;
    _destination = 1;
    break;
  case 7:
    _source = -1;
    _destination = 0;
    break;
  case 8:
    _source = -1;
    _destination = -1;
    break;
  default:
    throw std::runtime_error("Errore nell'assegnazione del tipo di veicolo\n");
  }
}