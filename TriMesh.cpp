//
// Created by Jiang Kairong on 3/19/18.
//

#include "TriMesh.h"
#include <fstream>
#include <sstream>

TriMesh::TriMesh(std::string inputFileName) : halfEdgeMeshInitialized(false), normalUpdated(false) {
  std::ifstream fin(inputFileName);
  char c;
  fin.get(c);
  while (fin.good()) {
    double x, y, z;
    int v[3];
    switch (c) {
      case '#':fin.ignore(256, '\n');
        fin.get(c);
        break;
      case 'v':fin >> x >> y >> z;
        vertices.push_back(std::make_shared<Vertex>(x, y, z));
        vertices[vertices.size() - 1]->index = static_cast<int>(vertices.size() - 1);
        fin.get(c);
        break;
      case 'f':fin >> v[0] >> v[1] >> v[2];
        faceIndices.emplace_back(std::initializer_list<int>{v[0] - 1, v[1] - 1, v[2] - 1});
        fin.get(c);
        break;
      default:fin.get(c);
        break;
    }
  }
  initializeHalfEdgeMesh();
  fin.close();
}

bool TriMesh::writeToObjFile(std::string outputFileName) {
  std::ofstream fout(outputFileName);
  fout.precision(10);
  for (auto &vertex: vertices) {
    fout << "v " << vertex->position->operator()(0) << " " << vertex->position->operator()(1) << " "
         << vertex->position->operator()(2) << std::endl;
  }
  for (auto &face: faceIndices) {
    fout << "f " << face(0) + 1 << " " << face(1) + 1 << " " << face(2) + 1 << std::endl;
  }
  fout.close();
  return true;
}

void TriMesh::initializeHalfEdgeMesh() {
  if (halfEdgeMeshInitialized) {
    return;
  }
  halfEdges.clear();
  faces.clear();
  for (int t = 0; t < faceIndices.size(); t++) {
    faces.push_back(std::make_shared<Face>());
    for (int i = 0; i < 3; i++) {
      halfEdges[getEdgePair(faceIndices[t], i)] = std::make_shared<HalfEdge>();
    }
    for (int i = 0; i < 3; i++) {
      auto e = getEdgePair(faceIndices[t], i);
      auto halfEdge = halfEdges[e];
      halfEdge->face = faces[t];
      faces[t]->halfEdge = halfEdge;
      halfEdge->startVertex = vertices[e.first];
      vertices[e.first]->halfEdge = halfEdge;
      halfEdge->nextHalfEdge = halfEdges[getEdgePair(faceIndices[t], i + 1)];
      auto oe = getOppositeEdgePair(e);
      if (halfEdges.find(oe) != halfEdges.end()) {
        halfEdge->oppositeHalfEdge = halfEdges[oe];
        halfEdges[oe]->oppositeHalfEdge = halfEdge;
      }
    }
  }
  halfEdgeMeshInitialized = true;
  updateNormals();
}

std::pair<int, int> TriMesh::getEdgePair(const Vector3i &face, int i) {
  int u = face(i % 3);
  int v = face((i + 1) % 3);
  return std::make_pair(u, v);
}

std::pair<int, int> TriMesh::getOppositeEdgePair(std::pair<int, int> e) {
  return {e.second, e.first};
}

void TriMesh::subdivision() {
  initializeHalfEdgeMesh();
  std::vector<Vector3i> newFaceIndices;
  std::map<std::shared_ptr<HalfEdge>, int> newVertexIndices;
  auto oldVertexNumber = static_cast<int>(vertices.size());
  for (auto &face: faces) {
    auto halfEdge = face->halfEdge;
    do {
      auto u = halfEdge->startVertex;
      auto v = halfEdge->nextHalfEdge->startVertex;
      if (newVertexIndices.find(halfEdge) == newVertexIndices.end()) {
        auto w = halfEdge->nextHalfEdge->nextHalfEdge->startVertex;
        auto x = halfEdge->oppositeHalfEdge->nextHalfEdge->nextHalfEdge->startVertex;
        Vector3d newPosition =
            (*(u->position)) * 3. / 8. + (*(v->position)) * 3. / 8. + (*(w->position)) / 8.
                + (*(x->position)) / 8.;
        vertices.push_back(std::make_shared<Vertex>(newPosition));
        vertices[vertices.size() - 1]->index = static_cast<int>(vertices.size() - 1);
        newVertexIndices[halfEdge] = newVertexIndices[halfEdge->oppositeHalfEdge] =
            static_cast<int>(vertices.size() -
                1);
      }
      halfEdge = halfEdge->nextHalfEdge;
    } while (halfEdge != face->halfEdge);
    do {
      newFaceIndices.emplace_back(
          std::initializer_list<int>{newVertexIndices[halfEdge],
                                     halfEdge->nextHalfEdge->startVertex->index,
                                     newVertexIndices[halfEdge->nextHalfEdge]});
      halfEdge = halfEdge->nextHalfEdge;
    } while (halfEdge != face->halfEdge);
    newFaceIndices.emplace_back(
        std::initializer_list<int>{newVertexIndices[halfEdge],
                                   newVertexIndices[halfEdge->nextHalfEdge],
                                   newVertexIndices[halfEdge->nextHalfEdge->nextHalfEdge]});
  }
  faceIndices = newFaceIndices;
  halfEdgeMeshInitialized = false;
  normalUpdated = false;
  std::vector<Vector3d> newPositions;
//  initializeHalfEdgeMesh();
  for (int i = 0; i < oldVertexNumber; i++) {
    auto neighbors = getVertexVertices(vertices[i]);
    double beta = 3. / (8. * neighbors.size());
    if (neighbors.size() == 3) {
      beta = 3. / 16.;
    }
    newPositions.emplace_back(vertices[i]->position->operator*(1 - neighbors.size() * beta));
    for (const auto &neighbor: neighbors) {
      newPositions[i] += neighbor->position->operator*(beta);
    }
  }
  for (int i = 0; i < oldVertexNumber; i++) {
    *vertices[i]->position = newPositions[i];
  }
}

