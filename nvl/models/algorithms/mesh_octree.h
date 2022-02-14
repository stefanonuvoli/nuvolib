/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_OCTREE_H
#define NVL_MODELS_MESH_OCTREE_H

#include <nvl/nuvolib.h>

#include <nvl/structures/trees/octree.h>

#include <nvl/math/numeric_limits.h>
namespace nvl {

template<class Mesh>
Octree<typename Mesh::Point, typename Mesh::VertexId> meshVertexOctree(
        const Mesh& mesh,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());

template<class Mesh>
typename Mesh::VertexId meshVertexOctreeClosestVertex(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point);

template<class Mesh>
typename Mesh::Point meshVertexOctreeClosestPoint(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point);

template<class Mesh>
typename Mesh::Point meshVertexOctreeClosestPoint(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);

}

#include "mesh_octree.cpp"

#endif // NVL_MODELS_MESH_OCTREE_H
