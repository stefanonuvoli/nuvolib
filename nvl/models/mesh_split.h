#ifndef NVL_MODELS_MESH_SPLIT_H
#define NVL_MODELS_MESH_SPLIT_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
typename Mesh::VertexId meshSplitEdge(
        Mesh& mesh,
        const typename Mesh::VertexId& vertex1,
        const typename Mesh::VertexId& vertex2,
        const typename Mesh::Point& newVertexPoint);

template<class Mesh>
typename Mesh::VertexId meshSplitEdge(
        Mesh& mesh,
        const typename Mesh::VertexId& vertex1,
        const typename Mesh::VertexId& vertex2,
        const typename Mesh::Point& newVertexPoint,
        std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);

}

#include "mesh_split.cpp"

#endif // NVL_MODELS_MESH_SPLIT_H
