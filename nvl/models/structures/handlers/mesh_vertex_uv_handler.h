/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_VERTEX_UV_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_UV_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class V, class U>
class MeshVertexUVHandler
{

public:

    /* Typedefs */

    typedef V Vertex;
    typedef U VertexUV;

    typedef typename V::Id VertexId;

    typedef VectorWithDelete<VertexUV> Container;


    /* Constructors */

    MeshVertexUVHandler();


    /* Methods */

    VertexUV& vertexUV(const VertexId& id);
    const VertexUV& vertexUV(const VertexId& id) const;
    void setVertexUV(const VertexId& id, const VertexUV& uv);

    VertexUV& vertexUV(const Vertex& vertex);
    const VertexUV& vertexUV(const Vertex& vertex) const;
    void setVertexUV(const Vertex& vertex, const VertexUV& uv);


protected:

    Container vVertexUVs;

};

}

#include "mesh_vertex_uv_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_UV_HANDLER_H
