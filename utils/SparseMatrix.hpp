//! SparseMatrix class v1.7.0 by Grufoony

//!  This class implements a sparse matrix. The matrix is stored in a compressed
//!  row format. ++ 20 requiered.

#ifndef SparseMatrix_hpp
#define SparseMatrix_hpp

#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

template <typename T> class SparseMatrix {
  std::unordered_map<int, T> _matrix = {};
  int _rows = 0, _cols = 0;
  static constexpr T _defaultReturn = 0;
  bool _setSeed = false;
  int _seed;
  uint _randomGeneratedNumbers;

public:
  SparseMatrix() = default;
  /// @brief SparseMatrix constructor
  /// @param rows number of rows
  /// @param cols number of columns
  /// @throw std::invalid_argument if rows or cols are < 0
  SparseMatrix(int rows, int cols) {
    rows < 0 || cols < 0
        ? throw std::invalid_argument("SparseMatrix: rows and cols must be > 0")
        : _rows = rows,
          _cols = cols;
  };
  /// @brief SparseMatrix constructor - colum
  /// @param index number of rows
  /// @throw std::invalid_argument if index is < 0
  SparseMatrix(int index) {
    index < 0 ? throw std::invalid_argument("SparseMatrix: index must be > 0")
              : _rows = index,
                _cols = 1;
  };
  /// @brief Function to read a matrix from a file and rewrite it in a listed
  /// way
  /// @param filename name of the file
  static void encode(std::string const &filename) {
    std::fstream file;
    int rows, cols;
    std::unordered_map<int, T> matrix = {};
    file.open(filename, std::ios::in);
    file >> rows >> cols;
    T value;
    int i = 0;
    while (file >> value) {
      if (value != 0) {
        matrix.emplace(std::make_pair(i, value));
      }
      ++i;
    }
    file.close();
    file.open(filename, std::ios::out);
    file << rows << '\t' << cols << '\n';
    for (auto const &[key, value] : matrix) {
      file << key << '\t' << value << '\n';
    }
    file.close();
  }
  /// @brief Function to read a matrix from a file and rewrite it as a full
  /// matrix
  /// @param filename name of the file
  static void decode(std::string const &filename) {
    std::fstream file(filename);
    int rows, cols, index;
    std::unordered_map<int, T> matrix = {};
    file >> rows >> cols;
    T value;
    while (file >> index >> value) {
      matrix.emplace(std::make_pair(index, value));
    }
    file.close();
    file.open(filename, std::ios::out);
    file << rows << '\t' << cols << '\n';
    for (int i = 0; i < rows * cols; ++i) {
      if (matrix.find(i) != matrix.end()) {
        file << matrix[i] << '\t';
      } else {
        file << 0 << '\t';
      }
      if ((i + 1) % cols == 0) {
        file << '\n';
      }
    }
  }
  /// @brief Set random seed
  /// @param seed seed
  void setSeed(int seed) noexcept {
    _seed = seed;
    _randomGeneratedNumbers = 0;
    _setSeed = true;
  }
  /// @brief insert a value in the matrix
  /// @param i row index
  /// @param j column index
  /// @param value value to insert
  /// @throw std::out_of_range if the index is out of range
  void insert(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i * _cols + j, value));
  };
  /// @brief insert a value in the matrix
  /// @param i index
  /// @param value value to insert
  /// @throw std::out_of_range if the index is out of range
  void insert(int i, T value) {
    if (i >= _rows * _cols || i < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.emplace(std::make_pair(i, value));
  };
  /// @brief insert a value in the matrix. If the element already exist, it
  /// overwrites it
  /// @param i row index
  /// @param j column index
  /// @param value value to insert
  /// @throw std::out_of_range if the index is out of range
  void insert_or_assign(int i, int j, T value) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.insert_or_assign(i * _cols + j, value);
  };
  /// @brief insert a value in the matrix. If the element already exist, it
  /// overwrites it
  /// @param index index in vectorial form
  /// @param value value to insert
  /// @throw std::out_of_range if the index is out of range
  void insert_or_assign(int index, T value) {
    if (index < 0 || index > _rows * _cols - 1) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.insert_or_assign(index, value);
  };
  /// @brief remove a value from the matrix
  /// @param i row index
  /// @param j column index
  /// @throw std::out_of_range if the index is out of range
  /// @throw std::runtime_error if the element is not found
  void erase(int i, int j) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    _matrix.find(i * _cols + j) != _matrix.end()
        ? _matrix.erase(i * _cols + j)
        : throw std::runtime_error("SparseMatrix: element not found");
  };
  /// @brief remove a row from the matrix
  /// @param index row index
  /// @throw std::out_of_range if the index is out of range
  void eraseRow(int index) {
    if (index < 0 || index > _rows - 1) {
      throw std::out_of_range("Index out of range");
    }
    for (int i = 0; i < _cols; ++i) {
      _matrix.erase(index * _cols + i);
    }
    std::unordered_map<int, T> new_matrix = {};
    for (auto const &[key, value] : _matrix) {
      if (key / _cols < index) {
        new_matrix.emplace(std::make_pair(key, value));
      } else {
        new_matrix.emplace(std::make_pair(key - _cols, value));
      }
    }
    --_rows;
    _matrix = new_matrix;
  };
  /// @brief remove a column from the matrix
  /// @param index column index
  /// @throw std::out_of_range if the index is out of range
  void eraseColumn(int index) {
    if (index < 0 || index > _cols - 1) {
      throw std::out_of_range("Index out of range");
    }
    for (int i = 0; i < _rows; ++i) {
      _matrix.erase(i * _cols + index);
    }
    std::unordered_map<int, T> new_matrix = {};
    for (auto const &[key, value] : _matrix) {
      if (key % _cols < index) {
        new_matrix.emplace(std::make_pair(key - key / _cols, value));
      } else {
        new_matrix.emplace(
            std::make_pair(key / _cols * (_cols - 1) + key % _cols - 1, value));
      }
    }
    --_cols;
    _matrix = new_matrix;
  };
  void clear() noexcept {
    _matrix.clear();
    _rows = 0;
    _cols = 0;
  };
  /// @brief check if the element is non zero
  /// @param i row index
  /// @param j column index
  /// @return true if the element is non zero
  /// @throw std::out_of_range if the index is out of range
  bool contains(int i, int j) const {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    return _matrix.contains(i * _cols + j);
  };
  /// @brief check if the element is non zero
  /// @param index index in vectorial form
  /// @return true if the element is non zero
  /// @throw std::out_of_range if the index is out of range
  bool contains(int const index) const {
    if (index < 0 || index > _rows * _cols - 1) {
      throw std::out_of_range("Index out of range");
    }
    return _matrix.contains(index);
  };
  /// @brief get the input degree of all nodes
  /// @return a SparseMatrix vector with the input degree of all nodes
  /// @throw std::runtime_error if the matrix is not square
  SparseMatrix<int> getDegreeVector() {
    if (_rows != _cols) {
      throw std::runtime_error(
          "SparseMatrix: getDegreeVector only works on square matrices");
    }
    auto degreeVector = SparseMatrix<int>(_rows, 1);
    for (auto &i : _matrix) {
      degreeVector.insert_or_assign(i.first / _cols, 0,
                                    degreeVector.at(i.first / _cols, 0) + 1);
    }
    return degreeVector;
  };
  /// @brief get the strength of all nodes
  /// @return a SparseMatrix vector with the strength of all nodes
  /// @throw std::runtime_error if the matrix is not square
  SparseMatrix<double> getStrengthVector() {
    if (_rows != _cols) {
      throw std::runtime_error(
          "SparseMatrix: getStrengthVector only works on square matrices");
    }
    auto strengthVector = SparseMatrix<double>(_rows, 1);
    for (auto &i : _matrix) {
      strengthVector.insert_or_assign(
          i.first / _cols, 0, strengthVector.at(i.first / _cols, 0) + i.second);
    }
    return strengthVector;
  };
  /// @brief get the laplacian matrix
  /// @return the laplacian matrix
  /// @throw std::runtime_error if the matrix is not square
  SparseMatrix<int> getLaplacian() {
    if (_rows != _cols) {
      throw std::runtime_error(
          "SparseMatrix: getLaplacian only works on square matrices");
    }
    auto laplacian = SparseMatrix<int>(_rows, _cols);
    for (auto &i : _matrix) {
      laplacian.insert_or_assign(i.first / _cols, i.first % _cols, -1);
    }
    auto degreeVector = this->getDegreeVector();
    for (int i = 0; i < _rows; i++) {
      laplacian.insert_or_assign(i, i, degreeVector.at(i, 0));
    }
    return laplacian;
  };

  /// @brief get a row as a row vector
  /// @param index row index
  /// @return a row vector
  /// @throw std::out_of_range if the index is out of range
  SparseMatrix getRow(int index) const {
    if (index >= _rows || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    SparseMatrix row(1, _cols);
    for (auto &it : _matrix) {
      if (it.first / _cols == index) {
        row.insert(it.first % _cols, it.second);
      }
    }
    return row;
  }
  /// @brief get a column as a column vector
  /// @param index column index
  /// @return a column vector
  /// @throw std::out_of_range if the index is out of range
  SparseMatrix getCol(int index) const {
    if (index >= _cols || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    SparseMatrix col(_rows, 1);
    for (auto &it : _matrix) {
      if (it.first % _cols == index) {
        col.insert(it.first / _cols, it.second);
      }
    }
    return col;
  }
  /// @brief get a random non-zero element from a row
  /// @param index row index
  /// @return a pair containing the column index and the value
  /// @throw std::out_of_range if the index is out of range
  std::pair<int, T> getRndRowElement(int index) {
    if (index >= _rows || index < 0)
      throw std::out_of_range("Index out of range");
    auto row = this->getRow(index);
    if (row.size() == 0)
      throw std::runtime_error("SparseMatrix: row is empty");
    std::pair<int, T> res = row.getRndElement();
    res.first += index * _cols;
    return res;
  }
  /// @brief get a random non-zero element from a column
  /// @param index column index
  /// @return a pair containing the row index and the value
  /// @throw std::out_of_range if the index is out of range
  std::pair<int, T> getRndColElement(int index) {
    if (index >= _cols || index < 0)
      throw std::out_of_range("Index out of range");
    auto col = this->getCol(index);
    if (col.size() == 0)
      throw std::runtime_error("SparseMatrix: col is empty");
    std::pair<int, T> res = col.getRndElement();
    res.first *= _cols;
    res.first += index;
    return res;
  }
  /// @brief get a random non-zero element from the matrix
  /// @return a pair containing the row index and the value
  /// @throw std::runtime_error if the matrix is empty
  std::pair<int, T> getRndElement() {
    if (this->size() == 0)
      throw std::runtime_error("SparseMatrix: matrix is empty");
    auto it = _matrix.begin();
    std::mt19937 rng(std::random_device{}());
    if (_setSeed) {
      rng.seed(_seed);
      rng.discard(
          _randomGeneratedNumbers); // maybe not that smart, but it works
    }
    auto dist = std::uniform_int_distribution<int>(0, this->size() - 1);
    std::advance(it, dist(rng));
    std::pair<int, T> res = *it;
    ++_randomGeneratedNumbers;
    return res;
  }
  /// @brief get a matrix of double with every row normalized to 1
  /// @return a matrix of double
  SparseMatrix<double> getNormRows() const {
    SparseMatrix<double> normRows(_rows, _cols);
    for (int index = 0; index < _rows; index++) {
      auto row = this->getRow(index);
      double sum = 0.;
      for (auto &it : row) {
        sum += std::abs(it.second);
      }
      sum < std::numeric_limits<double>::epsilon() ? sum = 1. : sum = sum;
      for (auto &it : row) {
        normRows.insert(it.first + index * _cols, it.second / sum);
      }
    }
    return normRows;
  }
  /// @brief get a matrix of double with every column normalized to 1
  /// @return a matrix of double
  SparseMatrix<double> getNormCols() const {
    SparseMatrix<double> normCols(_rows, _cols);
    for (int index = 0; index < _cols; index++) {
      auto col = this->getCol(index);
      double sum = 0.;
      for (auto &it : col) {
        sum += std::abs(it.second);
      }
      sum < std::numeric_limits<double>::epsilon() ? sum = 1. : sum = sum;
      for (auto &it : col) {
        normCols.insert(it.first * _cols + index, it.second / sum);
      }
    }
    return normCols;
  }
  /// @brief get the number of rows
  /// @return number of rows
  int getRowDim() const noexcept { return this->_rows; }
  /// @brief get the number of columns
  /// @return number of columns
  int getColDim() const noexcept { return this->_cols; }
  /// @brief get the number of non zero elements in the matrix
  /// @return number of non zero elements
  int size() const noexcept { return _matrix.size(); };
  /// @brief get the maximum number of elements in the matrix
  /// @return maximum number of elements
  int max_size() const noexcept { return this->_rows * this->_cols; }
  /// @brief access an element of the matrix
  /// @param i row index
  /// @param j column index
  /// @return the element
  /// @throw std::out_of_range if the index is out of range
  T at(int i, int j) const {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    auto const &it = _matrix.find(i * _cols + j);
    return it != _matrix.end() ? it->second : _defaultReturn;
  }
  /// @brief access an element of the matrix
  /// @param index index in vectorial form
  /// @return the element
  /// @throw std::out_of_range if the index is out of range
  T at(int index) const {
    if (index >= _rows * _cols || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    auto const &it = _matrix.find(index);
    return it != _matrix.end() ? it->second : _defaultReturn;
  }
  /// @brief symmetrize the matrix
  void symmetrize() { *this += this->operator++(); }

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
  /// @brief print the matrix on a file
  /// @param filename name of the file
  void fprint(std::string const &filename) const noexcept {
    std::ofstream file(filename);
    file << _rows << '\t' << _cols << '\n';
    // move buffer on file
    auto const rdbufBackup = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());
    this->print();
    std::cout.rdbuf(rdbufBackup);
    file.close();
  }
  /// @brief return the begin iterator of the matrix
  /// @return the begin iterator
  typename std::unordered_map<int, T>::const_iterator begin() const noexcept {
    return _matrix.begin();
  }
  /// @brief return the end iterator of the matrix
  /// @return the end iterator
  typename std::unordered_map<int, T>::const_iterator end() const noexcept {
    return _matrix.end();
  }
  /// @brief access an element of the matrix
  /// @param i row index
  /// @param j column index
  /// @return the element
  /// @throw std::out_of_range if the index is out of range
  T const &operator()(int i, int j) {
    if (i >= _rows || j >= _cols || i < 0 || j < 0) {
      throw std::out_of_range("Index out of range");
    }
    auto const &it = _matrix.find(i * _cols + j);
    return it != _matrix.end() ? it->second : _defaultReturn;
  }
  /// @brief access an element of the matrix
  /// @param index index in vectorial form
  /// @return the element
  /// @throw std::out_of_range if the index is out of range
  T const &operator()(int index) {
    if (index >= _rows * _cols || index < 0) {
      throw std::out_of_range("Index out of range");
    }
    auto const &it = _matrix.find(index);
    return it != _matrix.end() ? it->second : _defaultReturn;
  }
  /// @brief print the matrix on a stream
  friend std::ostream &operator<<(std::ostream &os, const SparseMatrix &m) {
    os << m._rows << '\t' << m._cols << '\n';
    for (auto &it : m._matrix) {
      os << it.first << '\t' << it.second << '\n';
    }
    return os;
  }
  /// @brief read the matrix from a stream
  friend std::istream &operator>>(std::istream &is, SparseMatrix &m) {
    is >> m._rows >> m._cols;
    int index;
    T value;
    while (is >> index >> value) {
      m._matrix.emplace(std::make_pair(index, value));
    }
    return is;
  }
  /// @brief sum of two matrices
  /// @param other the other matrix
  /// @return the sum of the two matrices
  /// @throw std::runtime_error if the dimensions do not match
  template <typename U>
  SparseMatrix operator+(const SparseMatrix<U> &other) const {
    if (this->_rows != other._rows || this->_cols != other._cols) {
      throw std::runtime_error("SparseMatrix: dimensions do not match");
    }
    auto result = SparseMatrix(this->_rows, this->_cols);
    std::unordered_map<int, bool> unique;
    for (auto &it : this->_matrix) {
      unique.insert_or_assign(it.first, true);
    }
    for (auto &it : other._matrix) {
      unique.insert_or_assign(it.first, true);
    }
    for (auto &it : unique) {
      result.insert(it.first / this->_cols, it.first % this->_cols,
                    this->at(it.first) + other.at(it.first));
    }
    return result;
  }
  /// @brief difference of two matrices
  /// @param other the other matrix
  /// @return the difference of the two matrices
  /// @throw std::runtime_error if the dimensions do not match
  template <typename U>
  SparseMatrix operator-(const SparseMatrix<U> &other) const {
    if (this->_rows != other._rows || this->_cols != other._cols) {
      throw std::runtime_error("SparseMatrix: dimensions do not match");
    }
    auto result = SparseMatrix(this->_rows, this->_cols);
    std::unordered_map<int, bool> unique;
    for (auto &it : this->_matrix) {
      unique.insert_or_assign(it.first, true);
    }
    for (auto &it : other._matrix) {
      unique.insert_or_assign(it.first, true);
    }
    for (auto &it : unique) {
      result.insert(it.first / this->_cols, it.first % this->_cols,
                    this->at(it.first) - other.at(it.first));
    }
    return result;
  }
  // template <typename U>
  // SparseMatrix operator*(const SparseMatrix<U> &other) const {
  //   if (this->_cols != other._rows) {
  //     throw std::runtime_error("SparseMatrix: dimensions do not match");
  //   }
  //   auto result = SparseMatrix(this->_rows, other._cols);
  //   for (int i = 0; i < this->_rows; ++i) {
  //     for (int j = 0; j < other._cols; ++j) {
  //       T sum = 0;
  //       for (int k = 0; k < this->_cols; ++k) {
  //         sum += this->at(i, k) * other.at(k, j);
  //       }
  //       if (sum != 0) {
  //         result.insert(i, j, sum);
  //       }
  //     }
  //   }
  //   return result;
  // }
  /// @brief transpose the matrix
  /// @return the transposed matrix
  SparseMatrix operator++() {
    auto transpost = SparseMatrix(this->_cols, this->_rows);
    for (auto &it : _matrix) {
      transpost.insert(it.first % _cols, it.first / _cols, it.second);
    }
    return transpost;
  }
  /// @brief sum of two matrices
  /// @param other the other matrix
  /// @return the sum of the two matrices
  /// @throw std::runtime_error if the dimensions do not match
  template <typename U> SparseMatrix &operator+=(const SparseMatrix<U> &other) {
    if (this->_rows != other._rows || this->_cols != other._cols) {
      throw std::runtime_error("SparseMatrix: dimensions do not match");
    }
    for (auto &it : other._matrix) {
      this->contains(it.first)
          ? this->insert_or_assign(it.first,
                                   this->operator()(it.first) + it.second)
          : this->insert(it.first, it.second);
    }
    return *this;
  }
  /// @brief difference of two matrices
  /// @param other the other matrix
  /// @return the difference of the two matrices
  /// @throw std::runtime_error if the dimensions do not match
  template <typename U> SparseMatrix &operator-=(const SparseMatrix<U> &other) {
    if (this->_rows != other._rows || this->_cols != other._cols) {
      throw std::runtime_error("SparseMatrix: dimensions do not match");
    }
    for (auto &it : other._matrix) {
      this->contains(it.first)
          ? this->insert_or_assign(it.first,
                                   this->operator()(it.first) - it.second)
          : this->insert(it.first, -it.second);
    }
    return *this;
  }
  // template <typename U> SparseMatrix &operator*=(const SparseMatrix<U>
  // &other) {
  //   if (this->_cols != other._rows) {
  //     throw std::runtime_error("SparseMatrix: dimensions do not match");
  //   }
  //   auto result = SparseMatrix(this->_rows, other._cols);
  //   for (int i = 0; i < this->_rows; ++i) {
  //     for (int j = 0; j < other._cols; ++j) {
  //       T sum = 0;
  //       for (int k = 0; k < this->_cols; ++k) {
  //         sum += this->at(i, k) * other.at(k, j);
  //       }
  //       if (sum != 0) {
  //         result.insert(i, j, sum);
  //       }
  //     }
  //   }
  //   *this = result;
  //   return *this;
  // }
};

#endif