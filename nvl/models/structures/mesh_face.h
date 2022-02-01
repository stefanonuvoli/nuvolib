/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_FACE_H
#define NVL_MODELS_MESH_FACE_H

#include <nvl/nuvolib.h>

#include <array>
#include <vector>
#include <ostream>

namespace nvl {

template<class V, class C>
class MeshFace
{

public:

    /* Typedefs */

    typedef Index Id;

    typedef V Vertex;
    typedef typename V::Id VertexId;

    typedef C Container;


    /* Constructors */

    explicit MeshFace();


    /* Methods */

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    Size vertexNumber() const;

    Container& vertexIds();
    const Container& vertexIds() const;
    void setVertexIds(const Container& container);

    VertexId& vertexId(const Index& pos);
    const VertexId& vertexId(const Index& pos) const;
    void setVertexId(const Index& pos, const VertexId& vId);
    void setVertex(const Index& pos, const Vertex& vId);

    VertexId& nextVertexId(const Index& pos);
    const VertexId& nextVertexId(const Index& pos) const;
    void setNextVertexId(const Index& pos, const VertexId& vId);
    void setNextVertex(const Index& pos, const Vertex& vId);



protected:

    Id vId;

    Container vVertexIds;
};


template<class V, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,C>& face);

}

#include "mesh_face.cpp"

#endif // NVL_MODELS_MESH_FACE_H
