/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_IMPLICIT_FUNCTION_H
#define NVL_MODELS_MESH_IMPLICIT_FUNCTION_H

#include <nvl/models/algorithms/mesh_adjacencies.h>

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
