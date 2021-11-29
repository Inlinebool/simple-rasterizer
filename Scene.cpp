//
// Created by Jiang Kairong on 3/10/18.
//

#include <fstream>
#include "Scene.h"

Scene::Scene(const std::string &sceneFileName) : objects(), mainCamera(), lightSources() {
  std::string path = sceneFileName.substr(0, sceneFileName.find_last_of("/\\") + 1);
  std::ifstream ifs;
  ifs.open(sceneFileName.data(), std::ifstream::in);
  std::string token;
  ifs >> token;
  while (ifs.good()) {
    if (token == "e") {
      double x, y, z;
      ifs >> x >> y >> z;
      mainCamera.setEyePosition(Vector3d({x, y, z}));
    } else if (token == "l") {
      double x, y, z;
      ifs >> x >> y >> z;
      mainCamera.setLookAtPosition(Vector3d({x, y, z}));
    } else if (token == "u") {
      double x, y, z;
      ifs >> x >> y >> z;
      mainCamera.setUpDirection(Vector3d({x, y, z}));
    } else if (token == "f") {
      double angle;
      ifs >> angle;
      mainCamera.setAngle(angle);
    } else if (token == "i") {
      int width, height;
      ifs >> width >> height;
      mainCamera.setImageSize(std::pair<int, int>({width, height}));
    } else if (token == "d") {
      double near, far;
      ifs >> near >> far;
      mainCamera.setDepths(std::pair<double, double>({near, far}));
    } else if (token == "L") {
      double x, y, z;
      ifs >> x >> y >> z;
      Vector3d position({x, y, z});
      float r, g, b;
      ifs >> r >> g >> b;
      ColorRGB32f intensity({r, g, b});
      lightSources.push_back(std::make_shared<LightSource>(position, intensity));
    } else if (token == "M") {
      std::string inputFileName;
      ifs >> inputFileName;
      inputFileName = path + inputFileName;
      float r, g, b;
      ifs >> r >> g >> b;
      ColorRGB32f colorAmbient({r, g, b});
      ifs >> r >> g >> b;
      ColorRGB32f colorDiffuse({r, g, b});
      ifs >> r >> g >> b;
      ColorRGB32f colorSpecular({r, g, b});
      double phongExponent;
      ifs >> phongExponent;
      objects.push_back(std::make_shared<Surface>(inputFileName,
                                                  colorAmbient,
                                                  colorDiffuse,
                                                  colorSpecular,
                                                  phongExponent));
    }
    ifs >> token;
  }
}

const std::vector<std::shared_ptr<Surface>> &Scene::getObjects() const {
  return objects;
}

void Scene::setObjects(const std::vector<std::shared_ptr<Surface>> &objects) {
  Scene::objects = objects;
}

const Camera &Scene::getMainCamera() const {
  return mainCamera;
}

void Scene::setMainCamera(const Camera &mainCamera) {
  Scene::mainCamera = mainCamera;
}

const std::vector<std::shared_ptr<LightSource>> &Scene::getLightSources() const {
  return lightSources;
}

void Scene::setLightSources(const std::vector<std::shared_ptr<LightSource>> &lightSources) {
  Scene::lightSources = lightSources;
}
