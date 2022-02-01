/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_io_data.h"

namespace nvl {

NVL_INLINE IOMeshMode::IOMeshMode()
    : vertices(true), polylines(true), faces(true),
      vertexNormals(true), vertexUVs(true), vertexColors(true),
      polylineColors(true),
      faceNormals(true), materials(true)
{

}

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
IOMeshData<P,VN,UV,VC,PC,FN,M>::IOMeshData()
{
    clear();
}

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
void IOMeshData<P,VN,UV,VC,PC,FN,M>::clear()
{
    vertices.clear();
    faces.clear();
    polylines.clear();
    vertexNormals.clear();
    vertexUVs.clear();
    vertexColors.clear();
    polylineColors.clear();
    faceNormals.clear();
    materials.clear();
    faceMaterials.clear();
    faceVertexNormals.clear();
    faceVertexUVs.clear();
}

}
