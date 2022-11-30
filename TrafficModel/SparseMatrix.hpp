///////////////////////////////////////////////////////////////////////////////
// SPARSE MATRIX CLASS v 1.0
// by Grufoony https://github.com/Grufoony
///////////////////////////////////////////////////////////////////////////////
// This class implements a sparse matrix.  The matrix is stored in a
// compressed row format.
///////////////////////////////////////////////////////////////////////////////

#ifndef SparseMatrix_hpp
#define SparseMatrix_hpp

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <type_traits>

template <typename T> class SparseMatrix {
  std::unordered_map<int, T> _matrix = {};
  int _rows = 0, _cols = 0;
  static T constexpr defaultReturn = 0;

public:
  SparseMatrix() = default;
  SparseMatrix(int rows, int cols) {
    rows < 0 || cols < 0
        ? throw std::invalid_argument("SparseMatrix: rows and cols must be > 0")
        : _rows = rows,
          _cols = cols;
  };
  SparseMatrix(const char *fName) {
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
  SparseMatrix(SparseMatrix const &other) {
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_matrix = other._matrix;
  };

  std::unordered_map<int,bool> const& getMatrix() { return _matrix; }

  void insert(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i * _cols + j, value));
  };
  void insert(int i, T value) {
    if (i >= _rows * _cols || i < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i, value));
  };
  void insert_or_assign(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.insert_or_assign(i * _cols + j, value);
  };
  void erase(int i, int j) {
    _matrix.find(i * _cols + j) != _matrix.end()
        ? _matrix.erase(i * _cols + j)
        : throw std::out_of_range("Index out of range");
  };
  void clear() noexcept { _matrix.clear(); };
  bool exists(int i, int j) const noexcept {
    return _matrix.count(i * _cols + j);
  };
  std::vector<int> getDegreeVector() {
    if(_rows != _cols || !std::is_same<T, bool>::value) {
      throw std::runtime_error("SparseMatrix: getDegreeVector only works on square boolean matrices");
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
  int getRowDim() const noexcept { return this->_rows; };
  int getColDim() const noexcept { return this->_cols; };
  int size() const noexcept { return this->_rows * this->_cols; };

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
  void save(const char *fName) const {
    std::ofstream file(fName);
    file << _rows << '\t' << _cols << '\n';
    for (auto const &it : _matrix) {
      file << it.first << '\t' << it.second << '\n';
    }
    file.close();
  }
  void saveAsMatrix(const char *fName) const {
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
    return it != _matrix.end() ? it->second : defaultReturn;
  }
  SparseMatrix &operator=(const SparseMatrix &other) {
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_matrix = other._matrix;
    return *this;
  }
};

#endif
