//
// Created by Jiang Kairong on 4/14/18.
//

#ifndef PROG05_RENDERER_H
#define PROG05_RENDERER_H

#include "Scene.h"
#include "Image.h"
struct Fragment{
  int i, j;
  Vector3d position;
  Vector3d normal;
  ColorRGB32f flatColor;
  ColorRGB32f gouraudColor;
  ColorRGB32f phongColor;
  std::shared_ptr<SurfaceColorSettings> colorSettings;
};

/// Rasterizing Renderer
class Renderer {
 public:
  enum ShadingPolicy {
    FLAT_SHADING,
    GOURAUD_SHADING,
    PHONG_SHADING
  };
  /// Construct the renderer using the input scene file
  /// \param inputSceneFileName
  explicit Renderer(const std::string &inputSceneFileName);
  /// Render the scene
  /// \return the rendered 8-bit image
  std::shared_ptr<Image8i> renderForDisplay();
  /// get the shading method
  /// \return one of the shading method defined in the ShadingPolicy enum
  int getShadingPolicy() const;
  /// set the shading method
  /// \param shadingPolicy
  void setShadingPolicy(int shadingPolicy);
 private:
  void prepareMatrices();
  void processVertices();
  void rasterize();
  void fragmentShading();
  ColorRGB32f shading(const Vector3d &position,
                        const Vector3d &normal,
                        const std::vector<std::shared_ptr<LightSource>> &lights,
                        const SurfaceColorSettings &colorSettings);
  std::shared_ptr<Scene> scene;
  Matrix4d m;
  std::map<std::shared_ptr<Vertex>, std::shared_ptr<Vector4d>> processedVertexPositions;
  std::map<std::shared_ptr<Vertex>, std::shared_ptr<Vector3d>> dividedVertexPositions;
  std::map<std::shared_ptr<Vertex>, std::shared_ptr<ColorRGB32f>> vertexColors;
  std::map<std::shared_ptr<Face>, std::shared_ptr<ColorRGB32f>> faceColors;
  std::map<std::pair<int, int>, std::shared_ptr<Fragment>> fragments;
  std::map<std::pair<int, int>, double> zBuffer;
  int shadingPolicy;
  std::shared_ptr<Image32f> frameBuffer;
};

#endif //PROG05_RENDERER_H
