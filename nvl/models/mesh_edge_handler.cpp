#include "mesh_edge_handler.h"

namespace nvl {

template<class T>
MeshEdgeHandler<T>::MeshEdgeHandler()
{

}

template<class T>
Size MeshEdgeHandler<T>::edgeNumber() const
{
    return vEdges.size();
}

template<class T>
typename MeshEdgeHandler<T>::EdgeId MeshEdgeHandler<T>::nextEdgeId() const
{
    return vEdges.realSize();
}

template<class T>
typename MeshEdgeHandler<T>::Edge& MeshEdgeHandler<T>::edge(const EdgeId& id)
{
    return vEdges[id];
}

template<class T>
const typename MeshEdgeHandler<T>::Edge& MeshEdgeHandler<T>::edge(const EdgeId& id) const
{
    return vEdges[id];
}

template<class T>
typename MeshEdgeHandler<T>::EdgeId MeshEdgeHandler<T>::addEdge(const Edge& edge)
{
    assert(edge.vertexNumber() == 2);
    Index newId = nextEdgeId();
    vEdges.push_back(edge);
    vEdges[newId].setId(newId);
    return newId;
}

template<class T>
template<class... Ts>
typename MeshEdgeHandler<T>::EdgeId MeshEdgeHandler<T>::addEdge(const Ts... vertices)
{
    Index newId = nextEdgeId();
    Edge edge(vertices...);
    assert(edge.vertexNumber() == 2);
    edge.setId(newId);
    vEdges.push_back(edge);
    return newId;
}


template<class T>
void MeshEdgeHandler<T>::deleteEdge(const EdgeId& id)
{
    Index copyId = id;
    vEdges[copyId].setId(MAX_INDEX);
    vEdges.erase(copyId);
}

template<class T>
bool MeshEdgeHandler<T>::isEdgeDeleted(const EdgeId& id) const
{
    return vEdges.isDeleted(id);
}

template<class T>
void MeshEdgeHandler<T>::deleteEdge(const Edge& edge)
{
    deleteEdge(edge.id());
}

template<class T>
bool MeshEdgeHandler<T>::isEdgeDeleted(const Edge& edge) const
{
    return isEdgeDeleted(edge.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshEdgeHandler<T>::Edge>, typename VectorWithDelete<typename MeshEdgeHandler<T>::Edge>::iterator> MeshEdgeHandler<T>::edges()
{
    return IteratorWrapper<VectorWithDelete<Edge>, typename VectorWithDelete<Edge>::iterator>(&vEdges);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshEdgeHandler<T>::Edge>, typename VectorWithDelete<typename MeshEdgeHandler<T>::Edge>::const_iterator> MeshEdgeHandler<T>::edges() const
{
    return IteratorWrapper<const VectorWithDelete<Edge>, typename VectorWithDelete<Edge>::const_iterator>(&vEdges);
}

template<class T>
void MeshEdgeHandler<T>::clearEdges()
{
    vEdges.clear();
}

}
