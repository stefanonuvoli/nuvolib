#ifndef NVL_MODELS_MESH_SUBDIVISION_H
#define NVL_MODELS_MESH_SUBDIVISION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
void meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh);
template<class Mesh>
void meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace);
template<class Mesh>
std::vector<typename Mesh::FaceId> meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh,
        const typename Mesh::FaceId& fId);

}

#include "mesh_subdivision.cpp"

#endif // NVL_MODELS_MESH_SUBDIVISION_H
