/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_graph.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>

namespace nvl {

template<class Mesh>
Graph<typename Mesh::VertexId> meshGraph(
        const Mesh& mesh)
{
    return meshGraph(mesh, meshVertexVertexAdjacencies(mesh));
}


template<class Mesh>
Graph<typename Mesh::VertexId> meshGraph(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    Graph<VertexId> graph;

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        graph.addNode(vId);
    }


    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        for (const VertexId& adjVId : vvAdj[vId]) {
            const Point& p1 = mesh.vertexPoint(vId);
            const Point& p2 = mesh.vertexPoint(adjVId);

            const double distance = (p1 - p2).norm();
            graph.addEdge(vId, adjVId, distance);
        }
    }

    return graph;
}


template<class Mesh>
Graph<typename Mesh::FaceId> meshDualGraph(
        const Mesh& mesh,
        const bool directed)
{
    return meshGraph(mesh, meshFaceFaceAdjacencies(mesh));
}


template<class Mesh>
Graph<typename Mesh::FaceId> meshDualGraph(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Point Point;

    Graph<FaceId> graph;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        graph.addNode(fId);
    }


    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        for (const FaceId& adjFId : ffAdj[fId]) {
            const Point& p1 = meshFaceBarycenter(mesh, fId);
            const Point& p2 = meshFaceBarycenter(mesh, adjFId);

            const double distance = (p1 - p2).norm();
            graph.addEdge(fId, adjFId, distance);
        }
    }

    return graph;
}

}
