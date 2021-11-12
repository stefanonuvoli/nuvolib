#ifndef NVL_MODELS_MESH_SUBDIVISION_H
#define NVL_MODELS_MESH_SUBDIVISION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
void meshSubdivideInTrianglesBarycenter(
        Mesh& mesh);
template<class Mesh>
void meshSubdivideInTrianglesBarycenter(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace);
template<class Mesh>
std::vector<typename Mesh::FaceId> meshSubdivideInTrianglesBarycenter(
        Mesh& mesh,
        const typename Mesh::FaceId& fId);

}

#include "mesh_subdivision.cpp"

#endif // NVL_MODELS_MESH_SUBDIVISION_H
