#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./src/Graph.hpp"
#include "./src/Street.hpp"
#include "./src/Vehicle.hpp"
#include "./src/VehicleType.hpp"
#include "./utils/SparseMatrix.hpp"
#include "./utils/doctest.h"
#include "./utils/utils.hpp"
#include <cstdio>

// test file used to test the code

/****************************************************************************************
 *                                                                                      *
 *  TESTS FOR THE SPARSE MATRIX CLASS *
 *                                                                                      *
 ****************************************************************************************/

TEST_CASE("Boolean Matrix") {
  SUBCASE("Default constructor") {
    /*This test tests if the default constructor works correctly
    The default constructor should create a matrix with 0 rows and 0 columns
    and a max size of 0
    GIVEN: the default constructor is called
    WHEN: the matrix is created
    THEN: the matrix should have 0 rows and 0 columns and a max size of 0
    */
    SparseMatrix<bool> m;
    // Check the dimensions
    CHECK(m.getRowDim() == 0);
    CHECK(m.getColDim() == 0);
    CHECK(m.max_size() == 0);
  }
  SUBCASE("Constructor with dimensions") {
    /*This test tests if the constructor with dimensions works correctly
    The constructor should create a matrix with the specified dimensions and a
    max size equal to the product of the dimensions
    GIVEN: the constructor is called with dimensions 3 and 4
    WHEN: the matrix is created
    THEN: the matrix should have 3 rows and 4 columns and a max size of 12
    */
    SparseMatrix<bool> m(3, 4);
    // Check the dimensions
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 4);
    CHECK(m.max_size() == 12);
    // Check out of range exceptions
    CHECK_THROWS(m(-1, 0));
    CHECK_THROWS(m(0, -1));
  }
  SUBCASE("Constructor with dimension") {
    /*This test tests if the constructor with dimension works correctly
    The constructor should create a row vector with the specified dimension
    GIVEN: the constructor is called with dimension 3
    WHEN: the matrix is created
    THEN: the matrix should have 3 rows and 1 column and a max size of 3
    */
    SparseMatrix<bool> m(3);
    // Check the dimensions
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 1);
    CHECK(m.max_size() == 3);
    // Check out of range exceptions
    CHECK_THROWS(m(-1));
  }
  SUBCASE("Encode") {
    /*This test tests if the encode function works correctly
    The encode function should transform a matrix into a list
    GIVEN: the encode function is called
    WHEN: the function is called on a file
    THEN: the function should transform the matrix into a list
    */
    std::string fileName = "./data/test/convert.dat";
    SparseMatrix<double>::encode(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/encoded_ref.dat");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
  }
  SUBCASE("Decode") {
    /*This test tests if the decode function works correctly
    The decode function should transform a list into a matrix
    GIVEN: the decode function is called
    WHEN: the function is called on a file
    THEN: the function should transform the list into a matrix
    */
    std::string fileName = "./data/test/convert.dat";
    SparseMatrix<double>::decode(fileName);
    // compare the two files
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/matrix_ref.dat");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
  }
  SUBCASE("Insertion exceptions") {
    /*This test tests if the insert function throws exceptions correctly
    The insert function should throw an exception if the inserted element is out
    of range GIVEN: the insert function is called WHEN: the function is called
    on a matrix THEN: the function should throw an exception if the inserted
    element is out of range
    */
    SparseMatrix<bool> m(3, 3);
    // Check that an exception is thrown if the element is out of range
    CHECK_THROWS(m.insert(-1, true));
    CHECK_THROWS(m.insert(3, 2, true));
  }
  SUBCASE("insert_or_assign exceptions") {
    /*This test tests if the insert_or_assign function throws exceptions
    correctly The insert_or_assign function should throw an exception if the
    inserted element is out of range GIVEN: the insert_or_assign function is
    called WHEN: the function is called on a matrix THEN: the function should
    throw an exception if the inserted element is out of range
    */
    SparseMatrix<bool> m(3, 3);
    // Check that an exception is thrown if the element is out of range
    CHECK_THROWS(m.insert_or_assign(-1, -2, true));
    CHECK_THROWS(m.insert_or_assign(3, 2, true));
  }
  SUBCASE("Insertions") {
    /*This test tests if the insert function works correctly
    The insert function should insert a value in the matrix
    GIVEN: the insert function is called
    WHEN: the function is called on a matrix
    THEN: the function should insert a value in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    // Insert a true value
    m.insert(0, 0, true);
    m.insert(5, true);
    // Check all values
    CHECK(m(0, 0));
    CHECK(m(1, 2));
    for (int i = 1; i < 9; ++i) {
      if (i != 5) {
        CHECK(!m(i / 3, i % 3));
      }
    }
  }
  SUBCASE("insert_or_assign") {
    /*This test tests if the insert_or_assign function works correctly
    The insert_or_assign function should insert a value in the matrix
    or assign a new value to an existing element
    GIVEN: the insert_or_assign function is called
    WHEN: the function is called on a matrix
    THEN: the function should insert a value in the matrix
    */
    SparseMatrix<int> m(4, 3);
    // Insert a true value
    m.insert_or_assign(1, 2, 10);
    CHECK(m(1, 2) == 10);
    m.insert_or_assign(1, 2, 20);
    CHECK(m(1, 2) == 20);
  }
  SUBCASE("at - exceptions") {
    /*This test tests if the at function throws exceptions correctly
    The at function should throw an exception if the element is out of range
    GIVEN: the at function is called
    WHEN: the function is called on a matrix
    THEN: the function should throw an exception if the element is out of range
    */
    SparseMatrix<bool> m(2, 3);
    // Check that an exception is thrown if the element is out of range
    CHECK_THROWS(m.at(-1, -2));
    CHECK_THROWS(m.at(-2));
    CHECK_THROWS(m.at(3, 2));
    CHECK_THROWS(m.at(6));
  }
  SUBCASE("at") {
    /*This test tests if the at function works correctly
    The at function should return the value at the specified position
    GIVEN: the at function is called
    WHEN: the function is called on a matrix
    THEN: the function should return the value at the specified position
    */
    SparseMatrix<bool> m(3, 3);
    // Insert a true value
    m.insert(1, 0, true);
    // Check the value
    CHECK(m.at(1, 0));
    CHECK(m.at(3));
  }
  SUBCASE("Deletions") {
    /*This test tests if the erase function works correctly
    The erase function should delete a value in the matrix
    GIVEN: the erase function is called
    WHEN: the function is called on a matrix
    THEN: the function should delete a value in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.erase(0, 0);
    // Check if the value has been deleted
    CHECK(!m(0, 0));
  }
  SUBCASE("Clear") {
    /*This test tests if the clear function works correctly
    The clear function should delete all the elements and dimensions in the
    matrix GIVEN: the clear function is called WHEN: the function is called on a
    matrix THEN: the function should delete all the elements and dimensions in
    the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.clear();
    // Check if the matrix is empty
    CHECK(m.size() == 0);
    CHECK_THROWS(m(0, 0));
  }
  SUBCASE("Contains exceptions") {
    /*This test tests if the contains function throws exceptions correctly
    The contains function should throw an exception if the element is out of
    range GIVEN: the contains function is called WHEN: the function is called on
    a matrix THEN: the function should throw an exception if the element is out
    of range
    */
    SparseMatrix<bool> m(3, 3);
    CHECK_THROWS(m.contains(-2, -1));
    CHECK_THROWS(m.contains(-1));
  }
  SUBCASE("Contains") {
    /*This test tests if the contains function works correctly
    The contains function should return true if the matrix contains the element
    GIVEN: the element is in the matrix
    WHEN: the function is called
    THEN: the function should return true
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(2, 1, true);
    CHECK(m.contains(0, 0));
    CHECK(m.contains(7));
  }
  SUBCASE("Get row") {
    /*This test tests if the getRow function works correctly
    The getRow function should return a vector containing the elements of the
    row GIVEN: the getRow function is called WHEN: the function is called on a
    matrix THEN: the function should return a vector (SparseMatrix) containing
    the elements of the row
    */
    SparseMatrix<bool> m(3, 3);
    // Create a row
    m.insert(0, 0, true);
    m.insert(0, 2, true);
    auto row = m.getRow(0);
    // verify attributes and values
    CHECK(row.size() == 2);
    CHECK(row(0));
    CHECK(!row(1));
    CHECK(row(2));
    // Check out of range exceptions
    CHECK_THROWS(m.getRow(-1));
    CHECK_THROWS(m.getRow(3));
  }
  SUBCASE("Get column") {
    /*This test tests if the getCol function works correctly
    The getCol function should return a vector containing the elements of the
    column GIVEN: the getCol function is called WHEN: the function is called on
    a matrix THEN: the function should return a vector (SparseMatrix) containing
    the elements of the column
    */
    SparseMatrix<bool> m(3, 3);
    // Create a column
    m.insert(0, 0, true);
    m.insert(2, 0, true);
    auto col = m.getCol(0);
    // verify attributes and values
    CHECK(col.size() == 2);
    CHECK(col(0));
    CHECK(!col(1));
    CHECK(col(2));
    // Check out of range exceptions
    CHECK_THROWS(m.getCol(-1));
    CHECK_THROWS(m.getCol(3));
  }
  SUBCASE("Get row dimension") {
    /*This test tests if the getRowDim function works correctly
    The getRowDim function should return the number of rows in the matrix
    GIVEN: the getRowDim function is called
    WHEN: the function is called on a matrix
    THEN: the function should return the number of rows in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    CHECK(m.getRowDim() == 3);
  }
  SUBCASE("Get column dimension") {
    /*This test tests if the getColDim function works correctly
    The getColDim function should return the number of columns in the matrix
    GIVEN: the getColDim function is called
    WHEN: the function is called on a matrix
    THEN: the function should return the number of columns in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    CHECK(m.getColDim() == 3);
  }
  SUBCASE("Get max_size") {
    /*This test tests if the max_size function works correctly
    The max_size function should return the maximum number of elements that can
    be stored in the matrix GIVEN: the max_size function is called WHEN: the
    function is called on a matrix THEN: the function should return the maximum
    number of elements that can be stored in the matrix
    */
    SparseMatrix<bool> m(3, 5);
    CHECK(m.max_size() == 15);
  }
  SUBCASE("Get size") {
    /*This test tests if the size function works correctly
    The size function should return the number of non-zero elements in the
    matrix GIVEN: the size function is called WHEN: the function is called on a
    matrix THEN: the function should return the number of elements in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    CHECK(m.size() == 3);
    m.insert(1, 1, true);
    CHECK(m.size() == 4);
  }
  SUBCASE("Erase row") {
    /*This test tests if the eraseRow function works correctly
    The eraseRow function should delete all the elements in the row (and also
    the row itself, reducing the number of rows by 1) GIVEN: the eraseRow
    function is called WHEN: the function is called on a matrix THEN: the
    function should delete all the elements in the row
    */
    SparseMatrix<bool> d(3, 3);
    // Create a row
    d.insert(0, 0, true);
    d.insert(1, 2, true);
    d.insert(2, 1, true);
    // Clone the matrix
    auto m = d;
    // Erase the row (for each row) and check if all the other elements are
    // rearranged correctly
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
    /*This test tests if the eraseColumn function works correctly
    The eraseColumn function should delete all the elements in the column (and
    also the column itself, reducing the number of columns by 1) GIVEN: the
    eraseColumn function is called WHEN: the function is called on a matrix
    THEN: the function should delete all the elements in the column
    */
    SparseMatrix<bool> d(3, 3);
    d.insert(0, 0, true);
    d.insert(1, 2, true);
    d.insert(2, 1, true);
    auto m = d;
    // Erase the row (for each row) and check if all the other elements are
    // rearranged correctly
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
    /*This test tests if the getDegreeVector function works correctly
    The getDegreeVector function should return a vector containing the degree of
    each row GIVEN: the getDegreeVector function is called WHEN: the function is
    called on a matrix THEN: the function should return a vector containing the
    degree of each row
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 0, true);
    m.insert(2, 1, true);
    m.insert(2, 2, true);
    auto v = m.getDegreeVector();
    // check if the sum on all rows is done correctly
    CHECK(v(0) == 2);
    CHECK(v(1) == 1);
    CHECK(v(2) == 3);
  }
  SUBCASE("Random Elements") {
    /*This test tests if the getRndElement function works correctly
    The getRndElement function should return a random element in the matrix
    GIVEN: the getRndElement function is called
    WHEN: the function is called on a matrix
    THEN: the function should return a random non-zero element in the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    std::mt19937 gen(std::random_device{}());
    gen.seed(69);
    m.setSeed(69);
    auto dist = std::uniform_int_distribution<int>(0, m.size() - 1);
    for (int i = 0; i < 9; ++i) {
      auto it = m.begin();
      auto e = m.getRndElement();
      std::advance(it, dist(gen));
      CHECK(e.first == (*it).first);
    }
  }
  SUBCASE("Random Row Element") {
    /*This test tests if the getRndRowElement function works correctly
    The getRndRowElement function should return a random element in the row
    GIVEN: the getRndRowElement function is called
    WHEN: the function is called on a matrix
    THEN: the function should return a random non-zero element in the row
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    auto m2 = m;
    m.setSeed(69);
    m2.setSeed(69);
    // check random elements on every row
    for (int i = 0; i < 3; ++i) {
      auto e = m.getRndRowElement(i);
      // comparing using getRndElement (which is tested above)
      CHECK(e.first == m2.getRow(i).getRndElement().first +
                           i * 3); // convert row index to matrix index
    }
  }
  SUBCASE("Random Column Element") {
    /*This test tests if the getRndColElement function works correctly
    The getRndColElement function should return a random element in the column
    GIVEN: the getRndColElement function is called
    WHEN: the function is called on a matrix
    THEN: the function should return a random non-zero element in the column
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    auto m2 = m;
    m.setSeed(69);
    m2.setSeed(69);
    // check random elements on every column
    for (int i = 0; i < 3; ++i) {
      auto e = m.getRndColElement(i);
      // comparing using getRndElement (which is tested above)
      CHECK(e.first == m2.getCol(i).getRndElement().first * 3 +
                           i); // convert column index to matrix index
    }
  }
  SUBCASE("Normalized Rows") {
    /*This test tests if the getNormRows function works correctly
    The getNormRows function should return a matrix containing the normalized
    rows GIVEN: the getNormRows function is called WHEN: the function is called
    on a matrix THEN: the function should return a matrix containing the
    normalized rows
    */
    SparseMatrix<bool> m(3, 3);
    // Create a row
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    m.insert(1, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    // Get the normalized rows and check their values, comparig floats with a
    // numeric limit
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
    /*This test tests if the getNormCols function works correctly
    The getNormCols function should return a matrix containing the normalized
    columns GIVEN: the getNormCols function is called WHEN: the function is
    called on a matrix THEN: the function should return a matrix containing the
    normalized columns
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    m.insert(1, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    // Get the normalized columns and check their values, comparig floats with a
    // numeric limit
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
    /*This test tests if the symmetrize function works correctly
    The symmetrize function should symmetrize the matrix
    GIVEN: the symmetrize function is called
    WHEN: the function is called on a matrix
    THEN: the function should symmetrize the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    // Symmetrize the matrix
    m.symmetrize();
    CHECK(m(0, 0));
    CHECK(m(0, 1));
    CHECK(m(1, 0));
    CHECK(m(1, 2));
    CHECK(m(2, 1));
    CHECK(m.size() == 5);
  }
  SUBCASE("+ operator") {
    /*This test tests if the + operator works correctly
    The + operator should sum two matrices
    GIVEN: the + operator is called
    WHEN: the function is called on two matrices
    THEN: the function should sum the two matrices
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    SparseMatrix<bool> m2(3, 3);
    m2.insert(0, 0, true);
    m2.insert(1, 0, true);
    m2.insert(2, 1, true);
    auto m3 = m + m2;
    CHECK(m3(0, 0));
    CHECK(m3(1, 0));
    CHECK(m3(2, 1));
    CHECK(m3(0, 1));
    CHECK(m3(1, 2));
    CHECK(m3.size() == 5);
  }
  SUBCASE("+= operator") {
    /*This test tests if the += operator works correctly
    The += operator should sum two matrices
    GIVEN: the += operator is called
    WHEN: the function is called on two matrices
    THEN: the function should sum the two matrices
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    SparseMatrix<bool> m2(3, 3);
    m2.insert(0, 0, true);
    m2.insert(1, 0, true);
    m2.insert(2, 1, true);
    m += m2;
    CHECK(m(0, 0));
    CHECK(m(1, 0));
    CHECK(m(2, 1));
    CHECK(m(0, 1));
    CHECK(m(1, 2));
    CHECK(m.size() == 5);
  }
  SUBCASE("- operator") {
    /*This test tests if the - operator works correctly
    The - operator should subtract two matrices
    GIVEN: the - operator is called
    WHEN: the function is called on two matrices
    THEN: the function should subtract the two matrices
    */
    SparseMatrix<int> m(3, 3);
    m.insert(0, 0, 1);
    m.insert(0, 1, 2);
    m.insert(1, 2, 3);
    SparseMatrix<int> m2(3, 3);
    m2.insert(0, 0, 1);
    m2.insert(1, 0, 2);
    m2.insert(2, 1, 3);
    auto m3 = m - m2;
    CHECK(m3(0, 0) == 0);
    CHECK(m3(1, 0) == -2);
    CHECK(m3(2, 1) == -3);
    CHECK(m3(0, 1) == 2);
    CHECK(m3(1, 2) == 3);
    CHECK(m3.size() == 5);
  }
  SUBCASE("-= operator") {
    /*This test tests if the -= operator works correctly
    The -= operator should subtract two matrices
    GIVEN: the -= operator is called
    WHEN: the function is called on two matrices
    THEN: the function should subtract the two matrices
    */
    SparseMatrix<int> m(3, 3);
    m.insert(0, 0, 1);
    m.insert(0, 1, 2);
    m.insert(1, 2, 3);
    SparseMatrix<int> m2(3, 3);
    m2.insert(0, 0, 1);
    m2.insert(1, 0, 2);
    m2.insert(2, 1, 3);
    m -= m2;
    CHECK(m(0, 0) == 0);
    CHECK(m(1, 0) == -2);
    CHECK(m(2, 1) == -3);
    CHECK(m(0, 1) == 2);
    CHECK(m(1, 2) == 3);
    CHECK(m.size() == 5);
  }
  SUBCASE("print") {
    /*This test tests if the print function (operator) works correctly
    The print function should print the matrix in a file
    GIVEN: the print function is called
    WHEN: the function is called on a matrix
    THEN: the function should print the matrix in a file
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    std::stringstream ss;
    // here we can use the << operator instead of the print function because it
    // does the same thing
    ss << m;
    // check edge list
    CHECK(ss.str() == "3\t3\n5\t1\n1\t1\n0\t1\n");
  }
  SUBCASE("fprint") {
    /*This test tests if the fprint function works correctly
    The fprint function should print the matrix in a file
    GIVEN: the fprint function is called
    WHEN: the function is called on a matrix
    THEN: the function should print the matrix in a file
    */
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
  SUBCASE("getStrengthVector") {
    /*This test tests if the getStrengthVector function works correctly
    The getStrengthVector function should return the strength vector of the
    matrix
    GIVEN: the getStrengthVector function is called
    WHEN: the function is called on a street with 1 vehicle and length 10
    THEN: the function should return a vector with 0.1
    */
    SparseMatrix<double> m(3, 3);
    m.insert(0, 0, 0.3);
    m.insert(0, 1, 0.3);
    m.insert(0, 2, 0.1);
    m.insert(1, 1, 0.5);
    m.insert(2, 0, 0.1);
    m.insert(2, 2, 1.);
    auto v = m.getStrengthVector();
    CHECK(v(0) == 0.7);
    CHECK(v(1) == 0.5);
    CHECK(v(2) == 1.1);
    CHECK(v.size() == 3);
  }
  SUBCASE("getLaplacian - exception") {
    /*This test tests if the getLaplacian function throws an exception
    The getLaplacian function should throw an exception if the street is empty
    GIVEN: the getLaplacian function is called
    WHEN: the function is called on a non-square matrix
    THEN: the function should throw an exception
    */
    SparseMatrix<double> m(4, 3);
    CHECK_THROWS(m.getLaplacian());
  }
  SUBCASE("getLaplacian") {
    /*This test tests if the getLaplacian function works correctly
    The getLaplacian function should return the Laplacian matrix of the matrix
    GIVEN: the getLaplacian function is called
    WHEN: the function is called on a matrix
    THEN: the function should return the Laplacian matrix of the matrix
    */
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(1, 2, true);
    auto m2 = m.getLaplacian();
    CHECK(m2(0, 0) == 2);
    CHECK(m2(0, 1) == -1);
    CHECK(m2(1, 1) == 1);
    CHECK(m2(1, 2) == -1);
    CHECK(m2(2, 2) == 0);
    CHECK(m2.size() == 5);
  }
}

/****************************************************************************************
 *                                                                                      *
 *  TESTS FOR THE VEHICLETYPE CLASS *
 *                                                                                      *
 ****************************************************************************************/

TEST_CASE("VehicleType") {
  SUBCASE("Constructor and getters") {
    /*This test tests if the constructor and the getters work correctly
    The constructor should create a VehicleType with the specified source and
    destination
    GIVEN: the constructor is called with source 0 and destination 1
    WHEN: the VehicleType is created
    THEN: the VehicleType should have source 0 and destination 1
    */
    VehicleType v(0, 1);
    CHECK(v.getSource() == 0);
    CHECK(v.getDestination() == 1);
  }
  SUBCASE("getTransMatrix exception") {
    /*This test tests if the getTransMatrix function throws an exception
    The getTransMatrix function should throw an exception if the transition
    matrix is empty
    GIVEN: the getTransMatrix function is called
    WHEN: the function is called on a VehicleType with an empty transition
    matrix
    THEN: the function should throw an exception
    */
    auto v2 = VehicleType(0, 1);
    // Check that an exception is thrown if the transition matrix is empty
    CHECK_THROWS(v2.getTransMatrix());
  }
  SUBCASE("Transition Matrix") {
    /*This test tests if the transition matrix works correctly
    The transition matrix should be a square matrix with the same number of rows
    and columns as the number of nodes in the graph
    GIVEN: the transition matrix
    WHEN: the transition matrix is set
    THEN: the transition matrix should be a square matrix with the same number
    of rows and columns as the number of nodes in the graph normalized on the
    rows
    */
    VehicleType v(0, 1);
    // Create a transition matrix
    SparseMatrix<double> m(3, 3);
    m.insert(0, 0, 0.3);
    m.insert(0, 1, 0.3);
    m.insert(0, 2, 0.4);
    m.insert(1, 1, 0.5);
    m.insert(1, 2, 0.5);
    m.insert(2, 2, 1.);
    // Set the transition matrix
    v.setTransMatrix(m);
    // Get the transition matrix
    auto m2 = v.getTransMatrix();
    // Check the transition matrix
    // Here we can compare floats with ==, I don't really know why but it works
    CHECK(m2(0, 0) == 0.3);
    CHECK(m2(0, 1) == 0.3);
    CHECK(m2(0, 2) == 0.4);
    CHECK(m2(1, 1) == 0.5);
    CHECK(m2(1, 2) == 0.5);
    CHECK(m2(2, 2) == 1.);
  }
}

// Vehicle

TEST_CASE("Vehicle") {
  SUBCASE("AddVehicleType") {
    /*This test tests if the addVehicleType function works correctly
    The addVehicleType function should add a new VehicleType to the list of
    VehicleTypes
    GIVEN: the addVehicleType function is called
    WHEN: the function is called with source 0 and destination 1
    THEN: the function should add a new VehicleType with source 0 and
    destination 1 to the list of VehicleTypes
    */
    Vehicle::addVehicleType(0, 1);
    // Check the number of vehicle types
    CHECK(Vehicle::getNVehicleType() == 1);
    // Check exceptions
    Vehicle::addVehicleType(3, 7);
    CHECK(Vehicle::getNVehicleType() == 2);
  }
  SUBCASE("addVehicleType exception") {
    /*This test tests if the addVehicleType function throws an exception
    The addVehicleType function should throw an exception if the VehicleType is
    already in the list of VehicleTypes
    GIVEN: the addVehicleType function is called
    WHEN: the function is called with source 0 and destination 1
    THEN: the function should throw an exception
    */
    CHECK_THROWS(Vehicle::addVehicleType(0, 1));
  }
  SUBCASE("AddVehicleType from file") {
    /*This test tests if the addVehicleType function works correctly
    The addVehicleType function should add new VehicleTypes to the list of
    VehicleType from a file
    GIVEN: the addVehicleType function is called
    WHEN: the function is called with a file containing the VehicleTypes
    THEN: the function should add new VehicleTypes to the list of VehicleTypes
    */
    Vehicle::addVehicleType("./data/vehicletype_old.dat");
    // Check correct import
    CHECK(Vehicle::getVehicleType(2)->getSource() == 0);
    CHECK(Vehicle::getVehicleType(2)->getDestination() == 8);
    CHECK(Vehicle::getVehicleType(3)->getSource() == 2);
    CHECK(Vehicle::getVehicleType(3)->getDestination() == 6);
  }
  SUBCASE("AddVehicleType from file exceptions") {
    /*This test tests if the addVehicleType function throws an exception
    The addVehicleType function should throw an exception if the file is not
    found or if the file contains already existing VehicleTypes
    GIVEN: the addVehicleType function is called
    WHEN: the function is called with a file containing the VehicleTypes
    THEN: the function should throw an exception
    */
    CHECK_THROWS(Vehicle::addVehicleType("./not/a/path"));
    CHECK_THROWS(Vehicle::addVehicleType("./data/vehicletype_old.dat"));
  }
  SUBCASE("Constructor and getters") {
    /*This test tests if the constructor and the getters work correctly
    The constructor should create a Vehicle with the specified type
    GIVEN: the constructor is called with type 0
    WHEN: the Vehicle is created
    THEN: the Vehicle should have type 0, i.e. source 0 and destination 1 and
    other values should be set to default
    */
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
  SUBCASE("Constructor exceptions") {
    /*This test tests if the constructor throws an exception
    The constructor should throw an exception if the type is out of the range
    GIVEN: the constructor is called with type 12 or -1 (negative case)
    WHEN: the Vehicle is created
    THEN: the function should throw an exception
    */
    CHECK_THROWS(Vehicle(12));
    CHECK_THROWS(Vehicle(-1));
  }
  SUBCASE("Constructor") {
    /*This test tests if the constructor works correctly
    The constructor should create a Vehicle with the specified destination
    GIVEN: the constructor is called with a VehicleType with destination 7 and
    source 3 WHEN: the Vehicle is created THEN: the Vehicle should have
    destination 7
    */
    Vehicle v2(1);
    CHECK(v2.getDestination() == 7);
  }
  SUBCASE("getType") {
    /*This test tests if the getType function works correctly
    The getType function should return the index of the type of the Vehicle
    GIVEN: the getType function is called
    WHEN: the function is called on a Vehicle with type 0
    THEN: the function should return 0
    */
    auto v = Vehicle(0);
    CHECK(v.getType() == 0);
  }
  SUBCASE("VehicleType return type") {
    /*This test tests if the VehicleType function works correctly
    The VehicleType function should return the VehicleType of the Vehicle
    GIVEN: the VehicleType function is called
    WHEN: the function is called on a Vehicle with type 0
    THEN: the function should return the VehicleType with source 0 and
    destination 1
    */
    auto vt = VehicleType(0, 1);
    CHECK(vt.getSource() == Vehicle::getVehicleType(0)->getSource());
    CHECK(vt.getDestination() == Vehicle::getVehicleType(0)->getDestination());
  }
  SUBCASE("setPosition exception") {
    /*This test tests if the setPosition function throws an exception
    The setPosition function should throw an exception if the position is out of
    the range GIVEN: the setPosition function is called with position -1
    WHEN: the function is called on a Vehicle
    THEN: the function should throw an exception
    */
    auto v = Vehicle(1);
    CHECK_THROWS(v.setPosition(-1));
  }
  SUBCASE("Position") {
    /*This test tests if the setPosition and getPosition functions work
    correctly The setPosition function should set the position of the Vehicle
    (node index) The getPosition function should return the position of the
    Vehicle GIVEN: the setPosition function is called with position 10 WHEN: the
    function is called on a Vehicle THEN: the function should set the position
    of the Vehicle to 10
    */
    auto v = Vehicle(1);
    CHECK(v.getPosition() == 3); // default VehicleType value
    // Set the position
    v.setPosition(10);
    // Check the position
    CHECK(v.getPosition() == 10);
  }
  SUBCASE("getPreviousPosition") {
    /*This test tests if the getPreviousPosition function works correctly
    The getPreviousPosition function should return the previous position of the
    Vehicle (node index)
    GIVEN: the getPreviousPosition function is called
    WHEN: the function is called on a Vehicle with previous position 3
    THEN: the function should return 3
    */
    auto v = Vehicle(1);
    // Set the previous position
    v.setPosition(10);
    // Check the previous position
    CHECK(v.getPreviousPosition() == 3);
  }
  SUBCASE("Street") {
    /*This test tests if the setStreet and getStreet functions work correctly
    The setStreet function should set the street of the Vehicle (edge index)
    The getStreet function should return the street of the Vehicle
    GIVEN: the setStreet function is called with street 10
    WHEN: the function is called on a Vehicle
    THEN: the function should set the street of the Vehicle to 10
    */
    auto v = Vehicle(1);
    // Set the street
    v.setStreet(10);
    // Check the street
    CHECK(v.getStreet() == 10);
  }
  SUBCASE("setVelocity exception") {
    /*This test tests if the setVelocity function throws an exception
    The setVelocity function should throw an exception if the velocity is
    negative GIVEN: the setVelocity function is called with velocity -1 WHEN:
    the function is called on a Vehicle THEN: the function should throw an
    exception
    */
    auto v = Vehicle(1);
    CHECK_THROWS(v.setVelocity(-1.));
  }
  SUBCASE("Velocity") {
    /*This test tests if the setVelocity and getVelocity functions work
    correctly The setVelocity function should set the velocity of the Vehicle
    The getVelocity function should return the velocity of the Vehicle
    GIVEN: the setVelocity function is called with velocity 10
    WHEN: the function is called on a Vehicle
    THEN: the function should set the velocity of the Vehicle to 10
    */
    auto v = Vehicle(1);
    CHECK(v.getVelocity() == 0.); // default value
    // Set the velocity
    v.setVelocity(10.);
    // Check the velocity
    CHECK(v.getVelocity() == 10.);
  }
  SUBCASE("Time of travel") {
    /*This test tests if the incrementTimeTraveled, resetTimeTraveled and
    getTimeTraveled functions work correctly
    The incrementTimeTraveled function
    should increment the time of travel of the Vehicle
    The resetTimeTraveled function should reset the time of travel of the
    Vehicle
    The getTimeTraveled function should return the time of travel of the
    Vehicle
    GIVEN: the incrementTimeTraveled function is called
    WHEN: the function is called on a Vehicle
    THEN: the function should increment the time of travel of the Vehicle, then
    the resetTimeTraveled function is called and the function should reset the
    time of travel of the Vehicle
    */
    auto v = Vehicle(1);
    CHECK(v.getTimeTraveled() == 0); // default value
    // Increment the time of travel
    v.incrementTimeTraveled();
    // Check the time of travel
    CHECK(v.getTimeTraveled() == 1);
    // Reset the time of travel
    v.resetTimeTraveled();
    CHECK(v.getTimeTraveled() == 0);
  }
  SUBCASE("Cycle for time of travel") {
    /*This test tests if the incrementTimeTraveled function works correctly
    The incrementTimeTraveled function should increment the time of travel of
    the Vehicle many times
    GIVEN: the incrementTimeTraveled function is called
    WHEN: the function is called on a Vehicle with cycle time 10
    THEN: the function should increment the time of travel of the Vehicle many
    times
    */
    auto v = Vehicle(3);
    for (int i = 0; i < 10; ++i) {
      v.incrementTimeTraveled();
    }
    CHECK(v.getTimeTraveled() == 10);
  }
  SUBCASE("setTimePenalty exception") {
    /*This test tests if the setTimePenalty function throws an exception
    The setTimePenalty function should throw an exception if the time penalty is
    negative GIVEN: the setTimePenalty function is called with time penalty -1
    WHEN: the function is called on a Vehicle
    THEN: the function should throw an exception
    */
    auto v = Vehicle(1);
    CHECK_THROWS(v.setTimePenalty(-1));
  }
  SUBCASE("Time penalty") {
    /*This test tests if the setTimePenalty and getTimePenalty functions work
    correctly The setTimePenalty function should set the time penalty of the
    Vehicle The getTimePenalty function should return the time penalty of the
    Vehicle GIVEN: the setTimePenalty function is called with time penalty 6
    WHEN: the function is called on a Vehicle
    THEN: the function should set the time penalty of the Vehicle to 6
    */
    auto v = Vehicle(1);
    CHECK(v.getTimePenalty() == 0); // default value
    // Set the time penalty
    v.setTimePenalty(6);
    // Check the time penalty
    CHECK(v.getTimePenalty() == 6);
  }
}

/****************************************************************************************
 *                                                                                      *
 *  TESTS FOR THE STREET CLASS *
 *                                                                                      *
 ****************************************************************************************/

TEST_CASE("Street") {
  SUBCASE("Constructor exceptions") {
    /*This test tests if the constructor throws an exception
    The constructor should throw an exception if the length is negative or if
    the index is negative GIVEN: the constructor is called with length -10 or
    index -1 WHEN: the Street is created THEN: the function should throw an
    exception
    */
    CHECK_THROWS(Street(0, 1, -10., 0));
    CHECK_THROWS(Street(0, 1, 10., -1));
  }
  SUBCASE("Constructor and getters") {
    /*This test tests if the constructor and the getters work correctly
    The constructor should create a street with the specified origin,
    destination, length and index GIVEN: the constructor is called with origin
    0, destination 1, length 10 and index 0 WHEN: the street is created THEN:
    the street should have origin 0, destination 1, length 10 and index 0 and
    other values should be set to default
    */
    Street s(0, 1, 10., 0);
    // Check the values
    CHECK(s.getOrigin() == 0);
    CHECK(s.getDestination() == 1);
    CHECK(s.getLength() == 10.);
    CHECK(s.getIndex() == 0);
    // default values
    CHECK(!s.isFull());
    CHECK(s.getNVehicles() == 0);
    CHECK(s.getVMax() == 13.9);
    CHECK(s.getInputVelocity() == s.getVMax());
    CHECK(s.getDensity() == 0.);
    CHECK(s.getVehicleDensity() == 0.);
    CHECK(s.getNLanes() == 1);
  }
  SUBCASE("setNLanes exception") {
    /*This test tests if the setNLanes function throws an exception
    The setNLanes function should throw an exception if the number of lanes is
    negative GIVEN: the setNLanes function is called with number of lanes -10
    WHEN: the function is called on a street
    THEN: the function should throw an exception
    */
    Street s(0, 1, 10., 0);
    CHECK_THROWS(s.setNLanes(-10));
  }
  SUBCASE("NLanes") {
    /*This test tests if the setNLanes and getNLanes functions work correctly
    The setNLanes function should set the number of lanes of the street
    The getNLanes function should return the number of lanes of the street
    GIVEN: the setNLanes function is called with number of lanes 2
    WHEN: the function is called on a street
    THEN: the function should set the number of lanes of the street to 2
    */
    Street s(0, 1, 10., 0);
    // Set the number of lanes
    s.setNLanes(2);
    // Check the number of lanes
    CHECK(s.getNLanes() == 2);
  }
  SUBCASE("setVMax exception") {
    /*This test tests if the setVMax function throws an exception
    The setVMax function should throw an exception if the maximum velocity is
    negative GIVEN: the setVMax function is called with maximum velocity -7
    WHEN: the function is called on a street
    THEN: the function should throw an exception
    */
    Street s(0, 1, 10., 0);
    CHECK_THROWS(s.setVMax(-7.));
  }
  SUBCASE("VMax") {
    /*This test tests if the setVMax and getVMax functions work correctly
    The setVMax function should set the maximum velocity of the street
    The getVMax function should return the maximum velocity of the street
    GIVEN: the setVMax function is called with maximum velocity 10
    WHEN: the function is called on a street
    THEN: the function should set the maximum velocity of the street to 10
    */
    Street s(0, 1, 10., 0);
    // Set the maximum velocity
    s.setVMax(10.);
    // Check the maximum velocity
    CHECK(s.getVMax() == 10.);
  }
  SUBCASE("remVehicle exception") {
    /*This test tests if the remVehicle function throws an exception
    The remVehicle function should throw an exception if the street is empty
    GIVEN: the remVehicle function is called
    WHEN: the function is called on a street
    THEN: the function should throw an exception
    */
    // Create a street
    Street s(0, 1, 10., 0);
    CHECK_THROWS(s.remVehicle());
  }
  SUBCASE("Add and remove vehicles") {
    /*This test tests if the addVehicle and remVehicle functions work correctly
    The addVehicle function should add a vehicle to the street
    The remVehicle function should remove a vehicle from the street
    GIVEN: the addVehicle function is called with a vehicle of type 0
    WHEN: the function is called on a street
    THEN: the function should add a vehicle of type 0 to the street
    */
    // VehicleType is added in the Vehicle test
    // Create a street
    Street s(0, 1, 10., 0);
    auto v = std::make_shared<Vehicle>(0);
    s.addVehicle(v);
    // Check modified attributes of vehicle
    CHECK(v->getPosition() == 1);
    CHECK(v->getVelocity() == s.getVMax());
    CHECK(v->getStreet() == 0);
    CHECK(v->getTimePenalty() == static_cast<int>(10. / v->getVelocity()));
    // Check the number of vehicles
    CHECK(s.getNVehicles() == 1);
    // Remove a vehicle
    s.remVehicle();
    // Check the number of vehicles
    CHECK(s.getNVehicles() == 0);
  }
  SUBCASE("getInputVelocity") {
    /*This test tests if the getInputVelocity function works correctly
    The getInputVelocity function should return the input velocity of the
    street, which depends on the vehicle density on it
    GIVEN: the getInputVelocity function is called
    WHEN: the function is called on a street
    THEN: the function should return the input velocity of the street
    */
    // Create a street
    Street s(0, 1, 100., 0);
    // Add a vehicle
    auto v = Vehicle(0);
    for (int i = 0; i < 10; ++i) {
      s.addVehicle(std::make_shared<Vehicle>(v));
    }
    // Check the input velocity
    CHECK(s.getInputVelocity() == 5.2125);
  }
  SUBCASE("getNVehicles") {
    /*This test tests if the getNVehicles function works correctly
    The getNVehicles function should return the number of vehicles on the
    street
    GIVEN: the getNVehicles function is called
    WHEN: the function is called on a street with 2 vehicles
    THEN: the function should return 2
    */
    // Create a street
    Street s(0, 1, 10., 0);
    // Add a vehicle
    auto v = Vehicle(0);
    s.addVehicle(std::make_shared<Vehicle>(v));
    s.addVehicle(std::make_shared<Vehicle>(v));
    // Check the number of vehicles
    CHECK(s.getNVehicles() == 2);
  }
  SUBCASE("getIndex") {
    /*This test tests if the getIndex function works correctly
    The getIndex function should return the index of the street
    GIVEN: the getIndex function is called
    WHEN: the function is called on a street with index 10
    THEN: the function should return 10
    */
    // Create a street
    Street s(0, 1, 10., 10);
    // Check the index
    CHECK(s.getIndex() == 10);
  }
  SUBCASE("getLength") {
    /*This test tests if the getLength function works correctly
    The getLength function should return the length of the street
    GIVEN: the getLength function is called
    WHEN: the function is called on a street with length 10
    THEN: the function should return 10
    */
    Street s(0, 1, 10., 0);
    CHECK(s.getLength() == 10.);
  }
  SUBCASE("Full") {
    /*This test tests if the isFull function works correctly
    The isFull function should return true if the street is full
    GIVEN: the isFull function is called
    WHEN: the function is called on a street with 1 vehicle and length 8
    THEN: the function should return true
    */
    // Create a street
    Street s(0, 1, 8., 0);
    // Add a vehicle
    auto v = Vehicle(0);
    s.addVehicle(std::make_shared<Vehicle>(v));
    // Check if the street is full
    CHECK(s.isFull());
  }
  SUBCASE("getDensity") {
    /*This test tests if the getDensity function works correctly
    The getDensity function should return the density of the street
    GIVEN: the getDensity function is called
    WHEN: the function is called on a street with 1 vehicle and length 16
    THEN: the function should return 0.5
    */
    // Create a street
    Street s(0, 1, 16., 0);
    // Add a vehicle
    auto v = Vehicle(0);
    s.addVehicle(std::make_shared<Vehicle>(v));
    // Check the density
    CHECK(s.getDensity() == 0.5);
  }
  SUBCASE("getVehicleDensity") {
    /*This test tests if the getVehicleDensity function works correctly
    The getVehicleDensity function should return the vehicle density of the
    street
    GIVEN: the getVehicleDensity function is called
    WHEN: the function is called on a street with 1 vehicle and length 10
    THEN: the function should return 0.1
    */
    Street s(0, 1, 10., 0);
    Vehicle v(0);
    s.addVehicle(std::make_shared<Vehicle>(v));
    CHECK(s.getVehicleDensity() == 0.1);
  }
}

/****************************************************************************************
 *                                                                                      *
 *  TESTS FOR THE UTILS FUNCTIONS *
 *                                                                                      *
 ****************************************************************************************/

TEST_CASE("utils") {
  SUBCASE("minDistance") {
    /*This test tests if the minDistance function works correctly
    The minDistance function should return the minimum distance from the source
    for nodes with sptSet false GIVEN: the minDistance function is called WHEN:
    the function is called on a vector with distances 1, 2, 3 and a vector with
    false, false, false THEN: the function should return 0 cause 1 is the
    minimum
    */
    std::vector<int> v{1, 2, 3}; // distances
    std::vector<bool> sptSet{false, false, false};
    CHECK(minDistance(v, sptSet, 3) == 0.);
  }
  SUBCASE("minDistance 2") {
    /*This test tests if the minDistance function works correctly
    The minDistance function should return the minimum distance from the source
    for nodes with sptSet false GIVEN: the minDistance function is called WHEN:
    the function is called on a vector with distances 1, 2, 3 and a vector with
    true, true, false THEN: the function should return 2 cause 3 is the minimum
    (only valid value)
    */
    std::vector<int> v{1, 2, 3}; // distances
    std::vector<bool> sptSet{true, true, false};
    CHECK(minDistance(v, sptSet, 3) == 2);
  }
  SUBCASE("normalizeVec") {
    /*This test tests if the normalizeVec function works correctly
    The normalizeVec function should normalize the vector
    GIVEN: the normalizeVec function is called
    WHEN: the function is called on a vector with values 1, 2, 3
    THEN: the function should normalize the vector to have the sum of the
    elements equal to 1
    */
    std::vector<double> v{1., 2., 3.};
    // Normalize the vector
    normalizeVec(v);
    // Check the vector
    CHECK(v[0] == 1. / 6);
    CHECK(v[1] == 2. / 6);
    CHECK(v[2] == 3. / 6);
  }
  SUBCASE("normalizeVec - zero sum") {
    /*This test tests if the normalizeVec function works correctly
    The normalizeVec function should normalize the vector
    GIVEN: the normalizeVec function is called
    WHEN: the function is called on a vector with values 0, 0, 0
    THEN: the function should return the same vector
    */
    std::vector<double> v{0., 0., 0.};
    normalizeVec(v);
    CHECK(v[0] == 0.);
    CHECK(v[1] == 0.);
    CHECK(v[2] == 0.);
  }
}

/****************************************************************************************
 *                                                                                      *
 *  TESTS FOR THE GRAPH CLASS *
 *                                                                                      *
 ****************************************************************************************/

TEST_CASE("Graph") {
  SUBCASE("Constructor exception") {
    /*This test tests if the constructor throws an exception
    The constructor should throw an exception if the file is not found
    GIVEN: the constructor is called with a file that doesn't exist
    WHEN: the constructor is called
    THEN: the function should throw an exception
    */
    CHECK_THROWS(Graph("./not/a/path"));
  }
  SUBCASE("setTemperature exception") {
    /*This test tests if the setTemperature function throws an exception
    The setTemperature function should throw an exception if the temperature is
    negative GIVEN: the setTemperature function is called with temperature -2
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    CHECK_THROWS(g.setTemperature(-2.));
  }
  SUBCASE("Temperature") {
    /*This test tests if the setTemperature and getTemperature functions work
    correctly The setTemperature function should set the temperature of the
    graph The getTemperature function should return the temperature of the graph
    GIVEN: the setTemperature function is called with temperature 300
    WHEN: the function is called on a graph
    THEN: the function should set the temperature of the graph to 300
    */
    Graph g("./data/matrix.dat");
    g.setTemperature(300.);
    CHECK(g.getTemperature() == 300.);
  }
  SUBCASE("setTimeScale exception") {
    /*This test tests if the setTimeScale function throws an exception
    The setTimeScale function should throw an exception if the time scale is
    negative GIVEN: the setTimeScale function is called with time scale -10
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    CHECK_THROWS(g.setTimeScale(-10));
  }
  SUBCASE("TimeScale") {
    /*This test tests if the setTimeScale and getTimeScale functions work
    correctly The setTimeScale function should set the time scale of the graph
    The getTimeScale function should return the time scale of the graph
    GIVEN: the setTimeScale function is called with time scale 42
    WHEN: the function is called on a graph
    THEN: the function should set the time scale of the graph to 42
    */
    Graph g("./data/matrix.dat");
    g.setTimeScale(42);
    CHECK(g.getTimeScale() == 42);
  }
  SUBCASE("addVehicle exceptions") {
    /*This test tests if the addVehicle function throws an exception
    The addVehicle function should throw an exception if the VehicleType is not
    in the list of VehicleTypes, i.e. if the index is out of range
    GIVEN: the addVehicle function is called with index 100 or -1 (negative
    case) WHEN: thefunction is called on a graph THEN: the function should throw
    an exception
    */
    Graph g("./data/matrix.dat");
    // VehicleType index < 0
    CHECK_THROWS(g.addVehicle(-1));
    // VehicleType index out of range
    CHECK_THROWS(g.addVehicle(100));
  }
  SUBCASE("addRndVehicle exceptions") {
    /*This test tests if the addRndmVehicles function throws an exception
    The addRndmVehicles function should throw an exception if the number of
    vehicles is negative
    GIVEN: the addRndmVehicles function is called with number of vehicles -1
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    // VehicleType index < 0
    CHECK_THROWS(g.addRndmVehicles(-1));
  }
  SUBCASE("addVehiclesUniformly exception") {
    /*This test tests if the addVehiclesUniformly function throws an exception
    The addVehiclesUniformly function should throw an exception if the number of
    vehicles is negative
    GIVEN: the addVehiclesUniformly function is called with number of vehicles
    -1
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    // VehicleType index < 0
    CHECK_THROWS(g.addVehiclesUniformly(-1));
  }
  // to check the constructor we'll check all the print functions
  SUBCASE("fprint - not verbose") {
    /*This test tests if the fprint function works correctly
    The fprint function should print the graph in a file
    GIVEN: the fprint function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
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
  }
  SUBCASE("fprint - verbose") {
    /*This test tests if the fprint function works correctly
    The fprint function should print the graph in a file
    GIVEN: the fprint function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix_old.dat");
    g.fprint(true);
    // compare the two files
    std::ifstream f1("./network_info.txt");
    std::ifstream f2("./data/test/test1_true_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
  }
  SUBCASE("fprintStreets without vehicles") {
    /*This test tests if the fprintStreets function works correctly
    The fprintStreets function should print the streets in a file
    GIVEN: the fprintStreets function is called
    WHEN: the function is called on a graph
    THEN: the function should print the streets' info in a file
    */
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
    /*This test tests if the addRndmVehicle function works correctly
    The addRndmVehicle function should add a vehicle of a random type to the
    graph
    GIVEN: the addRndmVehicle function is called (setting seed to 69)
    WHEN: the function is called on a graph
    THEN: the function should add a vehicle of a random type to the graph
    */
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
    // create by generating here
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
  SUBCASE("addVehiclesUniformly") {
    /*This test tests if the addVehiclesUniformly function works correctly
    The addVehiclesUniformly function should add vehicles of all types to the
    graph, uniformly distributed on the streets (not starting from origin!)
    GIVEN: the addVehiclesUniformly function is called (setting seed to 69)
    WHEN: the function is called on a graph
    THEN: the function should add vehicles of all types to the graph, uniformly
    distributed on the streets
    */
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addVehiclesUniformly(4);
    g.updateTransMatrix();
    g.evolve(false); // to put vehicles on the streets
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test2b_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  SUBCASE("fprintStreets with many vehicles") {
    /*This test tests if the fprintStreets function works correctly
    The fprintStreets function should print the streets in a file
    GIVEN: the fprintStreets function is called
    WHEN: the function is called on a graph
    THEN: the function should print the streets' info in a file
    */
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
    /*This test tests if the updateTransMatrix function works correctly
    The updateTransMatrix function should update the transition matrix of a
    vehicle type according to the graph
    GIVEN: the updateTransMatrix function is called
    WHEN: the function is called on a graph
    THEN: the function should update the transition matrix of a vehicle type
    */
    Graph g("./data/matrix.dat");
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
  SUBCASE("updateTransMatrix with non-uniform matrix") {
    /*This test tests if the updateTransMatrix function works correctly
    The updateTransMatrix function should update the transition matrix of a
    vehicle type according to the graph
    GIVEN: the updateTransMatrix function is called
    WHEN: the function is called on a graph
    THEN: the function should update the transition matrix of a vehicle type
    */
    Graph g("./data/test/transition_matrix.dat");
    g.addVehicle(0); // goes from 0 to 1
    g.updateTransMatrix();
    Vehicle v(0);
    SparseMatrix<double> mat = v.getVehicleType(0)->getTransMatrix();
    // check that the direct path is not the shortest
    CHECK(mat(0, 1) == 0.);
    // check the matrix which should bring from 0 to 1 passing from 12 and 13
    CHECK(mat(0, 12) == 1.);
    CHECK(mat(12, 13) == 1.);
    CHECK(mat(13, 1) == 1.);
    // check also that the vehicle cannot go back
    CHECK(mat(1, 13) == 0.);
    CHECK(mat(13, 12) == 0.);
    CHECK(mat(12, 0) == 0.);
  }
  SUBCASE("updateTransMatrix with noise") {
    /*This test tests if the updateTransMatrix function works correctly
    The updateTransMatrix function should update the transition matrix of a
    vehicle type according to the graph
    Adding noise means that the vehicle can go in the wrong direction with a
    certain probability
    GIVEN: the updateTransMatrix function is called
    WHEN: the function is called on a graph
    THEN: the function should update the transition matrix of a vehicle type
    */
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
    /*This test tests if the evolve function works correctly
    The evolve function should evolve the graph for one step
    GIVEN: the evolve function is called
    WHEN: the function is called on a graph
    THEN: the function should evolve the graph for one step
    */
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
  }
  SUBCASE("evolve until destination") {
    /*This test tests if the evolve function works correctly
    The evolve function should evolve the graph one step at a time
    GIVEN: the evolve function is called
    WHEN: the function is called on a graph, in a loop
    THEN: the function should evolve the graph until the vehicle reaches its
    destination
    */
    Graph g("./data/matrix.dat");
    // add a vehicle of type 0
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve(false);
    std::string fileName = "./data/test/temp.txt";
    // street is 500 meters long so it should take 36 steps to arrive
    for (int i = 0; i < 36; ++i) {
      g.evolve(false);
    }
    // check that the vehicle has arrived to destination
    g.fprintStreets(fileName);
    // compare the two files to verify that it has not been reinserted
    std::ifstream f1(fileName);
    std::ifstream f2("./data/test/test2_ref.txt"); // empty file
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove(fileName.c_str());
  }
  SUBCASE("evolve with reinsertion") {
    /*This test tests if the evolve function works correctly
    The evolve function should evolve the graph one step at a time.
    If the vehicle reaches its destination it should be reinserted in the graph
    GIVEN: the evolve function is called
    WHEN: the function is called on a graph, in a loop
    THEN: the function should evolve the graph until the vehicle reaches its
    destination, then it should reinsert the vehicle in its origin
    */
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addVehicle(0);
    g.updateTransMatrix();
    // default time scale is 100
    for (int i = 0; i < 100; ++i) {
      g.evolve();
    }
    std::string fileName = "./data/test/temp.txt";
    g.fprintStreets(fileName);
    // compare the two files to check the reinsertion
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
    /*This test tests if the evolve function works correctly
    The evolve function should evolve the graph one step at a time, with many
    vehicles
    GIVEN: the evolve function is called
    WHEN: the function is called on a graph, in a loop
    THEN: the function should evolve the graph until the vehicle reaches its
    destination, then it should reinsert the vehicles in their origins
    */
    Graph g("./data/matrix.dat");
    g.setSeed(69);
    g.addRndmVehicles(24);
    g.updateTransMatrix();
    for (int i = 0; i < 69; ++i) {
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
    /*This test tests if the fprintVisual function works correctly
    The fprintVisual function should print the graph street density in a file
    GIVEN: the fprintVisual function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintVisual(outFolder);
    // compare the two files
    std::ifstream f1(outFolder + "1.dat");
    std::ifstream f2("./data/test/test7_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "1.dat").c_str());
  }
  SUBCASE("fprintHistogram exceptions") {
    /*This test tests if the fprintHistogram function throws an exception
    The fprintHistogram function should throw an exception if the number of bins
    is negative or if the quantity is not valid or if the format is not valid
    GIVEN: the fprintHistogram function is called with invalid parameters
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    CHECK_THROWS(g.fprintHistogram(outFolder, "density", -5,
                                   "root")); // negative number of bins
    CHECK_THROWS(g.fprintHistogram(outFolder, "notValid", 6,
                                   "root")); // not valid quantity
    CHECK_THROWS(g.fprintHistogram(outFolder, "traveltime", 70,
                                   "notValid")); // not valid format
  }
  SUBCASE("fprintDistribution exception") {
    /*This test tests if the fprintDistribution function throws an exception
    The fprintDistribution function should throw an exception if the quantity is
    not valid
    GIVEN: the fprintDistribution function is called with invalid parameters
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    CHECK_THROWS(
        g.fprintDistribution(outFolder, "notValid")); // not valid quantity
  }
  SUBCASE("fprintTimeDistribution exception") {
    /*This test tests if the fprintTimeDistribution function throws an exception
    The fprintTimeDistribution function should throw an exception if the
    quantity is not valid
    GIVEN: the fprintTimeDistribution function is called with invalid parameters
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    CHECK_THROWS(g.fprintTimeDistribution(outFolder, "notValid",
                                          42.)); // not valid quantity
  }
  SUBCASE("fprintActualState exception") {
    /*This test tests if the fprintActualState function throws an exception
    The fprintActualState function should throw an exception if the quantity is
    not valid
    GIVEN: the fprintActualState function is called with invalid parameters
    WHEN: the function is called on a graph
    THEN: the function should throw an exception
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    CHECK_THROWS(
        g.fprintActualState(outFolder, "notValid")); // not valid quantity
  }
  SUBCASE("fprintHistogram output - density") {
    /*This test tests if the fprintHistogram function works correctly
    The fprintHistogram function should print the graph street density in a file
    GIVEN: the fprintHistogram function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    // add a lot of vehicles of the same type in order to see a change in
    // density
    for (int i = 0; i < 50; ++i) {
      g.addVehicle(0);
    }
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintHistogram(outFolder, "density", 10, "notRelevant");
    // compare the two files
    std::ifstream f1(outFolder + "1_den.dat");
    std::ifstream f2("./data/test/test8_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "1_den.dat").c_str());
  }
  SUBCASE("fprintHistogram output - traveltime") {
    /*This test tests if the fprintHistogram function works correctly
    The fprintHistogram function should print the graph traveltime info in a
    file GIVEN: the fprintHistogram function is called two times with different
    parameters (format)
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.setTimeScale(50);
    std::string outFolder = "./data/test/";
    for (int i = 0; i < 25; ++i) {
      g.evolve(false);
    }
    g.fprintHistogram(outFolder, "traveltime", 10, "root");
    g.fprintHistogram(outFolder, "traveltime", 10, "latex");
    // verify traveltime in all formats
    std::ifstream f1(outFolder + "25_root.dat");
    std::ifstream f2("./data/test/test8root_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::ifstream f3(outFolder + "25_t.dat");
    std::ifstream f4("./data/test/test8latex_ref.txt");
    std::string s3((std::istreambuf_iterator<char>(f3)),
                   std::istreambuf_iterator<char>());
    std::string s4((std::istreambuf_iterator<char>(f4)),
                   std::istreambuf_iterator<char>());
    CHECK(s3 == s4);
    std::remove((outFolder + "25_root.dat").c_str());
    std::remove((outFolder + "25_t.dat").c_str());
  }
  SUBCASE("fprintHistogram output - 0 traveltime") {
    /*This test tests if the fprintHistogram function works correctly
    The fprintHistogram function should print the graph traveltime info in a
    file Check if traveltime is 0 until the time reaches the time scale GIVEN:
    the fprintHistogram function is called two times with different parameters
    (format) WHEN: the function is called on a graph THEN: the function should
    print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.setTimeScale(50);
    std::string outFolder = "./data/test/";
    for (int i = 0; i < 50; ++i) {
      g.evolve(false); // not reinserting to verify 0 traveltime
    }
    g.fprintHistogram(outFolder, "traveltime", 10, "root");
    g.fprintHistogram(outFolder, "traveltime", 10, "latex");
    // here traveltime should be 0 (or empty root file)
    // compare the files
    std::ifstream f1(outFolder + "50_root.dat");
    std::ifstream f2("./data/test/test2_ref.txt"); // empty file
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::ifstream f3(outFolder + "50_t.dat");
    std::ifstream f4("./data/test/test9latex_ref.txt");
    std::string s3((std::istreambuf_iterator<char>(f3)),
                   std::istreambuf_iterator<char>());
    std::string s4((std::istreambuf_iterator<char>(f4)),
                   std::istreambuf_iterator<char>());
    CHECK(s3 == s4);
    std::remove((outFolder + "0_root.dat").c_str());
    std::remove((outFolder + "0_t.dat").c_str());
    std::remove((outFolder + "50_root.dat").c_str());
    std::remove((outFolder + "50_t.dat").c_str());
  }
  SUBCASE("fprintDistribution output - u/q") {
    /*This test tests if the fprintDistribution function works correctly
    The fprintDistribution function should print the graph u/q info in a file
    GIVEN: the fprintDistribution function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    // test also for time in file name
    for (int i = 0; i < 10; ++i) {
      g.evolve();
    }
    std::string outFolder = "./data/test/";
    g.fprintDistribution(outFolder, "u/q");
    // compare the two files
    std::ifstream f1(outFolder + "10_u-q.dat");
    std::ifstream f2("./data/test/test10uq_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "10_u-q.dat").c_str());
  }
  SUBCASE("fprintDistribution output - q/k") {
    /*This test tests if the fprintDistribution function works correctly
    The fprintDistribution function should print the graph q/k info in a file
    GIVEN: the fprintDistribution function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(2); // test for other vehicle types
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintDistribution(outFolder, "q/k");
    // compare the two files
    std::ifstream f1(outFolder + "1_q-k.dat");
    std::ifstream f2("./data/test/test10qk_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "1_q-k.dat").c_str());
  }
  SUBCASE("fprintDistribution output - u/k") {
    /*This test tests if the fprintDistribution function works correctly
    The fprintDistribution function should print the graph u/k info in a file
    GIVEN: the fprintDistribution function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintDistribution(outFolder, "u/k");
    // compare the two files
    std::ifstream f1(outFolder + "1_u-k.dat");
    std::ifstream f2("./data/test/test10uk_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "1_u-k.dat").c_str());
  }
  SUBCASE("fprintTimeDistribution output - q") {
    /*This test tests if the fprintTimeDistribution function works correctly
    The fprintTimeDistribution function should print the graph q/t info in a
    file GIVEN: the fprintTimeDistribution function is called WHEN: the function
    is called on a graph THEN: the function should print the graph's info in a
    file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintTimeDistribution(outFolder, "q", 10.);
    // compare the two files
    std::ifstream f1(outFolder + "q-t.dat");
    std::ifstream f2("./data/test/test11qt_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "q-t.dat").c_str());
  }
  SUBCASE("fprintTimeDistribution output - k") {
    /*This test tests if the fprintTimeDistribution function works correctly
    The fprintTimeDistribution function should print the graph k/t info in a
    file GIVEN: the fprintTimeDistribution function is called WHEN: the function
    is called on a graph THEN: the function should print the graph's info in a
    file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintTimeDistribution(outFolder, "k", 0.);
    // compare the two files
    std::ifstream f1(outFolder + "k-t.dat");
    std::ifstream f2("./data/test/test11kt_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "k-t.dat").c_str());
  }
  SUBCASE("fprintTimeDistribution output - v") {
    /*This test tests if the fprintTimeDistribution function works correctly
    The fprintTimeDistribution function should print the graph v/t info in a
    file GIVEN: the fprintTimeDistribution function is called WHEN: the function
    is called on a graph THEN: the function should print the graph's info in a
    file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(1);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintTimeDistribution(outFolder, "u", -10.);
    // compare the two files
    std::ifstream f1(outFolder + "u-t.dat");
    std::ifstream f2("./data/test/test11ut_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "u-t.dat").c_str());
  }
  SUBCASE("fprintActualState output - q/k") {
    /*This test tests if the fprintActualState function works correctly
    The fprintActualState function should print the graph q/k info in a file
    GIVEN: the fprintActualState function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintActualState(outFolder, "q/k");
    // compare the two files
    std::ifstream f1(outFolder + "q-k.dat");
    std::ifstream f2("./data/test/test12qk_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "q-k.dat").c_str());
  }
  SUBCASE("fprintActualState output - u/k") {
    /*This test tests if the fprintActualState function works correctly
    The fprintActualState function should print the graph u/k info in a file
    GIVEN: the fprintActualState function is called
    WHEN: the function is called on a graph
    THEN: the function should print the graph's info in a file
    */
    Graph g("./data/matrix.dat");
    g.addVehicle(0);
    g.updateTransMatrix();
    g.evolve();
    std::string outFolder = "./data/test/";
    g.fprintActualState(outFolder, "u/k");
    // compare the two files
    std::ifstream f1(outFolder + "u-k.dat");
    std::ifstream f2("./data/test/test12uk_ref.txt");
    std::string s1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());
    CHECK(s1 == s2);
    std::remove((outFolder + "u-k.dat").c_str());
  }
}