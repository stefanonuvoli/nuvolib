/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_CURVATURE_H
#define NVL_MODELS_MESH_CURVATURE_H

#include <nvl/nuvolib.h>

#include <nvl/math/principal_curvatures.h>

#include <vector>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvature(const Mesh& mesh);
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvature(const Mesh& mesh);

#ifdef NVL_VCGLIB_LOADED

template<class Mesh>
std::vector<PrincipalCurvatures<typename Mesh::Scalar>> meshPrincipalCurvature(const Mesh& mesh);

#endif

template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureLB(const Mesh& mesh);
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvatureLB(const Mesh& mesh);

#ifdef NVL_VCGLIB_LOADED

template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureVCG(const Mesh& mesh);
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvatureVCG(const Mesh& mesh);
template<class Mesh>
std::vector<PrincipalCurvatures<typename Mesh::Scalar>> meshPrincipalCurvatureVCG(const Mesh& mesh);

#endif

}

#include "mesh_curvature.cpp"

#endif // NVL_MODELS_MESH_CURVATURE_H
