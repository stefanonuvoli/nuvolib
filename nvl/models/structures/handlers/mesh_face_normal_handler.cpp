/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_face_normal_handler.h"

namespace nvl {

template<class F, class N>
MeshFaceNormalHandler<F,N>::MeshFaceNormalHandler()
{

}

template<class F, class N>
typename MeshFaceNormalHandler<F,N>::Normal& MeshFaceNormalHandler<F,N>::faceNormal(const FaceId& id)
{
    assert(id < vFaceNormals.realSize() && "Face normals not enabled or not properly initialized.");

    return vFaceNormals[id];
}

template<class F, class N>
const typename MeshFaceNormalHandler<F,N>::Normal& MeshFaceNormalHandler<F,N>::faceNormal(const FaceId& id) const
{
    assert(id < vFaceNormals.realSize() && "Face normals not enabled or not properly initialized.");

    return vFaceNormals[id];
}

template<class F, class N>
void MeshFaceNormalHandler<F,N>::setFaceNormal(const FaceId& id, const Normal& normal)
{
    assert(id < vFaceNormals.realSize() && "Face normals not enabled or not properly initialized.");

    vFaceNormals[id] = normal;
}

template<class F, class N>
typename MeshFaceNormalHandler<F,N>::Normal& MeshFaceNormalHandler<F,N>::faceNormal(const Face& face)
{
    return faceNormal(face.id());
}

template<class F, class N>
const typename MeshFaceNormalHandler<F,N>::Normal& MeshFaceNormalHandler<F,N>::faceNormal(const Face& face) const
{
    return faceNormal(face.id());
}

template<class F, class N>
void MeshFaceNormalHandler<F,N>::setFaceNormal(const Face& face, const Normal& normal)
{
    setFaceNormal(face.id(), normal);
}

}
