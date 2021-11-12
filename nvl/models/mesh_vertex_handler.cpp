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
typename MeshVertexHandler<T>::VertexId MeshVertexHandler<T>::addVertex(const Ts... coordinates)
{
    VertexId newId = nextVertexId();
    Vertex vertex(coordinates...);
    vertex.setId(newId);
    vVertices.push_back(vertex);
    return newId;
}

template<class T>
void MeshVertexHandler<T>::deleteVertex(const VertexId& id)
{
    Index copyId = id;
    vVertices[copyId].setId(MAX_INDEX);
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

template<class T>
typename MeshVertexHandler<T>::VertexNormal& MeshVertexHandler<T>::vertexNormal(const VertexId& id)
{
    return vertex(id).normal();
}

template<class T>
const typename MeshVertexHandler<T>::VertexNormal& MeshVertexHandler<T>::vertexNormal(const VertexId& id) const
{
    return vertex(id).normal();
}

template<class T>
void MeshVertexHandler<T>::setVertexNormal(const VertexId& id, const VertexNormal& normal)
{
    vertex(id).setNormal(normal);
}

template<class T>
typename MeshVertexHandler<T>::VertexNormal& MeshVertexHandler<T>::vertexNormal(const Vertex& vertex)
{
    return vertexNormal(vertex.id());
}

template<class T>
const typename MeshVertexHandler<T>::VertexNormal& MeshVertexHandler<T>::vertexNormal(const Vertex& vertex) const
{
    return vertexNormal(vertex.id());
}

template<class T>
void MeshVertexHandler<T>::setVertexNormal(const Vertex& vertex, const VertexNormal& normal)
{
    setVertexNormal(vertex.id(), normal);
}

template<class T>
typename MeshVertexHandler<T>::UV& MeshVertexHandler<T>::vertexUV(const VertexId& id)
{
    return vertex(id).uvCoords();
}

template<class T>
const typename MeshVertexHandler<T>::UV& MeshVertexHandler<T>::vertexUV(const VertexId& id) const
{
    return vertex(id).uvCoords();
}

template<class T>
void MeshVertexHandler<T>::setVertexUV(const VertexId& id, const UV& uvCoords)
{
    vertex(id).setUV(uvCoords);
}

template<class T>
typename MeshVertexHandler<T>::UV& MeshVertexHandler<T>::vertexUV(const Vertex& vertex)
{
    return vertexUV(vertex.id());
}

template<class T>
const typename MeshVertexHandler<T>::UV& MeshVertexHandler<T>::vertexUV(const Vertex& vertex) const
{
    return vertexUV(vertex.id());
}

template<class T>
void MeshVertexHandler<T>::setVertexUV(const Vertex& vertex, const UV& uvCoords)
{
    setVertexUV(vertex.id(), uvCoords);
}

template<class T>
typename MeshVertexHandler<T>::VertexColor& MeshVertexHandler<T>::vertexColor(const VertexId& id)
{
    return vertex(id).color();
}

template<class T>
const typename MeshVertexHandler<T>::VertexColor& MeshVertexHandler<T>::vertexColor(const VertexId& id) const
{
    return vertex(id).color();
}

template<class T>
void MeshVertexHandler<T>::setVertexColor(const VertexId& id, const VertexColor& color)
{
    vertex(id).setColor(color);
}

template<class T>
typename MeshVertexHandler<T>::VertexColor& MeshVertexHandler<T>::vertexColor(const Vertex& vertex)
{
    return vertexColor(vertex.id());
}

template<class T>
const typename MeshVertexHandler<T>::VertexColor& MeshVertexHandler<T>::vertexColor(const Vertex& vertex) const
{
    return vertexColor(vertex.id());
}

template<class T>
void MeshVertexHandler<T>::setVertexColor(const Vertex& vertex, const VertexColor& color)
{
    setVertexColor(vertex.id(), color);
}

}
