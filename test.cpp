#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./src/Graph.hpp"
#include "./src/Street.hpp"
#include "./src/Vehicle.hpp"
#include "./src/VehicleType.hpp"
#include "./utils/SparseMatrix.hpp"
#include "./utils/doctest.h"
#include "./utils/utils.hpp"
#include <cstdio>

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
  SUBCASE("print") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    std::stringstream ss;
    ss << m;
    // check edge list
    CHECK(ss.str() == "3\t3\n5\t1\n1\t1\n0\t1\n");
  }
  SUBCASE("fprint") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    std::string fileName = "./data/test/temp.txt";
    m.fprint(fileName);
    // compare output with reference
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test0_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
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
    Vehicle::addVehicleType(3, 7);
    CHECK(Vehicle::getNVehicleType() == 2);
    CHECK_THROWS(Vehicle::addVehicleType(0, 1));
  }
  SUBCASE("AddVehicleType from file") {
    // Add vehicle types from file
    Vehicle::addVehicleType("./data/vehicletype_old.dat");
    // Check correct import
    CHECK(Vehicle::getVehicleType(2)->getSource() == 0);
    CHECK(Vehicle::getVehicleType(2)->getDestination() == 8);
    CHECK(Vehicle::getVehicleType(3)->getSource() == 2);
    CHECK(Vehicle::getVehicleType(3)->getDestination() == 6);
    // Check exceptions
    CHECK_THROWS(Vehicle::addVehicleType("./not/a/path"));
    CHECK_THROWS(Vehicle::addVehicleType("./data/vehicletype_old.dat"));
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
    CHECK(v.getDestination() == 1);
  }
  SUBCASE("Constructor") {
    // Second type
    Vehicle v2(1);
    CHECK(v2.getDestination() == 7);
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
  SUBCASE("Position") {
    auto v = Vehicle(1);
    CHECK(v.getPosition() == 3); // default value
    // Set the position
    v.setPosition(10);
    // Check the position
    CHECK(v.getPosition() == 10);
    // Check exceptions
    CHECK_THROWS(v.setPosition(-1));
  }
  SUBCASE("Previous position") {
    auto v = Vehicle(1);
    CHECK(v.getPreviousPosition() == -1); // default value
    // Set the previous position
    v.setPosition(10);
    // Check the previous position
    CHECK(v.getPreviousPosition() == 3);
  }
  SUBCASE("Street") {
    auto v = Vehicle(1);
    CHECK(v.getStreet() == -1); // default value
    // Set the street
    v.setStreet(10);
    // Check the street
    CHECK(v.getStreet() == 10);
  }
  SUBCASE("Velocity") {
    auto v = Vehicle(1);
    CHECK(v.getVelocity() == 0.); // default value
    // Set the velocity
    v.setVelocity(10.);
    // Check the velocity
    CHECK(v.getVelocity() == 10.);
    // Check exceptions
    CHECK_THROWS(v.setVelocity(-1.));
  }
  SUBCASE("Time of travel") {
    auto v = Vehicle(1);
    CHECK(v.getTimeTraveled() == 0); // default value
    // Increment the time of travel
    v.incrementTimeTraveled();
    // Check the time of travel
    CHECK(v.getTimeTraveled() == 1);
    // Reset the time of travel
    v.resetTimeTraveled();
    // Check the time of travel
    CHECK(v.getTimeTraveled() == 0);
  }
  SUBCASE("Cycle for time of travel") {
    auto v = Vehicle(3);
    for (int i = 0; i < 10; ++i) {
      v.incrementTimeTraveled();
    }
    CHECK(v.getTimeTraveled() == 10);
  }
  SUBCASE("Time penalty") {
    auto v = Vehicle(1);
    CHECK(v.getTimePenalty() == 0); // default value
    // Set the time penalty
    v.setTimePenalty(10);
    // Check the time penalty
    CHECK(v.getTimePenalty() == 10);
    // Check exceptions
    CHECK_THROWS(v.setTimePenalty(-1));
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

// Utils

TEST_CASE("utils") {
  SUBCASE("minDistance") {
    // Create a vector
    std::vector<int> v{1, 2, 3};
    std::vector<bool> sptSet{false, false, false};
    // Check the minimum distance should be 0 cause of 1 is the minimum
    CHECK(minDistance(v, sptSet, 3) == 0.);
  }
  SUBCASE("minDistance 2") {
    // Create a vector
    std::vector<int> v{1, 2, 3};
    std::vector<bool> sptSet{true, true, false};
    // Check the minimum distance should be 2 due to the true values
    CHECK(minDistance(v, sptSet, 3) == 2);
  }
  SUBCASE("normalizeVec") {
    // Create a vector
    std::vector<double> v{1., 2., 3.};
    // Normalize the vector
    normalizeVec(v);
    // Check the vector
    CHECK(v[0] == 1. / 6);
    CHECK(v[1] == 2. / 6);
    CHECK(v[2] == 3. / 6);
  }
}

// Graph

TEST_CASE("Graph") {
  SUBCASE("Constructor exception") { CHECK_THROWS(Graph("./not/a/path")); }
  SUBCASE("Temperature") {
    Graph g("./data/matrix.dat");
    CHECK_THROWS(g.setTemperature(-1.));
    g.setTemperature(300.);
    CHECK(g.getTemperature() == 300.);
  }
  SUBCASE("TimeScale") {
    Graph g("./data/matrix.dat");
    CHECK_THROWS(g.setTimeScale(-10));
    g.setTimeScale(10);
    CHECK(g.getTimeScale() == 10);
  }
  SUBCASE("addVehicle exceptions") {
    Graph g("./data/matrix.dat");
    // VehicleType index < 0
    CHECK_THROWS(g.addVehicle(-1));
    // VehicleType index out of range
    CHECK_THROWS(g.addVehicle(100));
  }
  SUBCASE("addRndVehicle exceptions") {
    Graph g("./data/matrix.dat");
    // VehicleType index < 0
    CHECK_THROWS(g.addRndmVehicles(-1));
  }
  // to check the constructor we'll check all the print functions
  SUBCASE("fprint") {
    Graph g("./data/matrix_old.dat");
    g.fprint(false);
    // compare the two files
    std::ifstream f1("./network_info.txt");
    std::ifstream f2("./data/test/test1_false_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    g.fprint(true);
    // compare the two files (verbose version)
    std::ifstream f3("./network_info.txt");
    std::ifstream f4("./data/test/test1_true_ref.txt");
    std::string s3((std::istreambuf_iterator<char>(f3)),
                   std::istreambuf_iterator<char>());
    std::string s4((std::istreambuf_iterator<char>(f4)),
                   std::istreambuf_iterator<char>());
    CHECK(s3 == s4);
  }
  SUBCASE("fprintStreets without vehicles") {
    Graph g("./data/matrix_old.dat");
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test2_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    // Streets are print only if they have vehicles so this should be empty
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  SUBCASE("addRndmVehicle") {
    std::mt19937 gen(std::random_device{}());
    gen.seed(69);
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addRndmVehicles(1);
    g.updateTransMatrix();
    g.evolve(false); // to put vehicles on the streets
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    Graph f("./data/matrix.dat");
    std::uniform_int_distribution<> dis(0, Vehicle::getNVehicleType() - 1);
    f.addVehicle(dis(gen));
    f.updateTransMatrix();
    f.evolve(false); // to put vehicles on the streets
    std::string fileName2 = "./data/test/temp2.txt";
    f.fprintStreets(fileName2);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2(fileName2);
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
    std::remove(fileName2.c_str());
  }
  SUBCASE("fprintStreets with many vehicles") {
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addRndmVehicles(10);
    g.updateTransMatrix();
    g.evolve(false); // to put vehicles on the streets
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test4_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  SUBCASE("updateTransMatrix") {
    Graph g("./data/matrix.dat");
    // add a vehicle of type 0
    g.addVehicle(1);
    g.updateTransMatrix();
    Vehicle v(1);
    SparseMatrix<double> mat = v.getVehicleType(1)->getTransMatrix();
    // check the matrix which should bring from 3 to 7 in a line
    CHECK(mat(3, 4) == 1.);
    CHECK(mat(4, 5) == 1.);
    CHECK(mat(5, 6) == 1.);
    CHECK(mat(6, 7) == 1.);
    // check also that the vehicle cannot go back
    CHECK(mat(7, 6) == 0.);
    CHECK(mat(6, 5) == 0.);
    CHECK(mat(5, 4) == 0.);
    CHECK(mat(4, 3) == 0.);
  }
  SUBCASE("updateTransMatrix with noise") {
    Graph g("./data/matrix.dat");
    // add a vehicle of type 0
    g.addVehicle(0);
    // set a probability to fail
    g.setTemperature(300);
    g.updateTransMatrix();
    Vehicle v(0);
    SparseMatrix<double> mat = v.getVehicleType(0)->getTransMatrix();
    // this vehicle has only two possible moves, one correct and one wrong
    CHECK(std::abs(mat(0, 1) - 0.924418) < 1e-6);
    CHECK(std::abs(mat(0, 12) - 0.0755823) < 1e-6);
  }
  SUBCASE("evolve") {
    Graph g("./data/matrix.dat");
    // add a vehicle of type 0
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve(false);
    std::string fileName = "./data/test/temp.txt";
    // should print 1 vehicle on street 0
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test3_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
    // street is 500 meters long so it should take 36 steps to arrive
    for (int i = 0; i < 36; i++) {
      g.evolve(false);
    }
    // check that the vehicle has arrived to destination
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f3(fileName);
    std::ifstream f4("./data/test/test2_ref.txt"); // empty file
    std::string s3((std::istreambuf_iterator<char>(f3)),
                   std::istreambuf_iterator<char>());
    std::string s4((std::istreambuf_iterator<char>(f4)),
                   std::istreambuf_iterator<char>());
    CHECK(s3 == s4);
    std::remove(fileName.c_str());
  }
  SUBCASE("evolve with reinsertion") {
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addVehicle(0);
    g.updateTransMatrix();
    // default time scale is 100
    for (int i = 0; i < 100; i++) {
      g.evolve();
    }
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test5_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  SUBCASE("evolve with many vehicles") {
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addRndmVehicles(24);
    g.updateTransMatrix();
    for (int i = 0; i < 69; i++) {
      g.evolve();
    }
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test6_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  // Now we can check the statistics of the vehicles
  SUBCASE("fprintVisual") {
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string fileName = "./data/test/";
    g.fprintVisual(fileName);
    // compare the two files
    std::ifstream f1(fileName + "1.dat");
    std::ifstream f2("./data/test/test7_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((fileName + "1.dat").c_str());
  }
  SUBCASE("fprintHistogram exceptions") {
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string fileName = "./data/test/";
    CHECK_THROWS(g.fprintHistogram(fileName, "density", -5, "root"));
    CHECK_THROWS(g.fprintHistogram(fileName, "notvalid", 5, "root"));
    CHECK_THROWS(g.fprintHistogram(fileName, "traveltime", 5, "notvalid"));
  }
}