#ifndef NVL_MESH_GRAPH_H
#define NVL_MESH_GRAPH_H

#include <nvl/structures/graph.h>

namespace nvl {

template<class Mesh>
nvl::Graph<typename Mesh::VertexId> meshGraph(
        const Mesh& mesh);

template<class Mesh>
nvl::Graph<typename Mesh::VertexId> meshGraph(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);


template<class Mesh>
nvl::Graph<typename Mesh::VertexId> meshDualGraph(
        const Mesh& mesh);

template<class Mesh>
nvl::Graph<typename Mesh::VertexId> meshDualGraph(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

}

#include "mesh_graph.cpp"

#endif // NVL_MESH_GRAPH_H
