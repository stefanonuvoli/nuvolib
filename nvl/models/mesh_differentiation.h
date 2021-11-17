#ifndef NVL_MODELS_MESH_DIFFERENTIATION_H
#define NVL_MODELS_MESH_DIFFERENTIATION_H

#include <nvl/nuvolib.h>

#include <nvl/models/mesh_adjacencies.h>
#include <nvl/math/vector.h>

#include <vector>

namespace nvl {

template<class Mesh, class R = double>
std::vector<Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh);
template<class Mesh, class R = double>
std::vector<Vector3<R>> meshDifferentialCoordinates(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
template<class Mesh, class R = double>
Vector3<R> meshDifferentialCoordinates(
        const Mesh& mesh,
        typename Mesh::VertexId& vId);
template<class Mesh, class R = double>
Vector3<R> meshDifferentialCoordinates(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);


}

#include "mesh_differentiation.cpp"

#endif // NVL_MODELS_MESH_DIFFERENTIATION_H
