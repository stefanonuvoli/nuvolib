/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_REFINE_H
#define NVL_MODELS_MESH_REFINE_H

#include <nvl/nuvolib.h>

#ifdef NVL_VCGLIB_LOADED

#include <nvl/math/segment.h>

#include <map>
#include <utility>
#include <vector>

namespace nvl {

template<class Mesh, class F>
Mesh refineByImplicitFunction(
        const Mesh& mesh,
        const std::vector<F>& vertexFunction,
        std::vector<std::pair<typename Mesh::VertexId, typename Mesh::VertexId>>& curveLines,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace);

template<class VCGMesh, class F>
bool vcgRefineByImplicitFunction(
        VCGMesh& mesh,
        const std::vector<F>& vertexFunction,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace);

template<class VCGMesh>
bool vcgRefineMesh(
        VCGMesh& mesh,
        const std::map<std::pair<typename VCGMesh::CoordType, typename VCGMesh::CoordType>, typename VCGMesh::CoordType>& splitMap,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace);

template<class VCGMesh, class SplitLev, class EdgePred>
bool vcgRefineMesh(
        VCGMesh& mesh,
        const SplitLev& sp,
        const EdgePred& ep,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace);

}

#endif

#include "mesh_refine.cpp"

#endif // NVL_MODELS_MESH_REFINE_H
