//
// Created by Jiang Kairong on 3/19/18.
//

#ifndef PROG04_INLINEBOOL_TRIMESH_H
#define PROG04_INLINEBOOL_TRIMESH_H

#include <map>
#include <array>
#include "Matrix.h"

struct HalfEdge;
struct Vertex;
struct Face;

/// Vertex structure, containing the position, normal and halfEdge informations
struct Vertex {
  Vertex(double x, double y, double z);
  explicit Vertex(const Vector3d &position);
  Vertex(const std::shared_ptr<Vector3d> &position, const std::shared_ptr<Vector3d> &normal);
  std::shared_ptr<Vector3d> position;
  std::shared_ptr<HalfEdge> halfEdge;
  int index;
  std::shared_ptr<Vector3d> normal;
};

/// Face structure, containing the position, normal and halfEdge informations
struct Face {
  std::shared_ptr<HalfEdge> halfEdge;
  std::shared_ptr<Vector3d> normal;
  std::shared_ptr<Vector3d> position;
};

/// HalfEdge structure, main structure for the half edge mesh
struct HalfEdge {
  std::shared_ptr<Face> face;
  std::shared_ptr<Vertex> startVertex;
  std::shared_ptr<HalfEdge> oppositeHalfEdge;
  std::shared_ptr<HalfEdge> nextHalfEdge;
};

/// Triangular mesh with half edge data structure.
class TriMesh {
 public:
  /// read and construct a triangular mesh from an .obj file
  /// \param inputFileName
  explicit TriMesh(std::string inputFileName);

  /// write the triangular mesh to an .obj file.
  /// \param outputFileName
  /// \return
  bool writeToObjFile(std::string outputFileName);

  /// subdivide the triangular mesh
  void subdivision();

  /// get the number of vertices
  /// \return number of vertices
  int getVertexNumber();

  /// get the number of faces
  /// \return number of faces
  int getFaceNumber();

  /// update the vertex and face normals
  void updateNormals();

  /// get the neighbor vertices of a face
  /// \param face
  /// \return a vector of pointers to the vertex neighbors
  std::vector<std::shared_ptr<Vertex>> getFaceVertices(std::shared_ptr<Face> face) const;
  /// get the face neighbors of a vertex
  /// \param vertex
  /// \return a vector of pointers to the face neighbors
  std::vector<std::shared_ptr<Face>> getVertexFaces(std::shared_ptr<Vertex> vertex) const;
  /// get the vertices
  /// \return a vector of pointers to the vertices
  const std::vector<std::shared_ptr<Vertex>> &getVertices() const;
  /// get the faces
  /// \return a vector of pointers to the faces
  const std::vector<std::shared_ptr<Face>> &getFaces() const;

  /// get the neighbor vertices of a vertex
  /// \param v
  /// \return
  std::vector<std::shared_ptr<Vertex>> getVertexVertices(const std::shared_ptr<Vertex> &v);

 private:
  void initializeHalfEdgeMesh();
  std::pair<int, int> getEdgePair(const Vector3i &face, int i);
  std::pair<int, int> getOppositeEdgePair(std::pair<int, int> e);
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Face>> faces;
  std::vector<Vector3i> faceIndices;
  std::map<std::pair<int, int>, std::shared_ptr<HalfEdge>> halfEdges;
  bool halfEdgeMeshInitialized;
  bool normalUpdated;
};

#endif //PROG04_INLINEBOOL_TRIMESH_H
