//
// Created by Jiang Kairong on 4/14/18.
//

#ifndef PROG05_CAMERA_H
#define PROG05_CAMERA_H

#include "Matrix.h"

/// Camera class
class Camera {
 public:
  /// get the eye position
  /// \return
  const Vector3d &getEyePosition() const;
  /// set the eye position
  /// \param eyePosition
  void setEyePosition(const Vector3d &eyePosition);
  /// get the look at position
  /// \return
  const Vector3d &getLookAtPosition() const;
  /// set the look at position
  /// \param lookUpPosition
  void setLookAtPosition(const Vector3d &lookUpPosition);
  /// get the up direction
  /// \return
  const Vector3d &getUpDirection() const;
  /// set the up direction
  /// \param upDirection
  void setUpDirection(const Vector3d &upDirection);
  /// get the vertical angle
  /// \return
  double getAngle() const;
  /// set the vertical angle
  /// \param angle
  void setAngle(double angle);
  /// get the image resolution
  /// \return
  const std::pair<int, int> &getImageSize() const;
  /// set the image resolution
  /// \param imageSize
  void setImageSize(const std::pair<int, int> &imageSize);
  /// get the near and far plane distances
  /// \return
  const std::pair<double, double> &getDepths() const;
  /// set the near and far plane distances
  /// \param depths
  void setDepths(const std::pair<double, double> &depths);
 private:
  Vector3d eyePosition, lookAtPosition, upDirection;
  double angle;
  std::pair<int, int> imageSize;
  std::pair<double, double> depths;
};

#endif //PROG05_CAMERA_H
