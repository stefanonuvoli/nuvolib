/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MESH_GEOMETRIC_INFORMATION_H
#define NVL_MESH_GEOMETRIC_INFORMATION_H

#include <nvl/nuvolib.h>

#include <nvl/math/alignedbox.h>

namespace nvl {

template<class Mesh>
AlignedBox3<typename Mesh::Scalar> meshBoundingBox(const Mesh& mesh);

template<class Mesh>
typename Mesh::Scalar meshAverageEdgeLength(const Mesh& mesh);

template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::FaceId& fId);
template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::Face& face);

template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::FaceId& fId, const Index& fePos);
template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::Face& face, const Index& fePos);

template<class Mesh>
typename Mesh::Scalar meshFaceArea(const Mesh& mesh, const typename Mesh::FaceId& fId);
template<class Mesh>
typename Mesh::Scalar meshFaceArea(const Mesh& mesh, const typename Mesh::Face& face);



}

#include "mesh_geometric_information.cpp"

#endif // NVL_MESH_GEOMETRIC_INFORMATION_H
