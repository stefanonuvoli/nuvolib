/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_SMOOTHING_H
#define NVL_MODELS_MESH_SMOOTHING_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* Laplacian smoothing */

template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const unsigned int iterations);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const unsigned int iterations,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);

template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
}

#include "mesh_smoothing.cpp"

#endif // NVL_MODELS_MESH_SMOOTHING_H
