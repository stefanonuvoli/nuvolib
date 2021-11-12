#ifndef NVL_MODELS_MESH_CONSISTENCY_H
#define NVL_MODELS_MESH_CONSISTENCY_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::FaceId> meshRemoveFacesWithDeletedVertices(Mesh& model);

template<class Mesh>
std::vector<typename Mesh::PolylineId> meshRemovePolylinesWithDeletedVertices(Mesh& model);

template<class Mesh>
std::vector<typename Mesh::EdgeId> meshRemoveEdgesWithDeletedVertices(Mesh& model);

template<class Mesh>
std::vector<typename Mesh::EdgeId> meshRemoveEdgesWithDeletedFaces(Mesh& model);

}

#include "mesh_consistency.cpp"

#endif // NVL_MODELS_MESH_CONSISTENCY_H
