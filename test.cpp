#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./src/Street.hpp"
#include "./src/Vehicle.hpp"
#include "./src/VehicleType.hpp"
#include "./utils/SparseMatrix.hpp"
#include "./utils/doctest.h"

TEST_CASE("Boolean Matrix") {
  SUBCASE("Default constructor") {
    // Create a boolean matrix
    SparseMatrix<bool> m;
    // Check the dimensions
    CHECK(m.getRowDim() == 0);
    CHECK(m.getColDim() == 0);
    CHECK(m.max_size() == 0);
  }
  SUBCASE("Constructor with dimensions") {
    // Create a boolean matrix
    SparseMatrix<bool> m(3, 3);
    // Check the dimensions
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 3);
    CHECK(m.max_size() == 9);
    // Check exceptions
    CHECK_THROWS(m(-1, 0));
    CHECK_THROWS(m(0, -1));
  }
  SUBCASE("Constructor with dimension") {
    // Create a boolean matrix
    SparseMatrix<bool> m(3);
    // Check the dimensions
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 1);
    CHECK(m.max_size() == 3);
    // Check exceptions
    CHECK_THROWS(m(-1));
  }
  SUBCASE("Insertions") {
    // Create a boolean matrix
    SparseMatrix<bool> m(3, 3);
    // Insert a true value
    m.insert(0, 0, true);
    // Check all values
    CHECK(m(0, 0));
    for (int i = 1; i < 9; ++i) {
      CHECK(!m(i / 3, i % 3));
    }
    // Check that an exception is thrown if the element is out of range
    CHECK_THROWS(m(-1, -1));
    CHECK_THROWS(m(3, 1));
  }
  SUBCASE("Deletions") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.erase(0, 0);
    // Check if the value has been deleted
    CHECK(!m(0, 0));
  }
  SUBCASE("Clear") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.clear();
    // Check if the matrix is empty
    CHECK(m.size() == 0);
    CHECK_THROWS(m(0, 0));
  }
  SUBCASE("Contains") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    CHECK(m.contains(0, 0));
    CHECK(!m.contains(0, 1));
    CHECK_THROWS(m.contains(-1, -1));
  }
  SUBCASE("Get row") {
    SparseMatrix<bool> m(3, 3);
    // Create a row
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    auto row = m.getRow(0);
    CHECK(row.size() == 3);
    CHECK(row(0));
    CHECK(row(1));
    CHECK(row(2));
    CHECK_THROWS(m.getRow(-1));
  }
  SUBCASE("Get column") {
    SparseMatrix<bool> m(3, 3);
    // Create a column
    m.insert(0, 0, true);
    m.insert(1, 0, true);
    m.insert(2, 0, true);
    auto col = m.getCol(0);
    CHECK(col.size() == 3);
    CHECK(col(0));
    CHECK(col(1));
    CHECK(col(2));
    CHECK_THROWS(m.getCol(-1));
  }
  SUBCASE("Get row dimension") {
    SparseMatrix<bool> m(3, 3);
    CHECK(m.getRowDim() == 3);
  }
  SUBCASE("Get column dimension") {
    SparseMatrix<bool> m(3, 3);
    CHECK(m.getColDim() == 3);
  }
  SUBCASE("Get size") {
    SparseMatrix<bool> m(3, 3);
    CHECK(m.max_size() == 9);
  }
  SUBCASE("Get number of non-zero elements") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    CHECK(m.size() == 3);
  }
  SUBCASE("Erase row") {
    SparseMatrix<bool> d(3, 3);
    // Create a row
    d.insert(0, 0, true);
    d.insert(1, 2, true);
    d.insert(2, 1, true);
    // Clone the matrix
    auto m = d;
    // Erase the row (for each row)
    m.eraseRow(1);
    // Check the values
    CHECK(m(0, 0));
    CHECK(m(1, 1));
    m = d;
    m.eraseRow(0);
    CHECK(m(0, 2));
    CHECK(m(1, 1));
    m = d;
    m.eraseRow(2);
    CHECK(m(0, 0));
    CHECK(m(1, 2));
    m.eraseRow(0);
    CHECK(m(0, 2));
  }
  SUBCASE("Erase column") {
    SparseMatrix<bool> d(3, 3);
    d.insert(0, 0, true);
    d.insert(1, 2, true);
    d.insert(2, 1, true);
    auto m = d;
    m.eraseColumn(1);
    CHECK(m(0, 0));
    CHECK(m(1, 1));
    m = d;
    m.eraseColumn(0);
    CHECK(m(1, 1));
    CHECK(m(2, 0));
    m = d;
    m.eraseColumn(2);
    CHECK(m(0, 0));
    CHECK(m(2, 1));
  }
  SUBCASE("Degree vector") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 0, true);
    m.insert(2, 1, true);
    m.insert(2, 2, true);
    auto v = m.getDegreeVector();
    CHECK(v(0) == 2);
    CHECK(v(1) == 1);
    CHECK(v(2) == 3);
  }
  SUBCASE("Random Elements") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    for (int i = 0; i < 9; ++i) {
      auto e = m.getRndElement();
      CHECK(m.contains(e.first));
    }
  }
  SUBCASE("Random Row") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    for (int i = 0; i < 3; ++i) {
      auto e = m.getRndRowElement(i);
      CHECK(m.contains(e.first));
    }
  }
  SUBCASE("Random Column") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    for (int i = 0; i < 3; ++i) {
      auto e = m.getRndColElement(i);
      CHECK(m.contains(e.first));
    }
  }
  SUBCASE("Normalized Rows") {
    SparseMatrix<bool> m(3, 3);
    // Create a row
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    m.insert(1, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    // Get the normalized rows
    auto v = m.getNormRows();
    CHECK(v(0, 0) - 1. / 3 < std::numeric_limits<double>::epsilon());
    CHECK(v(1, 1) - 1. / 2 < std::numeric_limits<double>::epsilon());
    CHECK(v(2, 1) - 1 < std::numeric_limits<double>::epsilon());
    CHECK(v(0, 0) + v(0, 1) + v(0, 2) - 1 <
          std::numeric_limits<double>::epsilon());
    CHECK(v(1, 0) + v(1, 1) + v(1, 2) - 1 <
          std::numeric_limits<double>::epsilon());
    CHECK(v(2, 0) + v(2, 1) + v(2, 2) - 1 <
          std::numeric_limits<double>::epsilon());
  }
  SUBCASE("Normalized Columns") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    m.insert(1, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    auto v = m.getNormCols();
    CHECK(v(0, 0) - 1 < std::numeric_limits<double>::epsilon());
    CHECK(v(1, 1) - 1. / 3 < std::numeric_limits<double>::epsilon());
    CHECK(v(2, 1) - 1. / 2 < std::numeric_limits<double>::epsilon());
    CHECK(v(0, 0) + v(1, 0) + v(2, 0) - 1 <
          std::numeric_limits<double>::epsilon());
    CHECK(v(0, 1) + v(1, 1) + v(2, 1) - 1 <
          std::numeric_limits<double>::epsilon());
    CHECK(v(0, 2) + v(1, 2) + v(2, 2) - 1 <
          std::numeric_limits<double>::epsilon());
  }
  SUBCASE("Symmetrization") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    // Symmetrize the matrix
    m.symmetrize();
    // Check the values
    for (int i = 0; i < 9; ++i) {
      if (i != 4 && i != 8 && i != 2 && i != 6) {
        CHECK(m.contains(i));
      } else {
        CHECK(!m.contains(i));
      }
    }
  }
}

