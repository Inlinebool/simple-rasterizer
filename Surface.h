//
// Created by Jiang Kairong on 4/11/18.
//

#ifndef PROG05_SURFACE_H
#define PROG05_SURFACE_H

#include "TriMesh.h"
#include "Color.h"
/// Color settings of a surface
struct SurfaceColorSettings {
  SurfaceColorSettings(const ColorRGB32f &kAmbient,
                       const ColorRGB32f &kDiffuse,
                       const ColorRGB32f &kSpecular,
                       double kExponent);
  ColorRGB32f kAmbient, kDiffuse, kSpecular;
  double phongExponent;
};
/// Surface class
class Surface {
 public:
  /// Read a mesh file and construct a surface object
  /// \param inputFileName input mesh file name
  /// \param kAmbient ambient parameter
  /// \param kDiffuse diffuse parameter
  /// \param kSpecular specular parameter
  /// \param phongExponent phone exponent
  Surface(const std::string &inputFileName,
          const ColorRGB32f &kAmbient,
          const ColorRGB32f &kDiffuse,
          const ColorRGB32f &kSpecular,
          double phongExponent);
  /// get the trigonal mesh
  /// \return reference to the trigonal mesh
  const TriMesh &getMesh() const;
  /// get the color parameters
  /// \return pointer the parameter struct
  const std::shared_ptr<SurfaceColorSettings> &getColorSettings() const;
  /// set the color parameters
  /// \param colorSettings
  void setColorSettings(const std::shared_ptr<SurfaceColorSettings> &colorSettings);
 private:
  TriMesh mesh;
  std::shared_ptr<SurfaceColorSettings> colorSettings;
};

#endif //PROG05_SURFACE_H
