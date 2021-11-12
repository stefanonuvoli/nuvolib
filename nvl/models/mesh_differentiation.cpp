#include "mesh_differentiation.h"

#include <nvl/math/normals.h>
#include <nvl/models/mesh_adjacencies.h>

namespace nvl {


template<class Mesh, class R>
std::vector<nvl::Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = nvl::meshVertexVertexAdjacencies(mesh);
    return meshDifferentialCoordinates(mesh, vvAdj);
}
template<class Mesh, class R>
std::vector<nvl::Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    std::vector<nvl::Vector3<R>> dc(mesh.nextVertexId());
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        dc[vId] = meshDifferentialCoordinates(mesh, vId, vvAdj);
    }
    return dc;
}
template<class Mesh, class R>
nvl::Vector3<R> meshDifferentialCoordinates(
        const Mesh& mesh,
        typename Mesh::VertexId& vId)
{
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = nvl::meshVertexVertexAdjacencies(mesh);
    return meshDifferentialCoordinates(mesh, vId, vvAdj);
}
template<class Mesh, class R>
nvl::Vector3<R> meshDifferentialCoordinates(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    Point currentPoint = mesh.vertex(vId).point();

    //Calculate delta
    nvl::Vector3<R> delta = nvl::Vector3<R>::Zero();
    const std::vector<VertexId>& neighbors = vvAdj[vId];
    for(const int& neighborId : neighbors) {
        const Point& neighborPoint = mesh.vertex(neighborId).point();
        delta += neighborPoint;
    }
    delta /= neighbors.size();

    //Calculate differential coordinates
    nvl::Vector3<R> dc = currentPoint - delta;

    return dc;

}

}
