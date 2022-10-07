#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Graph.cpp"
#include "Graph.hpp"

namespace py = pybind11;

PYBIND11_MODULE(Graph,m) {
	m.doc() = "A library for generating traffic flow dynamics data.";

	pybind11::class_<Graph>(m,"Graph")
		.def(pybind11::init<const char *>())
		.def(pybind11::init<const char *, const char *>())
		.def("addVehicle", static_cast<void (Graph::*)(int)>(&Graph::addVehicle))
		.def("addVehicle", static_cast<void (Graph::*)(int,int)>(&Graph::addVehicle))
		.def("addRndmVehicles", &Graph::addRndmVehicles)
		.def("addVehiclesUniformly", &Graph::addVehiclesUniformly)
		.def("loadVehicles", &Graph::loadVehicles)
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
		.def("fprintDistribution", &Graph::fprintDistribution)
		.def("fprintActualState", &Graph::fprintActualState)
		.def("save", &Graph::save)
		.def("test", &Graph::test);
		//.def("_minDistance", [](Graph const& g, int a, int b)
		//	{ return g._minDistance(a,b) } )
		//.def("_nextStep", &Graph::_nextStep)
		//.def("_evolve", &Graph::_evolve)
		//.def("_findStreet", &Graph::_findStreet)
		//.def("_getStreetMeanVelocity", &Graph::_getStreetMeanVelocity)
}
