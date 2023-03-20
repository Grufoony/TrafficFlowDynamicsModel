#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./utils/SparseMatrix.hpp"
#include "./utils/doctest.h"

TEST_CASE("Boolean Matrix") {
  SUBCASE("Default constructor") {
    SparseMatrix<bool> m;
    CHECK(m.getRowDim() == 0);
    CHECK(m.getColDim() == 0);
    CHECK(m.max_size() == 0);
  }
  SUBCASE("Constructor with dimensions") {
    SparseMatrix<bool> m(3, 3);
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 3);
    CHECK(m.max_size() == 9);
    CHECK_THROWS(m(-1, -1));
  }
  SUBCASE("Constructor with dimension") {
    SparseMatrix<bool> m(3);
    CHECK(m.getRowDim() == 3);
    CHECK(m.getColDim() == 1);
    CHECK(m.max_size() == 3);
    CHECK_THROWS(m(-1));
  }
  SUBCASE("Insertions") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    CHECK(m(0, 0));
    for (int i = 1; i < 9; ++i) {
      CHECK(!m(i / 3, i % 3));
    }
    CHECK_THROWS(m(-1, -1));
  }
  SUBCASE("Deletions") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.erase(0, 0);
    CHECK(!m(0, 0));
  }
  SUBCASE("Clear") {
    SparseMatrix<bool> m(3, 3);
    m.insert(0, 0, true);
    m.clear();
    CHECK(m.size() == 0);
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
    d.insert(0, 0, true);
    d.insert(1, 2, true);
    d.insert(2, 1, true);
    auto m = d;
    m.eraseRow(1);
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
    m.insert(1, 2, true);
    m.insert(2, 1, true);
    auto v = m.getDegreeVector();
    CHECK(v(0) == 1);
    CHECK(v(1) == 1);
    CHECK(v(2) == 1);
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
    m.insert(0, 0, true);
    m.insert(0, 1, true);
    m.insert(0, 2, true);
    m.insert(1, 1, true);
    m.insert(1, 2, true);
    m.insert(2, 1, true);
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
    m.symmetrize();
    for (int i = 0; i < 9; ++i) {
      if (i != 4 && i != 8 && i != 2 && i != 6) {
        CHECK(m.contains(i));
      } else {
        CHECK(!m.contains(i));
      }
    }
  }
}