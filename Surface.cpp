//
// Created by Jiang Kairong on 4/11/18.
//

#include "Surface.h"
Surface::Surface(const std::string &inputFileName,
                 const ColorRGB32f &kAmbient,
                 const ColorRGB32f &kDiffuse,
                 const ColorRGB32f &kSpecular,
                 double phongExponent)
    : mesh(inputFileName), colorSettings(std::make_shared<SurfaceColorSettings>(kAmbient, kDiffuse, kSpecular, phongExponent)) {
}
const TriMesh &Surface::getMesh() const {
  return mesh;
}
const std::shared_ptr<SurfaceColorSettings> &Surface::getColorSettings() const {
  return colorSettings;
}
void Surface::setColorSettings(const std::shared_ptr<SurfaceColorSettings> &colorSettings) {
  Surface::colorSettings = colorSettings;
}
SurfaceColorSettings::SurfaceColorSettings(const ColorRGB32f &colorAmbient,
                                           const ColorRGB32f &colorDiffuse,
                                           const ColorRGB32f &colorSpecular,
                                           double kExponent)
    : kAmbient(colorAmbient),
      kDiffuse(colorDiffuse),
      kSpecular(colorSpecular),
      phongExponent(kExponent) {}
