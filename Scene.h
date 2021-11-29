//
// Created by Jiang Kairong on 3/10/18.
//

#ifndef PROG03_INLINEBOOL_SCENE_H
#define PROG03_INLINEBOOL_SCENE_H

#include <vector>
#include "Surface.h"
#include "Camera.h"
#include "LightSource.h"

/// Scene class
class Scene {
 public:
  /// Read scene file and construct the scene
  /// \param sceneFileName input file name of the scene
  explicit Scene(const std::string &sceneFileName);

  /// get the objects in the scene
  /// \return vector of pointers to the objects
  const std::vector<std::shared_ptr<Surface>> &getObjects() const;

  /// set the objects in the scene
  /// \param objects
  void setObjects(const std::vector<std::shared_ptr<Surface>> &objects);

  /// get the camera in the scene
  /// \return reference to the camera object
  const Camera &getMainCamera() const;

  /// set the camera in the scene
  /// \param mainCamera
  void setMainCamera(const Camera &mainCamera);

  /// get the light sources in the scene
  /// \return vector of pointers to the light sources
  const std::vector<std::shared_ptr<LightSource>> &getLightSources() const;

  /// set the light sources in the scene
  /// \param lightSources
  void setLightSources(const std::vector<std::shared_ptr<LightSource>> &lightSources);

 private:
  std::vector<std::shared_ptr<Surface>> objects;
  Camera mainCamera;
  std::vector<std::shared_ptr<LightSource>> lightSources;
};

#endif //PROG03_INLINEBOOL_SCENE_H
