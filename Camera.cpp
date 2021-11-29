//
// Created by Jiang Kairong on 4/14/18.
//

#include "Camera.h"
const Vector3d &Camera::getEyePosition() const {
  return eyePosition;
}
void Camera::setEyePosition(const Vector3d &eyePosition) {
  Camera::eyePosition = eyePosition;
}
const Vector3d &Camera::getLookAtPosition() const {
  return lookAtPosition;
}
void Camera::setLookAtPosition(const Vector3d &lookUpPosition) {
  Camera::lookAtPosition = lookUpPosition;
}
const Vector3d &Camera::getUpDirection() const {
  return upDirection;
}
void Camera::setUpDirection(const Vector3d &upDirection) {
  Camera::upDirection = upDirection;
}
double Camera::getAngle() const {
  return angle;
}
void Camera::setAngle(double angle) {
  Camera::angle = angle;
}
const std::pair<int, int> &Camera::getImageSize() const {
  return imageSize;
}
void Camera::setImageSize(const std::pair<int, int> &imageSize) {
  Camera::imageSize = imageSize;
}
const std::pair<double, double> &Camera::getDepths() const {
  return depths;
}
void Camera::setDepths(const std::pair<double, double> &depths) {
  Camera::depths = depths;
}