// VehicleType

TEST_CASE("VehicleType") {
  SUBCASE("Constructor and getters") {
    // Create a VehicleType with source 0 and destination 1
    VehicleType v(0, 1);
    CHECK(v.getSource() == 0);
    CHECK(v.getDestination() == 1);
  }
  SUBCASE("Transition Matrix") {
    // Create a VehicleType with source 0 and destination 1
    VehicleType v(0, 1);
    // Create a transition matrix
    SparseMatrix<double> m(3, 3);
    m.insert(0, 0, 0.5);
    m.insert(0, 1, 0.5);
    m.insert(1, 1, 0.5);
    m.insert(1, 2, 0.5);
    m.insert(2, 2, 1.);
    // Set the transition matrix
    v.setTransMatrix(m);
    // Get the transition matrix
    auto m2 = v.getTransMatrix();
    // Check the transition matrix
    CHECK(m2(0, 0) == 0.5);
    CHECK(m2(0, 1) == 0.5);
    CHECK(m2(1, 1) == 0.5);
    CHECK(m2(1, 2) == 0.5);
    CHECK(m2(2, 2) == 1.);
    // Check exceptions
    auto v2 = VehicleType(0, 1);
    // Check that an exception is thrown if the transition matrix is empty
    CHECK_THROWS(v2.getTransMatrix());
  }
}

