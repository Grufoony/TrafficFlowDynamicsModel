//! SparseMatrix class v1.4 by Grufoony
//! https://github.com/Grufoony/miscellaneous

//!  This class implements a sparse matrix. The matrix is stored in a compressed
//!  row format. ++ 20 requiered. Docs at
//!  https://grufoony.github.io/TrafficFlowDynamicsModel/classSparseMatrix.html.

#ifndef SparseMatrix_hpp
#define SparseMatrix_hpp

#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

template <typename T> class SparseMatrix {
  std::unordered_map<int, T> _matrix = {};
  int _rows = 0, _cols = 0;
  static constexpr T _defaultReturn = 0;

public:
  SparseMatrix() = default;
  /// \brief SparseMatrix constructor
  /// \param rows number of rows
  /// \param cols number of columns
  SparseMatrix(int rows, int cols) {
    rows < 0 || cols < 0
        ? throw std::invalid_argument("SparseMatrix: rows and cols must be > 0")
        : _rows = rows,
          _cols = cols;
  };
  /// \brief SparseMatrix constructor
  /// \param fName matrix file path
  SparseMatrix(std::string fName) {
    std::ifstream file(fName);
    if (!file.is_open())
      throw std::invalid_argument("SparseMatrix: file not found");
    file >> _rows >> _cols;
    int pos;
    T value;
    while (file >> pos >> value) {
      if (pos < 0 || pos > (_rows * _cols) - 1)
        throw std::invalid_argument("SparseMatrix: index out of bounds");
      _matrix.insert({pos, value});
    }
    file.close();
  }
  /// \brief SparseMatrix constructor
  /// \param other other SparseMatrix
  SparseMatrix(SparseMatrix const &other) {
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_matrix = other._matrix;
  };

  /// \brief insert a value in the matrix
  /// \param i row index
  /// \param j column index
  /// \param value value to insert
  void insert(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i * _cols + j, value));
  };
  /// \brief insert a value in the matrix
  /// \param i index
  /// \param value value to insert
  void insert(int i, T value) {
    if (i >= _rows * _cols || i < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i, value));
  };
  /// \brief insert a value in the matrix. If the element already exist, it
  /// overwrites it \param i row index \param j column index \param value value
  /// to insert
  void insert_or_assign(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.insert_or_assign(i * _cols + j, value);
  };
  /// \brief remove a value from the matrix
  /// \param i row index
  /// \param j column index
  void erase(int i, int j) {
    _matrix.find(i * _cols + j) != _matrix.end()
        ? _matrix.erase(i * _cols + j)
        : throw std::out_of_range("Index out of range");
  };
  void clear() noexcept { _matrix.clear(); };
  /// \brief check if the element is non zero
  bool contains(int i, int j) const noexcept {
    return _matrix.contains(i * _cols + j);
  };
  std::vector<int> getDegreeVector() {
    if (_rows != _cols || !std::is_same<T, bool>::value) {
      throw std::runtime_error("SparseMatrix: getDegreeVector only works on "
                               "square boolean matrices");
    }
    std::vector<int> degreeVector(_rows, 0);
    for (auto &i : _matrix) {
      degreeVector[i.first / _cols]++;
    }
    return degreeVector;
  };

  std::unordered_map<int, T> getRow(int index) const {
    if (index >= _rows || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    std::unordered_map<int, T> row;
    for (auto &it : _matrix) {
      if (it.first / _cols == index) {
        row.emplace(std::make_pair(it.first % _cols, it.second));
      }
    }
    return row;
  }
  std::unordered_map<int, T> getCol(int index) const {
    if (index >= _cols || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    std::unordered_map<int, T> col;
    for (auto &it : _matrix) {
      if (it.first % _cols == index) {
        col.emplace(std::make_pair(it.first / _cols, it.second));
      }
    }
    return col;
  }
  /// @brief get a random element from a row
  /// @param index row index
  /// @return a pair containing the column index and the value
  std::pair<int, T> getRndRowElement(int index) const {
    if (index >= _rows || index < 0)
      throw std::out_of_range("Index out of range");
    auto row = this->getRow(index);
    if (row.size() == 0)
      throw std::runtime_error("SparseMatrix: row is empty");
    auto it = row.begin();
    std::random_device dev;
    std::mt19937 rng(dev());
    auto dist = std::uniform_int_distribution<int>(0, row.size() - 1);
    std::advance(it, dist(rng));
    return *it;
  }
  /// @brief get a random element from a column
  /// @param index column index
  /// @return a pair containing the row index and the value
  std::pair<int, T> getRndColElement(int index) const {
    if (index >= _cols || index < 0)
      throw std::out_of_range("Index out of range");
    auto col = this->getCol(index);
    if (col.size() == 0)
      throw std::runtime_error("SparseMatrix: col is empty");
    auto it = col.begin();
    std::random_device dev;
    std::mt19937 rng(dev());
    auto dist = std::uniform_int_distribution<int>(0, col.size() - 1);
    std::advance(it, dist(rng));
    return *it;
  }
  int getRowDim() const noexcept { return this->_rows; };
  int getColDim() const noexcept { return this->_cols; };
  int size() const noexcept { return this->_rows * this->_cols; };

  /// @brief print the matrix in standard output
  void print() const noexcept {
    for (int i = 0; i < _rows; ++i) {
      for (int j = 0; j < _cols; ++j) {
        auto const &it = _matrix.find(i * _cols + j);
        it != _matrix.end() ? std::cout << it->second : std::cout << 0;
        std::cout << '\t';
      }
      std::cout << '\n';
    }
  }
  /// @brief save the matrix in a file in map format
  void save(std::string fName) const {
    std::ofstream file(fName);
    file << _rows << '\t' << _cols << '\n';
    for (auto const &it : _matrix) {
      file << it.first << '\t' << it.second << '\n';
    }
    file.close();
  }
  /// @brief save the matrix in a file in matrix format
  void saveAsMatrix(std::string fName) const {
    std::ofstream file(fName);
    for (int i = 0; i < _rows; ++i) {
      for (int j = 0; j < _cols; ++j) {
        auto const &it = _matrix.find(i * _cols + j);
        it != _matrix.end() ? file << it->second : file << 0;
        file << '\t';
      }
      file << '\n';
    }
    file.close();
  }

  T const &operator()(int i, int j) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    auto const &it = _matrix.find(i * _cols + j);
    return it != _matrix.end() ? it->second : _defaultReturn;
  }
  SparseMatrix &operator=(const SparseMatrix &other) {
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_matrix = other._matrix;
    return *this;
  }
};

#endif