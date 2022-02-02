/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_CURVATURE_H
#define NVL_MODELS_MESH_CURVATURE_H

#include <nvl/nuvolib.h>

#include <nvl/models/algorithms/mesh_operators.h>

#include <vector>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureLB(const Mesh& mesh);

template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvature(const Mesh& mesh);

}

#include "mesh_curvature.cpp"

#endif // NVL_MODELS_MESH_CURVATURE_H
