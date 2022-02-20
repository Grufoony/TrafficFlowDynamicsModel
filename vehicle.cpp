#include "vehicle.hpp"
#include <cassert>

Vehicle::Vehicle(int t) : _type{t} { assert(_type > -1 && _type < 9); }