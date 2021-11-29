//
// Created by Jiang Kairong on 2/17/18.
//

#ifndef PROG03_INLINEBOOL_MATRIX_H
#define PROG03_INLINEBOOL_MATRIX_H

#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>

/// Matrix class for both matrices and vectors
/// \tparam T Scalar data type
/// \tparam Rows Number of Rows of the matrix, 0 for dynamic sized matrices
/// \tparam Cols Number of Columns of the matrix, 0 for dynamic sized matrices
template<typename T, unsigned long Rows = 0, unsigned long Cols = 0>
class Matrix {
 public:
  explicit Matrix();

  /// construct with an initial value for all elements.
  /// \param initialValue
  explicit Matrix(T initialValue);

  /// construct with an initializer list, useful for vector construction.
  /// \param initializer
  Matrix(std::initializer_list<T> initializer);
//  ~Matrix();

  /// copy constructor
  /// \param rhs
  Matrix(const Matrix<T, Rows, Cols> &rhs);

  /// get the element of the ith row and jth column.
  /// \param i
  /// \param j
  /// \return
  const T &operator()(int i, int j) const;

  /// get the element of the ith row and jth column.
  /// \param i
  /// \param j
  /// \return
  T &operator()(int i, int j);

  /// only for vectors (one column matrices), get the element of the ith row.
  /// \param i
  /// \return
  const T &operator()(int i) const;

  /// only for vectors (one column matrices), get the element of the ith row.
  /// \param i
  /// \return
  T &operator()(int i);

  /// resize the matrix. Only valid for dynamic size matrices.
  /// \param rows
  /// \param cols
  /// \return
  Matrix &resize(unsigned long rows, unsigned long cols);

  /// resize the matrix with initialization value. Only valid for dynamic size matrices.
  /// \param rows
  /// \param cols
  /// \return
  Matrix &resize(unsigned long rows, unsigned long cols, T val);

  /// assignment operator
  /// \param rhs
  /// \return
  Matrix &operator=(Matrix rhs);

  /// matrix production
  /// \tparam RhsCols number right operand columns. Need to be the same with the number of rows of the left operand.
  /// \param rhs
  /// \return
  template<unsigned long RhsCols>
  Matrix<T, Rows, RhsCols> operator*(const Matrix<T, Cols, RhsCols> &rhs) const;

  /// scalar production
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  const Matrix operator*(const U &rhs) const;

  /// scalar plus assign
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  Matrix &operator*=(const U &rhs);

  /// scalar division
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  const Matrix operator/(const U &rhs) const;

  /// scalar divide assign
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  Matrix &operator/=(const U &rhs);

  /// scalar addition
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  const Matrix operator+(const U &rhs) const;

  /// scalar plus assign
  /// \tparam U scalar data type
  /// \param rhs
  /// \return
  template<typename U>
  Matrix &operator+=(const U &rhs);

  /// scalar subtraction
  /// \tparam U
  /// \param rhs
  /// \return
  template<typename U>
  const Matrix operator-(const U &rhs) const;

  /// scalar minus assign
  /// \tparam U
  /// \param rhs
  /// \return
  template<typename U>
  Matrix &operator-=(const U &rhs);

  /// matrix addition
  /// \param rhs
  /// \return
  const Matrix operator+(const Matrix &rhs) const;

  /// matrix plus assign
  /// \param rhs
  /// \return
  Matrix &operator+=(const Matrix &rhs);

  /// matrix subtraction
  /// \param rhs
  /// \return
  const Matrix operator-(const Matrix &rhs) const;

  /// matrix minus assign
  /// \param rhs
  /// \return
  Matrix &operator-=(const Matrix &rhs);

  /// coefficient wise product
  /// \param rhs
  /// \return
  const Matrix cwiseProduct(const Matrix &rhs) const;

  /// l2 norm
  /// \return
  double norm() const;

  /// normalize, only valid for vectors
  /// \return
  Matrix &normalize();

  /// get the normalized vector, only valid for vectors
  /// \return
  const Matrix normalize() const;

  /// get number of rows
  /// \return
  unsigned long rows() const;

