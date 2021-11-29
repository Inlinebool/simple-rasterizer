//
// Created by Jiang Kairong on 4/9/18.
//

#ifndef PROG05_MATRIXUTILS_H
#define PROG05_MATRIXUTILS_H

#include "Matrix.h"
#include "TriMesh.h"
#include "Color.h"

/// Utility class
class Utils {
 public:
  /// perform homogeneous divide on a 4d vector
  /// \param rhs
  /// \return resulted 3d vector
  static Vector3d homoDivideVector4d(const Vector4d &rhs);
  /// construct the transformation matrix to change coordination system
  /// \param u x axis of the new coord system
  /// \param v y axis of the new coord system
  /// \param w w axis of the new coord system
  /// \return corresponding transformation matrix
  static Matrix4d make3dChangeCoordSysMatrix(const Vector3d &u, const Vector3d &v, const Vector3d &w);
  /// construct the transformation matrix to translate
  /// \param t translation vector
  /// \return corresponding transformation matrix
  static Matrix4d make3dTranslateMatrix(const Vector3d &t);
  /// construct the perspective projection transformation matrix
  /// \param n near plane
  /// \param f far plane
  /// \param b bottom plane
  /// \param t top plane
  /// \param l left plane
  /// \param r right plane
  /// \return corresponding transformation matrix
  static Matrix4d makePerspectiveProjectionMatrix(double n, double f, double b, double t, double l, double r);
  /// construct the viewport transformation matrix
  /// \param nx width of the image resolution
  /// \param ny height of the image resolution
  /// \return corresponding transformation matrix
  static Matrix4d makeViewPortTransformMatrix(int nx, int ny);
  /// get the 4d homogeneous point
  /// \param p original point
  /// \return corresponding 4d homogeneous point
  static Vector4d make4dHomoCoordPoint(const Vector3d &p);
  /// compute the barycentric coordinate for a point relative to a triangle
  /// \param v0 vertices of the triangle
  /// \param v1 vertices of the triangle
  /// \param v2 vertices of the triangle
  /// \param v underlying point
  /// \return the barycentric coordinate
  static Vector3d computeBaryCoord(const Vector3d &v0,
                                   const Vector3d &v1,
                                   const Vector3d &v2,
                                   const Vector3d &v);
  /// compute the distance between a point and a line, used to compute barycentric coordinates
  /// \param v0 points on the line
  /// \param v1 points on the line
  /// \param v underlying point
  /// \return distance between the point and the line
  static double computePointLineDistance(const Vector3d &v0, const Vector3d &v1, const Vector3d &v);
  /// linear interpolation on a triangle
  /// \param v0 vertices of the triangle
  /// \param v1 vertices of the triangle
  /// \param v2 vertices of the triangle
  /// \param baryCoord barycentric coordinate of the point
  /// \return interpolation result
  static Vector3d linearInterpolate(const Vector3d &v0,
                                    const Vector3d &v1,
                                    const Vector3d &v2,
                                    const Vector3d &baryCoord);
  /// linear interpolation on a triangle
  /// \param v0 vertices of the triangle
  /// \param v1 vertices of the triangle
  /// \param v2 vertices of the triangle
  /// \param baryCoord barycentric coordinate of the point
  /// \return interpolation result
  static ColorRGB32f linearInterpolate(const ColorRGB32f &v0,
                                       const ColorRGB32f &v1,
                                       const ColorRGB32f &v2,
                                       const Vector3d &baryCoord);
};

#endif //PROG05_MATRIXUTILS_H
