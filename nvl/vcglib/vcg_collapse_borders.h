#ifndef NVL_VCGLIB_VCG_COLLAPSE_BORDERS_H
#define NVL_VCGLIB_VCG_COLLAPSE_BORDERS_H

#include <nvl/nuvolib.h>

#include <vector>
#include <unordered_set>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::VertexId> collapseBorders(
    Mesh& mesh,
    const std::vector<typename Mesh::VertexId>& verticesToKeep,
    std::vector<typename Mesh::VertexId>& birthVertex,
    std::vector<typename Mesh::FaceId>& birthFace);

template<class VCGMesh>
std::vector<size_t> vcgCollapseBorders(
    VCGMesh& vcgMesh,
    const std::vector<size_t>& verticesToKeep,
    std::vector<Index>& vcgCollapseBirthVertex,
    std::vector<Index>& vcgCollapseBirthFace);

}

#include "vcg_collapse_borders.cpp"

#endif // NVL_VCGLIB_VCG_COLLAPSE_BORDERS_H
