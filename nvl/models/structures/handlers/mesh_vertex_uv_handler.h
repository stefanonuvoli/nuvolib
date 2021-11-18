#ifndef NVL_MODELS_MESH_VERTEX_UV_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_UV_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class V, class U>
class MeshVertexUVHandler
{

public:

    /* Typedefs */

    typedef V Vertex;
    typedef U UV;

    typedef typename V::Id VertexId;

    typedef VectorWithDelete<UV> Container;


    /* Constructors */

    MeshVertexUVHandler();


    /* Methods */

    UV& vertexUV(const VertexId& id);
    const UV& vertexUV(const VertexId& id) const;
    void setVertexUV(const VertexId& id, const UV& uv);

    UV& vertexUV(const Vertex& vertex);
    const UV& vertexUV(const Vertex& vertex) const;
    void setVertexUV(const Vertex& vertex, const UV& uv);


protected:

    Container vVertexUVs;

};

}

#include "mesh_vertex_uv_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_UV_HANDLER_H
