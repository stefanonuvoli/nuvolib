/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_face_wedge_uv_handler.h"

namespace nvl {

template<class F, class U>
MeshFaceWedgeUVHandler<F,U>::MeshFaceWedgeUVHandler()
{

}

template<class F, class U>
bool MeshFaceWedgeUVHandler<F,U>::faceWedgeUVsAreNull(const FaceId& id) const
{
    assert(id < vFaceWedgeUVs.realSize() && "Face wedge uv not enabled or not properly initialized.");

    return vFaceWedgeUVs[id].empty();
}

template<class F, class U>
std::vector<typename MeshFaceWedgeUVHandler<F,U>::WedgeUVId>& MeshFaceWedgeUVHandler<F,U>::faceWedgeUVs(const FaceId& id)
{
    assert(id < vFaceWedgeUVs.realSize() && "Face wedge uv not enabled or not properly initialized.");

    return vFaceWedgeUVs[id];
}

template<class F, class U>
const std::vector<typename MeshFaceWedgeUVHandler<F,U>::WedgeUVId>& MeshFaceWedgeUVHandler<F,U>::faceWedgeUVs(const FaceId& id) const
{
    assert(id < vFaceWedgeUVs.realSize() && "Face wedge uv not enabled or not properly initialized.");

    return vFaceWedgeUVs[id];
}

template<class F, class U>
void MeshFaceWedgeUVHandler<F,U>::setFaceWedgeUVs(const FaceId& id, const std::vector<WedgeUVId>& wedgeUVId)
{
    assert(id < vFaceWedgeUVs.realSize() && "Face wedge uv not enabled or not properly initialized.");

    vFaceWedgeUVs[id] = wedgeUVId;
}

template<class F, class U>
void MeshFaceWedgeUVHandler<F,U>::unsetFaceWedgeUVs(const FaceId& id)
{
    assert(id < vFaceWedgeUVs.realSize() && "Face wedge uv not enabled or not properly initialized.");

    vFaceWedgeUVs[id].clear();
}

template<class F, class U>
bool MeshFaceWedgeUVHandler<F,U>::faceWedgeUVsAreNull(const Face& face) const
{
    return faceWedgeUVsAreNull(face.id());
}

template<class F, class U>
std::vector<typename MeshFaceWedgeUVHandler<F,U>::WedgeUVId>& MeshFaceWedgeUVHandler<F,U>::faceWedgeUVs(const Face& face)
{
    return faceWedgeUVs(face.id());
}

template<class F, class U>
const std::vector<typename MeshFaceWedgeUVHandler<F,U>::WedgeUVId>& MeshFaceWedgeUVHandler<F,U>::faceWedgeUVs(const Face& face) const
{
    return faceWedgeUVs(face.id());
}

template<class F, class U>
void MeshFaceWedgeUVHandler<F,U>::setFaceWedgeUVs(const Face& face, const std::vector<WedgeUVId>& wedgeUVId)
{
    setFaceWedgeUVs(face.id(), wedgeUVId);
}

template<class F, class U>
void MeshFaceWedgeUVHandler<F,U>::unsetFaceWedgeUVs(const Face& face)
{
    unsetFaceWedgeUVs(face.id());
}

}
