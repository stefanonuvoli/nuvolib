#include "mesh_normals.h"

#include <vector>

#include <nvl/math/normals.h>
#include <nvl/models/mesh_adjacencies.h>

namespace nvl {

template<class Mesh>
void meshComputeFaceNormalsPlanar(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    #pragma omp parallel for
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (!mesh.isFaceDeleted(fId)) {
            meshComputeFaceNormalPlanar(mesh, fId, normalize);
        }
    }
}

template<class Mesh>
void meshComputeFaceNormalPlanar(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    Face& face = mesh.face(fId);

    std::vector<Point> points(face.vertexNumber());

    Index pointIndex = 0;
    for (const VertexId& vId : face.vertexIds()) {
        points[pointIndex] = mesh.vertex(vId).point();
        ++pointIndex;
    }

    mesh.setFaceNormal(face, computeFaceNormalStandard(points, normalize));
}

template<class Mesh>
void meshComputeFaceNormalsSVDFitting(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    #pragma omp parallel for
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (!mesh.isFaceDeleted(fId)) {
            meshComputeFaceNormalSVDFitting(mesh, fId, normalize);
        }
    }
}

template<class Mesh>
void meshComputeFaceNormalSVDFitting(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    Face& face = mesh.face(fId);

    std::vector<Point> points(face.vertexNumber());

    Index pointIndex = 0;
    for (const VertexId& vId : face.vertexIds()) {
        points[pointIndex] = mesh.vertex(vId).point();
        ++pointIndex;
    }

    if (face.vertexNumber() == 3) {
        mesh.setFaceNormal(face, faceNormalPlanar(points, normalize));
    }
    else if (face.vertexNumber() > 3) {
        mesh.setFaceNormal(face, faceNormalSVDFitting(points));
    }
}

template<class Mesh>
void meshComputeFaceNormalsCovarianceFitting(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    #pragma omp parallel for
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (!mesh.isFaceDeleted(fId)) {
            meshComputeFaceNormalsCovarianceFitting(mesh, fId, normalize);
        }
    }
}

template<class Mesh>
void meshComputeFaceNormalsCovarianceFitting(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    Face& face = mesh.face(fId);
    std::vector<Point> points(face.vertexNumber());

    Index pointIndex = 0;
    for (const VertexId& vId : face.vertexIds()) {
        points[pointIndex] = mesh.vertex(vId).point();
        ++pointIndex;
    }

    if (face.vertexNumber() == 3) {
        mesh.setFaceNormal(face, faceNormalPlanar(points, normalize));
    }
    else if (face.vertexNumber() > 3) {
        mesh.setFaceNormal(face, faceNormalCovarianceFitting(points));
    }
}

template<class Mesh>
void meshComputeFaceNormalsFromVertexNormals(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    #pragma omp parallel for
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (!mesh.isFaceDeleted(fId)) {
            meshComputeFaceNormalFromVertexNormals(mesh, fId, normalize);
        }
    }
}

template<class Mesh>
void meshComputeFaceNormalFromVertexNormals(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Face Face;

    Face& face = mesh.face(fId);
    Vector3d normal(0, 0, 0);

    for (const VertexId& vId : face.vertexIds()) {
        normal += mesh.vertexNormal(vId);
    }
    normal /= face.vertexNumber();

    if (normalize) {
        normal.normalize();
    }

    face.setNormal(normal);
}

template<class Mesh>
void meshComputeVertexNormalsFromFaceNormals(Mesh& mesh, const bool normalize)
{
    return meshComputeVertexNormalsFromFaceNormals(mesh, meshVertexFaceAdjacencies(mesh), normalize);
}

template<class Mesh>
void meshComputeVertexNormalFromFaceNormals(Mesh& mesh, typename Mesh::FaceId& vId, const bool normalize)
{
    return meshComputeVertexNormalsFromFaceNormals(mesh, vId, meshVertexFaceAdjacencies(mesh), normalize);
}

template<class Mesh>
void meshComputeVertexNormalsFromFaceNormals(
        Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vfAdj,
        bool normalize)
{
    typedef typename Mesh::VertexId VertexId;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            meshComputeVertexNormalFromFaceNormals(mesh, vId, vfAdj, normalize);
        }
    }
}

template<class Mesh>
void meshComputeVertexNormalFromFaceNormals(
        Mesh& mesh,
        typename Mesh::FaceId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vfAdj,
        bool normalize)
{
    typedef typename Mesh::VertexId FaceId;
    typedef typename Mesh::Vertex Vertex;

    Vertex& vertex = mesh.vertex(vId);
    Vector3d normal(0, 0, 0);

    if (!vfAdj[vertex.id()].empty()) {
        for (const FaceId& fId : vfAdj[vertex.id()]) {
            normal += mesh.faceNormal(fId);
        }
        normal /= vfAdj[vertex.id()].size();

        if (normalize) {
            normal.normalize();
        }
    }

    vertex.setNormal(normal);
}



}
