//
// Created by Jiang Kairong on 3/10/18.
//

#ifndef PROG03_INLINEBOOL_LIGHTSOURCE_H
#define PROG03_INLINEBOOL_LIGHTSOURCE_H

#include "Color.h"

/// Light source class
class LightSource {
 public:
  /// Construct a light source using the position and color
  /// \param position
  /// \param intensity color of the light source
  LightSource(const Vector3d &position,
              const ColorRGB32f &intensity);

  /// get the light source position
  /// \return
  const Vector3d &getPosition() const;

  /// set the light source position
  /// \param position
  void setPosition(const Vector3d &position);

  /// get the light source color
  /// \return
  const ColorRGB32f &getIntensity() const;

  /// set the light source colode
  /// \param intensity
  void setIntensity(const ColorRGB32f &intensity);

 private:
  Vector3d position;
  ColorRGB32f intensity;
};

#endif //PROG03_INLINEBOOL_LIGHTSOURCE_H
