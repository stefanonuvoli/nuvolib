#ifndef NVL_MODELS_MESH_VERTEX_NORMAL_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_NORMAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class V, class N>
class MeshVertexNormalHandler
{

public:

    /* Typedefs */

    typedef V Vertex;
    typedef N Normal;

    typedef typename V::Id VertexId;

    typedef VectorWithDelete<Normal> Container;


    /* Constructors */

    MeshVertexNormalHandler();


    /* Methods */

    Normal& vertexNormal(const VertexId& id);
    const Normal& vertexNormal(const VertexId& id) const;
    void setVertexNormal(const VertexId& id, const Normal& normal);

    Normal& vertexNormal(const Vertex& vertex);
    const Normal& vertexNormal(const Vertex& vertex) const;
    void setVertexNormal(const Vertex& vertex, const Normal& normal);


protected:

    Container vVertexNormals;

};

}

#include "mesh_vertex_normal_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_NORMAL_HANDLER_H
