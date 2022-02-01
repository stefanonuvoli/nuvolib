/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_FACE_HANDLER_H
#define NVL_MODELS_MESH_FACE_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class T>
class MeshFaceHandler
{

public:

    /* Typedefs */

    typedef T Face;
    typedef typename Face::Id FaceId;

    typedef typename Face::VertexId VertexId;

    typedef VectorWithDelete<Face> Container;


    /* Constructors */

    MeshFaceHandler();


    /* Methods */

    Size faceNumber() const;
    FaceId nextFaceId() const;

    Face& face(const FaceId& id);
    const Face& face(const FaceId& id) const;

    FaceId addFace(const Face& face);
    template<class... Ts>
    FaceId addFace(const Ts... vertices);

    FaceId allocateFaces(const Size& n);
    FaceId allocateFaces(const Size& n, const Face& face);

    void deleteFace(const FaceId& id);
    bool isFaceDeleted(const FaceId& id) const;

    void deleteFace(const Face& face);
    bool isFaceDeleted(const Face& face) const;

    IteratorWrapper<Container, typename Container::iterator> faces();
    IteratorWrapper<const Container, typename Container::const_iterator> faces() const;

    void clearFaces();


protected:

    Container vFaces;

};

}

#include "mesh_face_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_HANDLER_H
