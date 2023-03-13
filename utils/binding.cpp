#include "SparseMatrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(SparseMatrix, m) {
    m.doc() = "A library for managing sparse matrices.";

    // SparseMatrix class
    pybind11::class_<SparseMatrix<bool>>(m, "SparseMatrix")
      .def(pybind11::init<>())
      .def(pybind11::init<int, int>())
      .def(pybind11::init<int>())
      .def(pybind11::init<SparseMatrix<bool> const &>())
      .def("insert", static_cast<void (SparseMatrix<bool>::*)(int, int, bool)>(
                         &SparseMatrix<bool>::insert))
      .def("insert", static_cast<void (SparseMatrix<bool>::*)(int, bool)>(
                         &SparseMatrix<bool>::insert))
      .def("insert_or_assign",
           static_cast<void (SparseMatrix<bool>::*)(int, int, bool)>(
               &SparseMatrix<bool>::insert_or_assign))
      .def("insert_or_assign",
           static_cast<void (SparseMatrix<bool>::*)(int, bool)>(
               &SparseMatrix<bool>::insert_or_assign))
      .def("erase", &SparseMatrix<bool>::erase)
      .def("clear", &SparseMatrix<bool>::clear)
      .def("contains",
           static_cast<bool (SparseMatrix<bool>::*)(int, int) const>(
               &SparseMatrix<bool>::contains))
      .def("contains", static_cast<bool (SparseMatrix<bool>::*)(int) const>(
                           &SparseMatrix<bool>::contains))
      .def("getRow", &SparseMatrix<bool>::getRow)
      .def("getCol", &SparseMatrix<bool>::getCol)
      .def("getRndRowElement", &SparseMatrix<bool>::getRndRowElement)
      .def("getRndColElement", &SparseMatrix<bool>::getRndColElement)
      .def("getNormRows", &SparseMatrix<bool>::getNormRows)
      .def("getNormCols", &SparseMatrix<bool>::getNormCols)
      .def("getRowDim", &SparseMatrix<bool>::getRowDim)
      .def("getColDim", &SparseMatrix<bool>::getColDim)
      .def("size", &SparseMatrix<bool>::size)
      .def("print", &SparseMatrix<bool>::print);
}