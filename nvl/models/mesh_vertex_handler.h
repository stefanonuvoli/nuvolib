#ifndef NVL_MODELS_MESH_VERTEX_HANDLER_H
#define NVL_MODELS_MESH_VERTEX_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

#include <nvl/math/alignedbox.h>

namespace nvl {

template<class T>
class MeshVertexHandler
{

public:

    /* Typedefs */

    typedef T Vertex;
    typedef typename Vertex::VertexId VertexId;
    typedef typename Vertex::VertexNormal VertexNormal;
    typedef typename Vertex::VertexColor VertexColor;
    typedef typename Vertex::UV UV;
    typedef typename Vertex::Point Point;
    typedef typename Vertex::Scalar Scalar;

    typedef VectorWithDelete<Vertex> VertexContainer;

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
    VertexId addVertex(const Ts... coordinates);

    void deleteVertex(const VertexId& id);
    bool isVertexDeleted(const VertexId& id) const;

    void deleteVertex(const Vertex& vertex);
    bool isVertexDeleted(const Vertex& vertex) const;

    IteratorWrapper<VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::iterator> vertices();
    IteratorWrapper<const VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::const_iterator> vertices() const;

    void clearVertices();


    /* Vertex normals */

    VertexNormal& vertexNormal(const VertexId& id);
    const VertexNormal& vertexNormal(const VertexId& id) const;
    void setVertexNormal(const VertexId& id, const VertexNormal& normal);

    VertexNormal& vertexNormal(const Vertex& vertex);
    const VertexNormal& vertexNormal(const Vertex& vertex) const;
    void setVertexNormal(const Vertex& vertex, const VertexNormal& normal);


    /* Vertex UV coords */

    UV& vertexUV(const VertexId& id);
    const UV& vertexUV(const VertexId& id) const;
    void setVertexUV(const VertexId& id, const UV& uvCoords);

    UV& vertexUV(const Vertex& vertex);
    const UV& vertexUV(const Vertex& vertex) const;
    void setVertexUV(const Vertex& vertex, const UV& uvCoords);


    /* Vertex colors */

    VertexColor& vertexColor(const VertexId& id);
    const VertexColor& vertexColor(const VertexId& id) const;
    void setVertexColor(const VertexId& id, const VertexColor& color);

    VertexColor& vertexColor(const Vertex& vertex);
    const VertexColor& vertexColor(const Vertex& vertex) const;
    void setVertexColor(const Vertex& vertex, const VertexColor& color);

protected:

    VectorWithDelete<Vertex> vVertices;

};

}

#include "mesh_vertex_handler.cpp"

#endif // NVL_MODELS_MESH_VERTEX_HANDLER_H
