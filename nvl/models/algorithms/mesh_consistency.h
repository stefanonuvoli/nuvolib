/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_CONSISTENCY_H
#define NVL_MODELS_MESH_CONSISTENCY_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::FaceId> meshRemoveFacesWithDeletedVertices(Mesh& model);

template<class Mesh>
std::vector<typename Mesh::PolylineId> meshRemovePolylinesWithDeletedVertices(Mesh& model);

}

#include "mesh_consistency.cpp"

#endif // NVL_MODELS_MESH_CONSISTENCY_H
