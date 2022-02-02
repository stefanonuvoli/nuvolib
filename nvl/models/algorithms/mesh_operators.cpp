/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_operators.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>

#include <nvl/math/cotangent_weights.h>
#include <nvl/math/triangle.h>

namespace nvl {

/**
 * @brief Laplace operator of the vertices of a mesh
 * @param mesh Mesh
 * @return Laplace operator
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshLaplaceOperator(
        const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    return meshLaplaceOperator(mesh, vvAdj);
}

/**
 * @brief Laplace operator of the vertices of a mesh
 * @param mesh Mesh
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Laplace operator
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshLaplaceOperator(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    std::vector<Vector3<R>> lo(mesh.nextVertexId());
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        lo[vId] = meshLaplaceOperator(mesh, vId, vvAdj);
    }
    return lo;
}

/**
 * @brief Laplace operator of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @return Laplace operator
 */
template<class Mesh, class R>
Vector3<R> meshLaplaceOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    return meshLaplaceOperator(mesh, vId, vvAdj);
}

/**
 * @brief Laplace operator of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Laplace operator
 */
template<class Mesh, class R>
Vector3<R> meshLaplaceOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    Point currentPoint = mesh.vertexPoint(vId);

    //Calculate delta
    Vector3<R> delta = Vector3<R>::Zero();
    const std::vector<VertexId>& neighbors = vvAdj[vId];
    for(const VertexId& neighborId : neighbors) {
        const Point& neighborPoint = mesh.vertexPoint(neighborId);
        delta += neighborPoint;
    }
    delta /= neighbors.size();

    //Calculate Laplace operator
    Vector3<R> lo = delta - currentPoint;

    return lo;
}

/**
 * @brief Laplace-Beltrami operator of the vertices of a mesh
 * @param mesh Mesh
 * @return Laplace-Beltrami operator
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshLaplaceBeltramiOperator(
        const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    return meshLaplaceBeltramiOperator(mesh, vvAdj, vfAdj);
}

/**
 * @brief Laplace-Beltrami operator of the vertices of a mesh
 * @param mesh Mesh
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Laplace-Beltrami operator
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    std::vector<Vector3<R>> lbo(mesh.nextVertexId());

    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        lbo[vId] = meshLaplaceBeltramiOperator(mesh, vId, vvAdj, vfAdj);
    }
    return lbo;
}

/**
 * @brief Laplace-Beltrami operator of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @return Laplace-Beltrami operator
 */
template<class Mesh, class R>
Vector3<R> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    return meshLaplaceBeltramiOperator(mesh, vId, vvAdj, vfAdj);
}

/**
 * @brief Laplace-Beltrami operator of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Laplace-Beltrami operator
 */
template<class Mesh, class R>
Vector3<R> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::Scalar Scalar;

    Scalar areaSum = 0.0;

    Vector3<R> lbo;

    for (const VertexId& adjVId : vvAdj[vId]) {
        assert(adjVId != NULL_ID);

        std::vector<FaceId> adjFaces1;
        for (const FaceId& adjFId : vfAdj[vId]) {
            adjFaces1.push_back(adjFId);
        }
        std::vector<FaceId> adjFaces2;
        for (const FaceId& adjFId : vfAdj[adjVId]) {
            adjFaces2.push_back(adjFId);
        }
        std::vector<FaceId> adjFaces;
        std::set_intersection(adjFaces1.begin(), adjFaces1.end(), adjFaces2.begin(), adjFaces2.end(), std::back_inserter(adjFaces));

        Scalar weight;
        if (adjFaces.size() == 2 && adjFaces[0] != adjFaces[1]) {
            VertexId prevVId = NULL_ID;
            VertexId nextVId = NULL_ID;

            const Face& face1 = mesh.face(adjFaces[0]);
            for (Index pos = 0; pos < face1.vertexNumber(); ++pos) {
                if (face1.vertexId(pos) != vId && face1.vertexId(pos) != adjVId) {
                    prevVId = face1.vertexId(pos);
                    break;
                }
            }

            const Face& face2 = mesh.face(adjFaces[1]);
            for (Index pos = 0; pos < face2.vertexNumber(); ++pos) {
                if (face2.vertexId(pos) != vId && face2.vertexId(pos) != adjVId) {
                    nextVId = face2.vertexId(pos);
                    break;
                }
            }

            assert(prevVId != NULL_ID);
            assert(nextVId != NULL_ID);

            weight = cotangentWeight(
                    mesh.vertexPoint(vId),
                    mesh.vertexPoint(adjVId),
                    mesh.vertexPoint(prevVId),
                    mesh.vertexPoint(nextVId));

            Triangle<Point> triangle1(mesh.vertexPoint(vId), mesh.vertexPoint(adjVId), mesh.vertexPoint(prevVId));
            Triangle<Point> triangle2(mesh.vertexPoint(vId), mesh.vertexPoint(nextVId), mesh.vertexPoint(adjVId));
            areaSum += (triangle1.area() / 3.0 + triangle2.area() / 3.0) * 0.5;
        }
        else {
            weight = static_cast<Scalar>(1.0);

            if (adjFaces.size() == 1) {
                VertexId newVId = NULL_ID;
                const Face& face = mesh.face(adjFaces[0]);
                for (Index pos = 0; pos < face.vertexNumber(); ++pos) {
                    if (face.vertexId(pos) != vId && face.vertexId(pos) != adjVId) {
                        newVId = face.vertexId(pos);
                        break;
                    }
                }
                assert(newVId != NULL_ID);

                Triangle<Point> triangle1(
                    mesh.vertexPoint(vId),
                    mesh.vertexPoint(adjVId),
                    mesh.vertexPoint(newVId));
                areaSum += (triangle1.area() / 3.0) * 0.5;
            }
        }

        lbo += weight * (mesh.vertexPoint(adjVId) - mesh.vertexPoint(vId));
    }

    lbo /= 2.0 * areaSum;

    return lbo;
}

}
