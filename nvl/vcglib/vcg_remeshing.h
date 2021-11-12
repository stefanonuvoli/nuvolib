#ifndef NVL_VCGLIB_VCG_REMESHING_H
#define NVL_VCGLIB_VCG_REMESHING_H

#include <nvl/nuvolib.h>

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

#include "vcg_remeshing.cpp"

#endif // NVL_VCGLIB_VCG_REMESHING_H
