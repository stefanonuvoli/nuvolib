/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_SMOOTHING_REPROJECTION_H
#define NVL_MODELS_MESH_SMOOTHING_REPROJECTION_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/trees/octree.h>

namespace nvl {

/* Laplacian smoothing */

template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const unsigned int iterations,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());

template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const unsigned int iterations,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshLaplacianSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);


/* Cotangent laplacian smoothing */

template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const Size maxBoxElements = NVL_OCTREE_DEFAULT_BOX_ELEMENTS,
        const typename Mesh::Scalar maxBoxRadius = std::numeric_limits<typename Mesh::Scalar>::max());

template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);
template<class Mesh>
void meshCotangentSmoothingReprojection(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);

}

#include "mesh_smoothing_reprojection.cpp"

#endif // NVL_MODELS_MESH_SMOOTHING_REPROJECTION_H
