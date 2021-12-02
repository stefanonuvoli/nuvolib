#ifndef NVL_VCGLIB_VCG_CURVE_ON_MANIFOLD_H
#define NVL_VCGLIB_VCG_CURVE_ON_MANIFOLD_H

#include <nvl/nuvolib.h>

#include <nvl/math/segment.h>

#include <vector>
#include <utility>

namespace nvl {

template<class Mesh>
void curveOnManifold(
        const Mesh& mesh,
        const std::vector<Segment<typename Mesh::Point>>& segments,
        Mesh& outputMesh,
        std::vector<std::pair<typename Mesh::VertexId, typename Mesh::VertexId>>& curveLines,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        const int& firstStepIterations = 30,
        const int& secondStepIterations = 10,
        const double& baryCentricThreshold = 0.05,
        const bool fixBorders = true,
        const bool fixCorners = false);

template<class VCGMesh>
bool vcgCurveOnManifold(
        VCGMesh& mesh,
        VCGMesh& edgeMesh,
        VCGMesh& resultMesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace,
        const int& firstStepIterations = 30,
        const int& secondStepIterations = 10,
        const double& baryCentricThreshold = 0.05,
        const bool fixBorders = true,
        const bool fixCorners = false);

}

#include "vcg_curve_on_manifold.cpp"

#endif // NVL_VCGLIB_VCG_CURVE_ON_MANIFOLD_H