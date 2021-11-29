//
// Created by Jiang Kairong on 4/9/18.
//

#include "Utils.h"

Vector3d Utils::homoDivideVector4d(const Vector4d &rhs) {
  return Vector3d({rhs(0) / rhs(3), rhs(1) / rhs(3), rhs(2) / rhs(3)});
}
Matrix4d Utils::make3dChangeCoordSysMatrix(const Vector3d &u, const Vector3d &v, const Vector3d &w) {
  Matrix4d res(0.);
  res(0, 0) = u(0);
  res(0, 1) = u(1);
  res(0, 2) = u(2);
  res(1, 0) = v(0);
  res(1, 1) = v(1);
  res(1, 2) = v(2);
  res(2, 0) = w(0);
  res(2, 1) = w(1);
  res(2, 2) = w(2);
  res(3, 3) = 1;
  return res;
}
Matrix4d Utils::make3dTranslateMatrix(const Vector3d &t) {
  Matrix4d res(0.);
  for (int r = 0; r < 4; r++) {
    res(r, r) = 1.;
    if (r < 3) {
      res(r, 3) = -t(r);
    }
  }
  return res;
}
Matrix4d Utils::makePerspectiveProjectionMatrix(double n,
                                                double f,
                                                double b,
                                                double t,
                                                double l,
                                                double r) {
  Matrix4d res(0.);
  res(0, 0) = 2 * n / (r - l);
  res(0, 2) = (l + r) / (l - r);
  res(1, 1) = 2 * n / (t - b);
  res(1, 2) = (b + t) / (b - t);
  res(2, 2) = (f + n) / (n - f);
  res(2, 3) = 2 * f * n / (f - n);
  res(3, 2) = 1.;
  return res;
}
Matrix4d Utils::makeViewPortTransformMatrix(int nx, int ny) {
  Matrix4d res(0.);
  res(0, 0) = nx / 2.;
  res(0, 3) = (nx - 1) / 2.;
  res(1, 1) = ny / 2.;
  res(1, 3) = (ny - 1) / 2.;
  res(2, 2) = 1.;
  res(3, 3) = 1.;
  return res;
}
Vector4d Utils::make4dHomoCoordPoint(const Vector3d &p) {
  return Vector4d({p(0), p(1), p(2), 1.});
}

double Utils::computePointLineDistance(const Vector3d &v0, const Vector3d &v1, const Vector3d &v) {
  return (v0(1) - v1(1)) * v(0) + (v1(0) - v0(0)) * v(1) + v0(0) * v1(1) - v1(0) * v0(1);
}
Vector3d Utils::computeBaryCoord(const Vector3d &v0,
                                 const Vector3d &v1,
                                 const Vector3d &v2,
                                 const Vector3d &v) {
  Vector3d baryCoord;
  baryCoord(0) = computePointLineDistance(v1, v2, v) / computePointLineDistance(v1, v2, v0);
  baryCoord(1) = computePointLineDistance(v2, v0, v) / computePointLineDistance(v2, v0, v1);
  baryCoord(2) = computePointLineDistance(v0, v1, v) / computePointLineDistance(v0, v1, v2);
  return baryCoord;
}
Vector3d Utils::linearInterpolate(const Vector3d &v0,
                                  const Vector3d &v1,
                                  const Vector3d &v2,
                                  const Vector3d &baryCoord) {
  return v0 * baryCoord(0) + v1 * baryCoord(1) + v2 * baryCoord(2);
}
ColorRGB32f Utils::linearInterpolate(const ColorRGB32f &v0,
                                     const ColorRGB32f &v1,
                                     const ColorRGB32f &v2,
                                     const Vector3d &baryCoord) {
  return v0 * baryCoord(0) + v1 * baryCoord(1) + v2 * baryCoord(2);
}