  /// get number colomns
  /// \return
  unsigned long cols() const;

  /// print the matrix
  /// \param os
  void print(std::ostream &os) const;

  /// cross product. Only valid for vectors.
  /// \param rhs
  /// \return
  const Matrix cross(const Matrix &rhs) const;

  /// dot product. Only valid for vectors.
  /// \param rhs
  /// \return
  T dot(const Matrix &rhs) const;

  /// return the raw pointer to the start of the chunk of the raw data.
  /// \return
  T *getRawData();

 private:
  std::vector<T> data;
  unsigned long width, height;
};

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols>::Matrix() : width(Cols), height(Rows) {
  data = std::vector<T>(Rows * Cols, T(0));
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols>::Matrix(T initialValue) : width(Cols), height(Rows) {
  data = std::vector<T>(Rows * Cols, initialValue);
}

template<typename T, unsigned long Rows, unsigned long Cols>
unsigned long Matrix<T, Rows, Cols>::rows() const {
  return height;
}

template<typename T, unsigned long Rows, unsigned long Cols>
unsigned long Matrix<T, Rows, Cols>::cols() const {
  return width;
}

template<typename T, unsigned long Rows, unsigned long Cols>
const T &Matrix<T, Rows, Cols>::operator()(int i, int j) const {
  if (i >= height || j >= width || i < 0 || j < 0) {
    throw std::out_of_range("index out of bounds");
  }
  return data[i * width + j];
}

template<typename T, unsigned long Rows, unsigned long Cols>
T &Matrix<T, Rows, Cols>::operator()(int i, int j) {
  return const_cast<T &>((*static_cast<const Matrix<T, Rows, Cols> *>(this))(i, j));
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<unsigned long RhsCols>
Matrix<T, Rows, RhsCols> Matrix<T, Rows, Cols>::operator*(const Matrix<T, Cols, RhsCols> &rhs) const {

  if (width != rhs.rows()) {
    throw std::logic_error("wrong matrix sizes");
  }

  Matrix<T, Rows, RhsCols> res(0);

  if (Rows == 0 && RhsCols == 0) {
    res.resize(height, rhs.cols());
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < rhs.cols(); j++) {
      for (int k = 0; k < width; k++) {
        res(i, j) += (*this)(i, k) * rhs(k, j);
      }
    }
  }

  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
void Matrix<T, Rows, Cols>::print(std::ostream &os) const {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      os << (*this)(i, j) << " ";
    }
    os << std::endl;
  }
}