// Vehicle

TEST_CASE("Vehicle") {
  SUBCASE("AddVehicleType") {
    // Add a vehicle type
    Vehicle::addVehicleType(0, 1);
    // Check the number of vehicle types
    CHECK(Vehicle::getNVehicleType() == 1);
    // Check exceptions
    CHECK_THROWS(Vehicle::addVehicleType(0, 1));
  }
  SUBCASE("Constructor and getters") {
    // Create a vehicle
    Vehicle v(0);
    // Check the default values
    CHECK(v.getType() == 0);
    CHECK(v.getPosition() == 0);
    CHECK(v.getPreviousPosition() == -1);
    CHECK(v.getStreet() == -1);
    CHECK(v.getTimePenalty() == 0);
    CHECK(v.getVelocity() == 0.);
    CHECK(v.getTimeTraveled() == 0);
  }
  SUBCASE("Constructor exceptions") { CHECK_THROWS(Vehicle(12)); }
  SUBCASE("VehicleType") {
    auto v = Vehicle(0);
    // Check the vehicle type
    CHECK(v.getType() == 0);
  }
  SUBCASE("VehicleType return type") {
    auto vt = VehicleType(0, 1);
    // Check the vehicle type
    CHECK(vt.getSource() == Vehicle::getVehicleType(0)->getSource());
    CHECK(vt.getDestination() == Vehicle::getVehicleType(0)->getDestination());
  }
}

// Street

TEST_CASE("Street") {
  SUBCASE("Constructor and getters") {
    // Create a street
    Street s(0, 1, 10., 0);
    // Check the values
    CHECK(s.getOrigin() == 0);
    CHECK(s.getDestination() == 1);
    CHECK(s.getLength() == 10.);
    CHECK(s.getIndex() == 0);
    CHECK(!s.isFull());
    CHECK(s.getNVehicles() == 0);
    CHECK(s.getVMax() == 13.9); // default value
    CHECK(s.getInputVelocity() == s.getVMax());
    CHECK(s.getDensity() == 0.);
    CHECK(s.getVehicleDensity() == 0.);
    CHECK(s.getNLanes() == 1);
  }
  SUBCASE("NLanes") {
    // Create a street
    Street s(0, 1, 10., 0);
    // Set the number of lanes
    s.setNLanes(2);
    // Check the number of lanes
    CHECK(s.getNLanes() == 2);
    // Check exceptions
    CHECK_THROWS(s.setNLanes(-1));
  }
  SUBCASE("VMax") {
    // Create a street
    Street s(0, 1, 10., 0);
    // Set the maximum velocity
    s.setVMax(10.);
    // Check the maximum velocity
    CHECK(s.getVMax() == 10.);
    // Check exceptions
    CHECK_THROWS(s.setVMax(-1.));
  }
  SUBCASE("Add and remove vehicles") {
    // VehicleType is added in the Vehicle test
    // Create a street
    Street s(0, 1, 10., 0);
    // Add a vehicle
    s.addVehicle(std::make_shared<Vehicle>(0));
    // Check the number of vehicles
    CHECK(s.getNVehicles() == 1);
    // Remove a vehicle
    s.remVehicle();
    // Check the number of vehicles
    CHECK(s.getNVehicles() == 0);
    CHECK_THROWS(s.remVehicle());
  }
}