#include "mesh_face_wedge_normal_handler.h"

namespace nvl {

template<class F, class N>
MeshFaceWedgeNormalHandler<F,N>::MeshFaceWedgeNormalHandler()
{

}

template<class F, class N>
bool MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormalsAreNull(const FaceId& id) const
{
    assert(id < vFaceWedgeNormals.realSize() && "Face wedge normals not enabled or not properly initialized.");

    return vFaceWedgeNormals[id].empty();
}

template<class F, class N>
std::vector<typename MeshFaceWedgeNormalHandler<F,N>::WedgeNormalId>& MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormals(const FaceId& id)
{
    assert(id < vFaceWedgeNormals.realSize() && "Face wedge normals not enabled or not properly initialized.");

    return vFaceWedgeNormals[id];
}

template<class F, class N>
const std::vector<typename MeshFaceWedgeNormalHandler<F,N>::WedgeNormalId>& MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormals(const FaceId& id) const
{
    assert(id < vFaceWedgeNormals.realSize() && "Face wedge normals not enabled or not properly initialized.");

    return vFaceWedgeNormals[id];
}

template<class F, class N>
void MeshFaceWedgeNormalHandler<F,N>::setFaceWedgeNormals(const FaceId& id, const std::vector<WedgeNormalId>& wedgeNormalId)
{
    assert(id < vFaceWedgeNormals.realSize() && "Face wedge normals not enabled or not properly initialized.");

    vFaceWedgeNormals[id] = wedgeNormalId;
}

template<class F, class N>
void MeshFaceWedgeNormalHandler<F,N>::unsetFaceWedgeNormals(const FaceId& id)
{
    assert(id < vFaceWedgeNormals.realSize() && "Face wedge normals not enabled or not properly initialized.");

    vFaceWedgeNormals[id].clear();
}

template<class F, class N>
bool MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormalsAreNull(const Face& face) const
{
    return faceWedgeNormalsAreNull(face.id());
}

template<class F, class N>
std::vector<typename MeshFaceWedgeNormalHandler<F,N>::WedgeNormalId>& MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormals(const Face& face)
{
    return faceWedgeNormals(face.id());
}

template<class F, class N>
const std::vector<typename MeshFaceWedgeNormalHandler<F,N>::WedgeNormalId>& MeshFaceWedgeNormalHandler<F,N>::faceWedgeNormals(const Face& face) const
{
    return faceWedgeNormals(face.id());
}

template<class F, class N>
void MeshFaceWedgeNormalHandler<F,N>::setFaceWedgeNormals(const Face& face, const std::vector<WedgeNormalId>& wedgeNormalId)
{
    setFaceWedgeNormals(face.id(), wedgeNormalId);
}

template<class F, class N>
void MeshFaceWedgeNormalHandler<F,N>::unsetFaceWedgeNormals(const Face& face)
{
    unsetFaceWedgeNormals(face.id());
}

}
