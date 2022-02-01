/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_CURVE_ON_MANIFOLD_H
#define NVL_MODELS_MESH_CURVE_ON_MANIFOLD_H

#include <nvl/nuvolib.h>

#ifdef NVL_VCGLIB_LOADED

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

#endif

#include "mesh_curve_on_manifold.cpp"

#endif // NVL_MODELS_MESH_CURVE_ON_MANIFOLD_H
