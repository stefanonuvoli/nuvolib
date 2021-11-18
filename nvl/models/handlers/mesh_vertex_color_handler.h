#ifndef NVL_MODELS_MESH_VERTEX_COLOR_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_COLOR_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class V, class C>
class MeshVertexColorHandler
{

public:

    /* Typedefs */

    typedef V Vertex;
    typedef C Color;

    typedef typename V::Id VertexId;

    typedef VectorWithDelete<Color> Container;


    /* Constructors */

    MeshVertexColorHandler();


    /* Methods */

    Color& vertexColor(const VertexId& id);
    const Color& vertexColor(const VertexId& id) const;
    void setVertexColor(const VertexId& id, const Color& color);

    Color& vertexColor(const Vertex& vertex);
    const Color& vertexColor(const Vertex& vertex) const;
    void setVertexColor(const Vertex& vertex, const Color& color);


protected:

    Container vVertexColors;

};

}

#include "mesh_vertex_color_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_COLOR_HANDLER_H
