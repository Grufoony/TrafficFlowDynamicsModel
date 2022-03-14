#ifndef STREET_HPP
#define STREET_HPP

class Street {
private:
  double _lenght;     // lenght of the street
  int _capacity = 10; // maximum capacity of vehicles
  int _nVehicles = 0; // number of vehicles on the street
  int _nodes[2];      // nodes connections of the street

public:
  Street(int, int, double);

  int *getNodes();
  int getLenght() const noexcept;
  int getCapacity() const noexcept;
  int getNVehicles();
  double getVelocity();
  void addVehicle();
  void remVehicle();
};

#endif