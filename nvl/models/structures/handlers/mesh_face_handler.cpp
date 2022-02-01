/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_face_handler.h"

namespace nvl {

template<class T>
MeshFaceHandler<T>::MeshFaceHandler()
{

}

template<class T>
Size MeshFaceHandler<T>::faceNumber() const
{
    return vFaces.size();
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::nextFaceId() const
{
    return vFaces.realSize();
}

template<class T>
typename MeshFaceHandler<T>::Face& MeshFaceHandler<T>::face(const FaceId& id)
{
    return vFaces[id];
}

template<class T>
const typename MeshFaceHandler<T>::Face& MeshFaceHandler<T>::face(const FaceId& id) const
{
    return vFaces[id];
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::addFace(const Face& face)
{
    assert(face.vertexNumber() >= 3);
    FaceId newId = nextFaceId();
    vFaces.push_back(face);
    vFaces[newId].setId(newId);
    return newId;
}

template<class T>
template<class... Ts>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::addFace(const Ts... vertices)
{
    FaceId newId = nextFaceId();
    Face face(vertices...);
    assert(face.vertexNumber() >= 3);
    face.setId(newId);
    vFaces.push_back(face);
    return newId;
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::allocateFaces(const Size& n)
{
    return allocateFaces(n, Face());
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::allocateFaces(const Size& n, const Face& face)
{
    Index firstIndex = vFaces.realSize();
    Index lastIndex = firstIndex + n;

    vFaces.resize(lastIndex, face);
    for (Index i = firstIndex; i < lastIndex; i++) {
        vFaces[i].setId(i);
    }

    return firstIndex;
}

template<class T>
void MeshFaceHandler<T>::deleteFace(const FaceId& id)
{
    Index copyId = id;
    vFaces[copyId].setId(NULL_ID);
    vFaces.erase(copyId);
}

template<class T>
bool MeshFaceHandler<T>::isFaceDeleted(const FaceId& id) const
{
    return vFaces.isDeleted(id);
}

template<class T>
void MeshFaceHandler<T>::deleteFace(const Face& face)
{
    deleteFace(face.id());
}

template<class T>
bool MeshFaceHandler<T>::isFaceDeleted(const Face& face) const
{
    return isFaceDeleted(face.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshFaceHandler<T>::Face>, typename VectorWithDelete<typename MeshFaceHandler<T>::Face>::iterator> MeshFaceHandler<T>::faces()
{
    return IteratorWrapper<Container, typename Container::iterator>(&vFaces);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshFaceHandler<T>::Face>, typename VectorWithDelete<typename MeshFaceHandler<T>::Face>::const_iterator> MeshFaceHandler<T>::faces() const
{
    return IteratorWrapper<const Container, typename Container::const_iterator>(&vFaces);
}

template<class T>
void MeshFaceHandler<T>::clearFaces()
{
    vFaces.clear();
}

}