template<typename T, unsigned long Rows, unsigned long Cols>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator+(const Matrix<T, Rows, Cols> &rhs) const {
  if (height != rhs.rows() || width != rhs.cols()) {
    throw std::logic_error("wrong matrix sizes");
  }
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] += rhs.data[i];
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols>::Matrix(const Matrix<T, Rows, Cols> &rhs) : width(rhs.cols()), height(rhs.rows()),
                                                                  data(rhs.data) {}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator=(const Matrix<T, Rows, Cols> rhs) {
  height = rhs.rows();
  width = rhs.cols();
  data = rhs.data;

  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator-(const Matrix<T, Rows, Cols> &rhs) const {
  if (height != rhs.rows() || width != rhs.cols()) {
    throw std::logic_error("wrong matrix sizes");
  }
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] -= rhs.data[i];
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator*(const U &rhs) const {
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] *= rhs;
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator/(const U &rhs) const {
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] /= rhs;
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator+(const U &rhs) const {
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] += rhs;
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::operator-(const U &rhs) const {
  Matrix<T, Rows, Cols> res(*this);
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] -= rhs;
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
T Matrix<T, Rows, Cols>::dot(const Matrix<T, Rows, Cols> &rhs) const {
  if (Cols != 1) {
    throw std::logic_error("only vectors support dot product.");
  }
  T res(0);
  for (int i = 0; i < this->data.size(); i++) {
    res += (*this)(i) * rhs(i);
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
double Matrix<T, Rows, Cols>::norm() const {
  double res = 0.;
  for (int i = 0; i < width; i++) {
    double columnNorm = 0.;
    for (int j = 0; j < height; j++) {
      columnNorm += (*this)(j, i) * (*this)(j, i);
    }
    columnNorm = std::sqrt(columnNorm);
    res += columnNorm;
  }
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::cross(const Matrix &rhs) const {
  if (Rows != 3 || Cols != 1) {
    throw std::logic_error("only 3d vectors support cross product.");
  }
  Matrix<T, Rows, Cols> res;
  res(0) = (*this)(1) * rhs(2) - (*this)(2) * rhs(1);
  res(1) = (*this)(2) * rhs(0) - (*this)(0) * rhs(2);
  res(2) = (*this)(0) * rhs(1) - (*this)(1) * rhs(0);
  return res;
}

template<typename T, unsigned long Rows, unsigned long Cols>
T &Matrix<T, Rows, Cols>::operator()(int i) {
  if (Cols != 1) {
    throw std::logic_error("use both row and col for matrix indexing.");
  }
  return (*this)(i, 0);
}

template<typename T, unsigned long Rows, unsigned long Cols>
const T &Matrix<T, Rows, Cols>::operator()(int i) const {
  if (Cols != 1) {
    throw std::logic_error("use both row and col for matrix indexing.");
  }
  return (*this)(i, 0);
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::normalize() {
  *this = static_cast<const Matrix<T, Rows, Cols> *>(this)->normalize();
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator/=(const U &rhs) {
  *this = *this / rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator*=(const U &rhs) {
  *this = *this * rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator+=(const U &rhs) {
  *this = *this + rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
template<typename U>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator-=(const U &rhs) {
  *this = *this - rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator+=(const Matrix<T, Rows, Cols> &rhs) {
  *this = *this + rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::operator-=(const Matrix<T, Rows, Cols> &rhs) {
  *this = *this - rhs;
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<T> initializer) : width(Cols), height(Rows) {
  if (initializer.size() != Rows * Cols) {
    throw std::logic_error("wrong initializer list size.");
  }
  data = std::vector<T>(initializer);
}

template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::resize(unsigned long rows, unsigned long cols) {
  if (Rows || Cols) {
    throw std::logic_error("fixed sized matrices cannot be resized.");
  }
  height = rows;
  width = cols;
  data.resize(height * width);
  return *this;
}

template<typename T, unsigned long Rows, unsigned long Cols>
T *Matrix<T, Rows, Cols>::getRawData() {
  return data.data();
}

template<typename T, unsigned long Rows, unsigned long Cols>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::normalize() const {
  if (this->norm() == 1.) {
    return *this;
  }
  return *this / this->norm();
}

template<typename T, unsigned long Rows, unsigned long Cols>
const Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::cwiseProduct(const Matrix<T, Rows, Cols> &rhs) const {
  if (width != rhs.cols() || height != rhs.rows()) {
    throw std::logic_error("wrong matrix sizes.");
  }
  auto res = *this;
  for (int i = 0; i < res.data.size(); i++) {
    res.data[i] *= rhs.data[i];
  }
  return res;
}
template<typename T, unsigned long Rows, unsigned long Cols>
Matrix<T, Rows, Cols> &Matrix<T, Rows, Cols>::resize(unsigned long rows, unsigned long cols, T val) {
  if (Rows || Cols) {
    throw std::logic_error("fixed sized matrices cannot be resized.");
  }
  height = rows;
  width = cols;
  data.resize(height * width, val);
  return *this;
}

using Matrix2f = Matrix<float, 2, 2>;
using Matrix2d = Matrix<double, 2, 2>;
using Matrix3d = Matrix<double, 3, 3>;
using Matrix4d = Matrix<double, 4, 4>;
using Vector3d = Matrix<double, 3, 1>;
using Vector4d = Matrix<double, 4, 1>;
using Vector3f = Matrix<float, 3, 1>;
using Vector3uch = Matrix<unsigned char, 3, 1>;
using Vector3i = Matrix<int, 3, 1>;
#endif //PROG03_INLINEBOOL_MATRIX_H
