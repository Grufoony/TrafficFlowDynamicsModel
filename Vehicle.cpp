#include "Vehicle.hpp"

Vehicle::Vehicle(int type) { _index = type; }

int Vehicle::_nVehicleType = 0;
void Vehicle::addVehicleType(int origin, int destination) {
  _vehicleType[_nVehicleType] = new VehicleType(origin, destination);
  ++_nVehicleType;
}
void Vehicle::setTimePenalty(int time) { _timePenalty = time; }
int Vehicle::getTimePenalty() { return _timePenalty; }