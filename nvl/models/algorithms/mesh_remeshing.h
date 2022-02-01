/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_REMESHING_H
#define NVL_MODELS_MESH_REMESHING_H

#include <nvl/nuvolib.h>

#ifdef NVL_VCGLIB_LOADED

namespace nvl {

template<class Mesh>
Mesh isotropicRemeshing(
        const Mesh& mesh,
        const double& edgeSize,
        const unsigned int& iterations,
        const bool& fixBorders);

template<class VCGMesh>
void vcgIsotropicRemeshing(
        VCGMesh& vcgMesh,
        VCGMesh& vcgResultMesh,
        const double& edgeSize,
        const unsigned int& iterations,
        const bool& fixBorders);

}

#endif

#include "mesh_remeshing.cpp"

#endif // NVL_MODELS_MESH_REMESHING_H
