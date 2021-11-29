//
// Created by Jiang Kairong on 3/10/18.
//

#include "LightSource.h"

LightSource::LightSource(const Vector3d &position, const ColorRGB32f &intensity) : position(position),
                                                                                   intensity(intensity) {}

const Vector3d &LightSource::getPosition() const {
  return position;
}

void LightSource::setPosition(const Vector3d &position) {
  LightSource::position = position;
}

const ColorRGB32f &LightSource::getIntensity() const {
  return intensity;
}

void LightSource::setIntensity(const ColorRGB32f &intensity) {
  LightSource::intensity = intensity;
}