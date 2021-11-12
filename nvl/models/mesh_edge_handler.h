#ifndef NVL_MODELS_MESH_EDGE_HANDLER_H
#define NVL_MODELS_MESH_EDGE_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class T>
class MeshEdgeHandler
{

public:

    /* Typedefs */

    typedef T Edge;
    typedef Index EdgeId;

    typedef VectorWithDelete<Edge> EdgeContainer;


    /* Constructors */

    MeshEdgeHandler();


    /* Methods */

    Size edgeNumber() const;
    EdgeId nextEdgeId() const;

    Edge& edge(const EdgeId& id);
    const Edge& edge(const EdgeId& id) const;

    EdgeId addEdge(const Edge& edge);
    template<class... Ts>
    EdgeId addEdge(const Ts... vertices);

    void deleteEdge(const EdgeId& id);
    bool isEdgeDeleted(const EdgeId& id) const;

    void deleteEdge(const Edge& edge);
    bool isEdgeDeleted(const Edge& edge) const;

    IteratorWrapper<VectorWithDelete<Edge>, typename VectorWithDelete<Edge>::iterator> edges();
    IteratorWrapper<const VectorWithDelete<Edge>, typename VectorWithDelete<Edge>::const_iterator> edges() const;

    void clearEdges();


protected:

    VectorWithDelete<Edge> vEdges;

};

}

#include "mesh_edge_handler.cpp"

#endif // NVL_MODELS_MESH_EDGE_HANDLER_H
