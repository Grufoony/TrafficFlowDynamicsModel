#include "Vehicle.hpp"

Vehicle::Vehicle(int type) {
  _index = type;
  _position = _vehicleType[_index]->getOrigin();
}

int Vehicle::_nVehicleType = 0;
VehicleType *Vehicle::_vehicleType[_maxNumVehicleType];
void Vehicle::addVehicleType(int origin, int destination) {
  _vehicleType[_nVehicleType] = new VehicleType(origin, destination);
  ++_nVehicleType;
}
void Vehicle::setTimePenalty(int time) { _timePenalty = time; }
int Vehicle::getTimePenalty() { return _timePenalty; }