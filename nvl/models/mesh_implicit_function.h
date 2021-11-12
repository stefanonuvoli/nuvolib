#ifndef NVL_MODELS_MESH_IMPLICIT_FUNCTION_H
#define NVL_MODELS_MESH_IMPLICIT_FUNCTION_H

#include <nvl/models/mesh_adjacencies.h>

#include <nvl/math/segment.h>

#include <vector>


namespace nvl {

template<class Mesh, class F>
std::vector<Segment<typename Mesh::Point>> meshImplicitFunction(
        const Mesh& mesh,
        const std::vector<F>& vertexFunction);

}

#include "mesh_implicit_function.cpp"

#endif // NVL_MODELS_MESH_IMPLICIT_FUNCTION_H
