/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_differentiation.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>

namespace nvl {

/**
 * @brief Differential coordinates of the vertices of a mesh
 * @param mesh Mesh
 * @return Differential coordinates
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    return meshDifferentialCoordinates(mesh, vvAdj);
}

/**
 * @brief Differential coordinates of the vertices of a mesh
 * @param mesh Mesh
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Differential coordinates
 */
template<class Mesh, class R>
std::vector<Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    std::vector<Vector3<R>> dc(mesh.nextVertexId());
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        dc[vId] = meshDifferentialCoordinates(mesh, vId, vvAdj);
    }
    return dc;
}

/**
 * @brief Differential coordinates of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @return Differential coordinates
 */
template<class Mesh, class R>
Vector3<R> meshDifferentialCoordinates(
        const Mesh& mesh,
        typename Mesh::VertexId& vId)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    return meshDifferentialCoordinates(mesh, vId, vvAdj);
}

/**
 * @brief Differential coordinates of a single vertex of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @return Differential coordinates
 */
template<class Mesh, class R>
Vector3<R> meshDifferentialCoordinates(
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

    //Calculate differential coordinates
    Vector3<R> dc = currentPoint - delta;

    return dc;
}

}
