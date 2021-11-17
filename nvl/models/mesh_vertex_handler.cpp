#include "mesh_vertex_handler.h"

namespace nvl {

template<class T>
MeshVertexHandler<T>::MeshVertexHandler()
{

}

template<class T>
Size MeshVertexHandler<T>::vertexNumber() const
{
    return vVertices.size();
}

template<class T>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::nextVertexId() const
{
    return vVertices.realSize();
}

template<class T>
typename MeshVertexHandler<T>::Vertex& MeshVertexHandler<T>::vertex(const VertexId& id)
{
    return vVertices[id];
}

template<class T>
const typename MeshVertexHandler<T>::Vertex& MeshVertexHandler<T>::vertex(const VertexId& id) const
{
    return vVertices[id];
}

template<class T>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::addVertex(const Vertex& vertex)
{
    VertexId newId = nextVertexId();
    vVertices.push_back(vertex);
    vVertices[newId].setId(newId);
    return newId;
}

template<class T>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::addVertex(const Point& point)
{
    VertexId newId = nextVertexId();
    Vertex vertex(point);
    vertex.setId(newId);
    vVertices.push_back(vertex);
    return newId;
}

template<class T>
template<class... Ts>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::addVertex(const Ts... points)
{
    VertexId newId = nextVertexId();
    Vertex vertex(points...);
    vertex.setId(newId);
    vVertices.push_back(vertex);
    return newId;
}

template<class T>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::allocateVertices(const Size& n)
{
    return allocateVertices(n, T());
}

template<class T>
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::allocateVertices(const Size& n, const Vertex& vertex)
{
    Index firstIndex = vVertices.realSize();
    Index lastIndex = firstIndex + n;

    vVertices.resize(lastIndex, vertex);
    for (Index i = firstIndex; i < lastIndex; i++) {
        vVertices[i].setId(i);
    }

    return firstIndex;
}

template<class T>
void MeshVertexHandler<T>::deleteVertex(const VertexId& id)
{
    Index copyId = id;
    vVertices[copyId].setId(NULL_ID);
    vVertices.erase(copyId);
}

template<class T>
bool MeshVertexHandler<T>::isVertexDeleted(const VertexId& id) const
{
    return vVertices.isDeleted(id);
}

template<class T>
void MeshVertexHandler<T>::deleteVertex(const Vertex& vertex)
{
    deleteVertex(vertex.id());
}

template<class T>
bool MeshVertexHandler<T>::isVertexDeleted(const Vertex& vertex) const
{
    return isVertexDeleted(vertex.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshVertexHandler<T>::Vertex>, typename VectorWithDelete<typename MeshVertexHandler<T>::Vertex>::iterator> MeshVertexHandler<T>::vertices()
{
    return IteratorWrapper<VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::iterator>(&vVertices);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshVertexHandler<T>::Vertex>, typename VectorWithDelete<typename MeshVertexHandler<T>::Vertex>::const_iterator> MeshVertexHandler<T>::vertices() const
{
    return IteratorWrapper<const VectorWithDelete<Vertex>, typename VectorWithDelete<Vertex>::const_iterator>(&vVertices);
}

template<class T>
void MeshVertexHandler<T>::clearVertices()
{
    vVertices.clear();
}

}