int TriMesh::getVertexNumber() {
  return static_cast<int>(vertices.size());
}

int TriMesh::getFaceNumber() {
  return static_cast<int>(faces.size());
}

std::vector<std::shared_ptr<Vertex>> TriMesh::getVertexVertices(const std::shared_ptr<Vertex> &v) {
  std::vector<std::shared_ptr<Vertex>> neighbors;
  auto halfEdge = v->halfEdge;
  do {
    neighbors.push_back(halfEdge->nextHalfEdge->startVertex);
    halfEdge = halfEdge->nextHalfEdge->nextHalfEdge->oppositeHalfEdge;
  } while (halfEdge != v->halfEdge);
  return neighbors;
}
void TriMesh::updateNormals() {
  if (!halfEdgeMeshInitialized) {
    initializeHalfEdgeMesh();
  }
  if (normalUpdated) {
    return;
  }
  for (auto &face: faces) {
    auto vertices = getFaceVertices(face);
    Vector3d ab = (*(vertices[1]->position)) - (*(vertices[0]->position));
    Vector3d ac = (*(vertices[2]->position)) - (*(vertices[0]->position));
    face->normal = std::make_shared<Vector3d>(ab.cross(ac).normalize());
    Vector3d position(0.);
    for (int i = 0; i < 3; i++) {
      position += *vertices[i]->position;
    }
    position /= 3;
    face->position = std::make_shared<Vector3d>(position);
  }
  for (auto &vertex: vertices) {
    auto faceNeighbors = getVertexFaces(vertex);
    Vector3d normalSum(0);
    for (auto &face: faceNeighbors) {
      normalSum += *face->normal;
    }
    vertex->normal = std::make_shared<Vector3d>(normalSum.normalize());
  }
  normalUpdated = true;
}
std::vector<std::shared_ptr<Vertex>> TriMesh::getFaceVertices(std::shared_ptr<Face> face) const {
  std::vector<std::shared_ptr<Vertex>> res;
  res.push_back(face->halfEdge->startVertex);
  res.push_back(face->halfEdge->nextHalfEdge->startVertex);
  res.push_back(face->halfEdge->nextHalfEdge->nextHalfEdge->startVertex);
  return res;
}
std::vector<std::shared_ptr<Face>> TriMesh::getVertexFaces(std::shared_ptr<Vertex> vertex) const {
  std::vector<std::shared_ptr<Face>> res;
  auto halfEdge = vertex->halfEdge;
  do {
    res.push_back(halfEdge->face);
    halfEdge = halfEdge->nextHalfEdge->nextHalfEdge->oppositeHalfEdge;
  } while (halfEdge != vertex->halfEdge);
  return res;
}
const std::vector<std::shared_ptr<Vertex>> &TriMesh::getVertices() const {
  return vertices;
}
const std::vector<std::shared_ptr<Face>> &TriMesh::getFaces() const {
  return faces;
}

Vertex::Vertex(double x, double y, double z) {
  position = std::make_shared<Vector3d>(std::initializer_list<double>{x, y, z});
  halfEdge = nullptr;
  index = -1;
}

Vertex::Vertex(const Vector3d &position) {
  Vertex::position = std::make_shared<Vector3d>(position);
  halfEdge = nullptr;
  index = -1;
}
Vertex::Vertex(const std::shared_ptr<Vector3d> &position, const std::shared_ptr<Vector3d> &normal) : position(
    position), normal(normal) {}
