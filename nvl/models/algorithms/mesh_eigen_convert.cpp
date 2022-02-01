/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_eigen_convert.h"

#ifdef NVL_EIGEN_LOADED

namespace nvl {

template<class Mesh>
void convertMeshToEigenMesh(
        const Mesh& mesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        Size dim,
        Size numVerticesPerFace)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;

    assert(dim >= 2);

    V.resize(mesh.vertexNumber(), dim);
    F.resize(mesh.faceNumber(), numVerticesPerFace);

    int eigenVertexId = 0;
    for (VertexId vId = 0; vId < mesh.nextVertexId(); vId++){
        if (mesh.isVertexDeleted(vId))
            continue;

        const Point& point = mesh.vertexPoint(vId);

        for (Index j = 0; j < numVerticesPerFace; j++) {
            V(eigenVertexId, j) = point(j);
        }

        eigenVertexId++;
    }

    int eigenFaceId = 0;
    for (FaceId fId = 0; fId < mesh.nextFaceId(); fId++){
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        for (Index j = 0; j < face.vertexNumber(); j++) {
            assert(j < dim);
            F(eigenFaceId, j) = face.vertexId(j);
        }

        eigenFaceId++;
    }
}

template<class Mesh>
void convertMeshToEigenMesh(
        const Mesh& mesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        Size dim,
        Size numVerticesPerFace)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;

    assert(dim >= 2);

    V.resize(mesh.vertexNumber(), dim);
    F.resize(mesh.faceNumber(), numVerticesPerFace);

    birthVertex.resize(mesh.vertexNumber(), NULL_ID);
    int eigenVertexId = 0;
    for (VertexId vId = 0; vId < mesh.nextVertexId(); vId++){
        if (mesh.isVertexDeleted(vId))
            continue;

        const Point& point = mesh.vertexPoint(vId);

        for (Index j = 0; j < numVerticesPerFace; j++) {
            V(eigenVertexId, j) = point(j);
        }

        birthVertex[eigenVertexId] = vId;
        eigenVertexId++;
    }

    birthFace.resize(mesh.faceNumber(), NULL_ID);
    int eigenFaceId = 0;
    for (FaceId fId = 0; fId < mesh.nextFaceId(); fId++){
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        for (Index j = 0; j < face.vertexNumber(); j++) {
            assert(j < dim);
            F(eigenFaceId, j) = face.vertexId(j);
        }

        birthFace[eigenFaceId] = fId;
        eigenFaceId++;
    }
}

template<class Mesh>
void convertEigenMeshToMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        Mesh& mesh)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    mesh.clear();

    for (int i = 0; i < V.rows(); i++) {
        mesh.addVertex(Point(V(i, 0), V(i, 1), V(i, 2)));
    }

    for (int i = 0; i < F.rows(); i++) {
        std::vector<VertexId> vertexIds;

        for (int j = 0; j < F.cols(); j++) {
            vertexIds.push_back(F(i, j));
        }

        mesh.addFace(vertexIds);
    }
}

template<class Mesh>
void convertEigenMeshToMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        Mesh& mesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    mesh.clear();

    for (int i = 0; i < V.rows(); i++) {
        VertexId vId = mesh.addVertex(Point(V(i, 0), V(i, 1), V(i, 2)));
        birthVertex[vId] = i;
    }

    for (int i = 0; i < F.rows(); i++) {
        std::vector<VertexId> vertexIds;

        for (int j = 0; j < F.cols(); j++) {
            vertexIds.push_back(F(i, j));
        }

        VertexId fId = mesh.addFace(vertexIds);
        birthFace[fId] = i;
    }
}

}

#endif
