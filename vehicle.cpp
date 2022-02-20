#include "vehicle.hpp"
#include <cassert>

Vehicle::Vehicle(int t) : type{t} { assert(type > -1 && type < 9); }