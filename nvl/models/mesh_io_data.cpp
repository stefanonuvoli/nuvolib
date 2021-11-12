#include "mesh_io_data.h"

namespace nvl {

NVL_INLINE IOMeshMode::IOMeshMode()
    : vertices(true), polylines(true), faces(true),
      vertexNormals(true), vertexUV(true), vertexColors(true),
      textures(true), faceNormals(true), materials(true)
{

}

template<class P, class VN, class VC, class UV, class FN, class M>
IOMeshData<P,VN,VC,UV,FN,M>::IOMeshData()
{
    clear();
}

template<class P, class VN, class VC, class UV, class FN, class M>
void IOMeshData<P,VN,VC,UV,FN,M>::clear()
{
    vertices.clear();
    faces.clear();
    polylines.clear();
    vertexNormals.clear();
    vertexUV.clear();
    vertexColors.clear();
    faceNormals.clear();
    materials.clear();
    faceMaterials.clear();
    faceVertexNormals.clear();
    faceVertexUV.clear();
}

}
