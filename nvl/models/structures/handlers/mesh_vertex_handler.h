#ifndef NVL_MODELS_MESH_VERTEX_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class T>
class MeshVertexHandler
{

public:

    /* Typedefs */

    typedef T Vertex;
    typedef typename Vertex::Id VertexId;

    typedef typename Vertex::Point Point;

    typedef VectorWithDelete<Vertex> Container;


    /* Constructors */

    MeshVertexHandler();


    /* Vertices */

    Size vertexNumber() const;
    VertexId nextVertexId() const;

    Vertex& vertex(const VertexId& id);
    const Vertex& vertex(const VertexId& id) const;

    VertexId addVertex(const Vertex& vertex);
    VertexId addVertex(const Point& point);
    template<class... Ts>
    VertexId addVertex(const Ts... points);

    VertexId allocateVertices(const Size& n);
    VertexId allocateVertices(const Size& n, const Vertex& vertex);

    void deleteVertex(const VertexId& id);
    bool isVertexDeleted(const VertexId& id) const;

    void deleteVertex(const Vertex& vertex);
    bool isVertexDeleted(const Vertex& vertex) const;

    IteratorWrapper<VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::iterator> vertices();
    IteratorWrapper<const VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::const_iterator> vertices() const;

    void clearVertices();


protected:

    VectorWithDelete<Vertex> vVertices;

};

}

#include "mesh_vertex_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_HANDLER_H
