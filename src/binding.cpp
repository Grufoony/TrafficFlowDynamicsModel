#include "Graph.cpp"
#include "Graph.hpp"
#include "Street.cpp"
#include "Street.hpp"
#include "Vehicle.cpp"
#include "Vehicle.hpp"
#include "VehicleType.cpp"
#include "VehicleType.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Binding code for Graph, Street, Vehicle, and VehicleType classes

PYBIND11_MODULE(TrafficModel, m) {
  m.doc() = "A library for generating traffic flow dynamics data.";

  // Graph class
  pybind11::class_<Graph>(m, "Graph")
      .def(pybind11::init<std::string>())
      .def("addVehicle", static_cast<void (Graph::*)(int)>(&Graph::addVehicle))
      .def("addRndmVehicles", &Graph::addRndmVehicles)
      .def("addVehiclesUniformly", &Graph::addVehiclesUniformly)
      //   .def("loadVehicles", &Graph::loadVehicles)
      .def("setTemperature", &Graph::setTemperature)
      .def("updateTransMatrix", &Graph::updateTransMatrix)
      .def("evolve", static_cast<void (Graph::*)()>(&Graph::evolve))
      .def("evolve", static_cast<void (Graph::*)(bool)>(&Graph::evolve))
      .def("printMatrix", &Graph::printMatrix)
      .def("print", &Graph::print)
      .def("printStreets", &Graph::printStreets)
      .def("fprint", &Graph::fprint)
      .def("fprintVisual", &Graph::fprintVisual)
      .def("fprintHistogram", &Graph::fprintHistogram)
      .def("fprintTimeDistribution", &Graph::fprintTimeDistribution)
      .def("fprintActualState", &Graph::fprintActualState);
  //   .def("save", &Graph::save)
  //   .def("test", &Graph::test);

  // Street class
  pybind11::class_<Street>(m, "Street")
      .def(pybind11::init<int, int, double, int>())
      .def("getOrigin", &Street::getOrigin)
      .def("getDestination", &Street::getDestination)
      .def("getIndex", &Street::getIndex)
      .def("getLength", &Street::getLength)
      .def("isFull", &Street::isFull)
      .def("setNLanes", &Street::setNLanes)
      .def("getNVehicles", &Street::getNVehicles)
      .def("setVMax", &Street::setVMax)
      .def("getInputVelocity", &Street::getInputVelocity)
      .def("getVMax", &Street::getVMax)
      .def("getDensity", &Street::getDensity)
      .def("getVehicleDensity", &Street::getVehicleDensity)
      .def("addVehicle", &Street::addVehicle)
      .def("remVehicle", &Street::remVehicle);

  // VehicleType class
  pybind11::class_<VehicleType>(m, "VehicleType")
      .def(pybind11::init<int, int>())
      .def("getSource", &VehicleType::getSource)
      .def("getDestination", &VehicleType::getDestination)
      .def("setTransMatrix", &VehicleType::setTransMatrix);

  // Vehicle class
  pybind11::class_<Vehicle>(m, "Vehicle")
      .def(pybind11::init<int>())
      .def_static("addVehicleType", static_cast<void (*)(uint16_t, uint16_t)>(
                                        &Vehicle::addVehicleType))
      .def_static("addVehicleType",
                  static_cast<void (*)(std::string)>(&Vehicle::addVehicleType))
      .def_static("getVehicleType", &Vehicle::getVehicleType)
      .def("getType", &Vehicle::getType)
      .def("getNVehicleType", &Vehicle::getNVehicleType)
      .def("getType", &Vehicle::getType)
      .def("getPosition", &Vehicle::getPosition)
      .def("getPreviousPosition", &Vehicle::getPreviousPosition)
      .def("getStreet", &Vehicle::getStreet)
      .def("getDestination", &Vehicle::getDestination)
      .def("setTimePenalty", &Vehicle::setTimePenalty)
      .def("setVelocity", &Vehicle::setVelocity)
      .def("getVelocity", &Vehicle::getVelocity)
      .def("incrementTimeTraveled", &Vehicle::incrementTimeTraveled)
      .def("getTimeTraveled", &Vehicle::getTimeTraveled)
      .def("resetTimeTraveled", &Vehicle::resetTimeTraveled);
}
