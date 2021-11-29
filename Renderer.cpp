//
// Created by Jiang Kairong on 4/14/18.
//

#include "Renderer.h"
#include "Utils.h"
#include <cmath>
#include <algorithm>
#include <iostream>
void Renderer::prepareMatrices() {
  auto camera = scene->getMainCamera();
  Vector3d lookDirection = camera.getLookAtPosition() - camera.getEyePosition();
  Vector3d u = lookDirection.cross(camera.getUpDirection()).normalize();
  Vector3d v = u.cross(lookDirection).normalize();
  Vector3d w = u.cross(v).normalize();
  Matrix4d mCam = Utils::make3dChangeCoordSysMatrix(u, v, w)
      * Utils::make3dTranslateMatrix(camera.getEyePosition());
//  mCam.print(std::cout);
  double n = camera.getDepths().first;
  double f = camera.getDepths().second;
  double t = std::tan(camera.getAngle() * M_PI / 360.) * std::abs(n);
  double r = t * camera.getImageSize().first / camera.getImageSize().second;
  double b = -t;
  double l = -r;
  Matrix4d mPer = Utils::makePerspectiveProjectionMatrix(n, f, b, t, l, r);
//  mPer.print(std::cout);
  int nx = camera.getImageSize().first;
  int ny = camera.getImageSize().second;
  Matrix4d mVp = Utils::makeViewPortTransformMatrix(nx, ny);
//  mVp.print(std::cout);
  m = mVp * mPer * mCam;
//  m.print(std::cout);
}
void Renderer::processVertices() {
  for (auto &object: scene->getObjects()) {
    for (auto &vertex: object->getMesh().getVertices()) {
      processedVertexPositions[vertex] =
          std::make_shared<Vector4d>(m * Utils::make4dHomoCoordPoint(*vertex->position));
      if (shadingPolicy == GOURAUD_SHADING) {
        vertexColors[vertex] =
            std::make_shared<ColorRGB32f>(shading(*vertex->position,
                                                  *vertex->normal,
                                                  scene->getLightSources(),
                                                  *object->getColorSettings()));
      }
    }
    if (shadingPolicy == FLAT_SHADING) {
      for (auto &face: object->getMesh().getFaces()) {
        faceColors[face] =
            std::make_shared<ColorRGB32f>(shading(*face->position,
                                                  *face->normal,
                                                  scene->getLightSources(),
                                                  *object->getColorSettings()));
      }
    }
  }
}
ColorRGB32f Renderer::shading(const Vector3d &position,
                              const Vector3d &normal,
                              const std::vector<std::shared_ptr<LightSource>> &lights,
                              const SurfaceColorSettings &colorSettings) {
  ColorRGB32f result(0.f);
  result += colorSettings.kAmbient.cwiseProduct(ColorRGB32f({0.5f, 0.5f, 0.5f}));
  for (auto &light: lights) {
    Vector3d lightDirection = (light->getPosition() - position).normalize();
    double diffuseAngle = normal.dot(lightDirection);
    result += colorSettings.kDiffuse.cwiseProduct(light->getIntensity()) * diffuseAngle;
    Vector3d cameraDirection = (scene->getMainCamera().getEyePosition() - position).normalize();
    Vector3d h = (lightDirection + cameraDirection).normalize();
    double specularAngle = normal.dot(h);
    result += colorSettings.kSpecular.cwiseProduct(light->getIntensity())
        * std::pow(specularAngle, colorSettings.phongExponent);
  }
  return result;
}
void Renderer::rasterize() {
  for (auto &processedVertexPosition : processedVertexPositions) {
    dividedVertexPositions[processedVertexPosition.first] =
        std::make_shared<Vector3d>(Utils::homoDivideVector4d(*processedVertexPosition.second));
  }
  for (auto &object: scene->getObjects()) {
    auto imageSize = scene->getMainCamera().getImageSize();
    for (auto &face: object->getMesh().getFaces()) {
      auto vertexNeighbors = object->getMesh().getFaceVertices(face);
      auto v0 = dividedVertexPositions[vertexNeighbors[0]];
      auto v1 = dividedVertexPositions[vertexNeighbors[1]];
      auto v2 = dividedVertexPositions[vertexNeighbors[2]];
      auto xMin = static_cast<int>(std::floor(std::min({(*v0)(0), (*v1)(0), (*v2)(0)})));
      auto xMax = static_cast<int>(std::ceil(std::max({(*v0)(0), (*v1)(0), (*v2)(0)})));
      auto yMin = static_cast<int>(std::floor(std::min({(*v0)(1), (*v1)(1), (*v2)(1)})));
      auto yMax = static_cast<int>(std::ceil(std::max({(*v0)(1), (*v1)(1), (*v2)(1)})));
      if (xMin < 0 || xMax >= imageSize.first || yMin < 0 || yMin >= imageSize.second) {
        continue;
      }
      for (int i = xMin; i < xMax; i++) {
        for (int j = yMin; j < yMax; j++) {
          Vector3d baryCoord = Utils::computeBaryCoord(*v0, *v1, *v2, Vector3d({(double) i, (double) j, 0.}));
          bool inside = true;
          for (int i = 0; i < 3; i++) {
            if (baryCoord(i) < -0.01 || baryCoord(1) > 1.01) {
              inside = false;
            }
          }
          if (inside) {
            auto position = Utils::linearInterpolate(*v0, *v1, *v2, baryCoord);
            auto pixel = std::make_pair(i, j);
            if (zBuffer.find(pixel) != zBuffer.end() && zBuffer[pixel] > position(2)) {
              continue;
            }
            auto fragment = std::make_shared<Fragment>();
            fragment->i = imageSize.second - j;
            fragment->j = i;
            if (shadingPolicy == PHONG_SHADING) {
              fragment->position = Utils::linearInterpolate(*vertexNeighbors[0]->position,
                                                            *vertexNeighbors[1]->position,
                                                            *vertexNeighbors[2]->position,
                                                            baryCoord);
              fragment->normal = Utils::linearInterpolate(*vertexNeighbors[0]->normal,
                                                          *vertexNeighbors[1]->normal,
                                                          *vertexNeighbors[2]->normal,
                                                          baryCoord).normalize();
              fragment->colorSettings = object->getColorSettings();
            }
            if (shadingPolicy == FLAT_SHADING) {
              fragment->flatColor = *faceColors[face];
            }
            if (shadingPolicy == GOURAUD_SHADING) {
              fragment->gouraudColor = Utils::linearInterpolate(*vertexColors[vertexNeighbors[0]],
                                                                *vertexColors[vertexNeighbors[1]],
                                                                *vertexColors[vertexNeighbors[2]],
                                                                baryCoord);
            }
            fragments[pixel] = fragment;
            zBuffer[pixel] = position(2);
          }
        }
      }
    }
  }
}
Renderer::Renderer(const std::string &inputSceneFileName) {
  scene = std::make_shared<Scene>(inputSceneFileName);
  shadingPolicy = FLAT_SHADING;
  frameBuffer = std::make_shared<Image32f>();
}
int Renderer::getShadingPolicy() const {
  return shadingPolicy;
}
void Renderer::setShadingPolicy(int shadingPolicy) {
  Renderer::shadingPolicy = shadingPolicy;
}
std::shared_ptr<Image8i> Renderer::renderForDisplay() {
  std::cout << "Rendering using ";
  switch (shadingPolicy) {
    case GOURAUD_SHADING:std::cout << "Gourand shading." << std::endl;
      break;
    case PHONG_SHADING:std::cout << "Phong shading." << std::endl;
      break;
    case FLAT_SHADING:
    default:std::cout << "flat shading." << std::endl;
      break;
  }
  vertexColors.clear();
  faceColors.clear();
  fragments.clear();
  zBuffer.clear();
  prepareMatrices();
  processVertices();
  rasterize();
  fragmentShading();
  return ImageUtils::convertFloatImage2Int(*frameBuffer);
}
void Renderer::fragmentShading() {
  auto imageSize = scene->getMainCamera().getImageSize();
  frameBuffer->resize(static_cast<unsigned long>(imageSize.second),
                      static_cast<unsigned long>(imageSize.first), ColorRGB32f(0.f));
  ImageUtils::clearImage(*frameBuffer);
  for (auto &fragmentPair: fragments) {
    auto fragment = fragmentPair.second;
    switch (shadingPolicy) {
      case GOURAUD_SHADING:(*frameBuffer)(fragment->i, fragment->j) = fragment->gouraudColor;
        break;
      case PHONG_SHADING:
        (*frameBuffer)(fragment->i, fragment->j) =
            shading(fragment->position,
                    fragment->normal,
                    scene->getLightSources(),
                    *fragment->colorSettings);
        break;
      case FLAT_SHADING:
      default:(*frameBuffer)(fragment->i, fragment->j) = fragment->flatColor;
        break;
    }
  }
}

