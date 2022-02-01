/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_normals.h"

#include <vector>

#include <nvl/math/normals.h>
#include <nvl/models/algorithms/mesh_adjacencies.h>

namespace nvl {

template<class Mesh>
void meshComputeFaceNormalsPlanar(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    mesh.enableFaceNormals();

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
        points[pointIndex] = mesh.vertexPoint(vId);
        ++pointIndex;
    }

    mesh.setFaceNormal(face, computeFaceNormalStandard(points, normalize));
}

template<class Mesh>
void meshComputeFaceNormalsSVDFitting(Mesh& mesh, const bool normalize)
{
    typedef typename Mesh::FaceId FaceId;

    mesh.enableFaceNormals();

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
        points[pointIndex] = mesh.vertexPoint(vId);
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

    mesh.enableFaceNormals();

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
        points[pointIndex] = mesh.vertexPoint(vId);
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

    mesh.enableFaceNormals();

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
    typedef typename Mesh::FaceNormal FaceNormal;

    Face& face = mesh.face(fId);
    FaceNormal normal(0, 0, 0);

    for (const VertexId& vId : face.vertexIds()) {
        normal += mesh.vertexNormal(vId);
    }
    normal /= face.vertexNumber();

    if (normalize) {
        normal.normalize();
    }

    mesh.setFaceNormal(fId, normal);
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

    mesh.enableVertexNormals();

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
    typedef typename Mesh::VertexNormal VertexNormal;

    Vertex& vertex = mesh.vertex(vId);
    VertexNormal normal(0, 0, 0);

    if (!vfAdj[vertex.id()].empty()) {
        for (const FaceId& fId : vfAdj[vertex.id()]) {
            normal += mesh.faceNormal(fId);
        }
        normal /= vfAdj[vertex.id()].size();

        if (normalize) {
            normal.normalize();
        }
    }

    mesh.setVertexNormal(vId, normal);
}



}
