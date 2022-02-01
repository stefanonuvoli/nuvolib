/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_polyline.h"

namespace nvl {

template<class V>
MeshPolyline<V>::MeshPolyline() :
    vId(NULL_ID)
{

}

template<class V>
template<typename... Ts>
MeshPolyline<V>::MeshPolyline(Ts... vertices) : MeshPolyline()
{
    setVertexIds(vertices...);
}

template<class V>
typename MeshPolyline<V>::Id& MeshPolyline<V>::id()
{
    return vId;
}

template<class V>
const typename MeshPolyline<V>::Id& MeshPolyline<V>::id() const
{
    return vId;
}

template<class V>
void MeshPolyline<V>::setId(const Id& id)
{
    vId = id;
}


template<class V>
template<class T, typename... Ts>
void MeshPolyline<V>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) >= 1, "Polylines must be composed of at least 2 vertices.");
    assert(sizeof...(vertices) >= 1 && "Polylines must be composed of at least 2 vertices.");
    vVertices.resize(sizeof...(vertices) + 1);
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V>
void MeshPolyline<V>::setVertexIds(const Container& vector)
{
    assert(vector.size() >= 2 && "Polylines must be composed of at least 2 vertices.");
    this->vVertices.resize(vector.size());
    for (Index i = 0; i < vector.size(); ++i) {
        this->vVertices[i] = vector[i];
    }
}

template<class V>
Size MeshPolyline<V>::vertexNumber() const
{
    return vVertices.size();
}

template<class V>
void MeshPolyline<V>::resizeVertexNumber(Size vertexNumber)
{
    assert(vertexNumber >= 2 && "Polyline must be composed of at least 2 vertices.");
    vVertices.resize(vertexNumber, NULL_ID);
}

template<class V>
void MeshPolyline<V>::insertVertex(VertexId vId)
{
    this->vVertices.push_back(vId);
}

template<class V>
void MeshPolyline<V>::insertVertex(const Index& pos, const VertexId& vId)
{
    assert(pos <= this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.insert(this->vVertices.begin() + pos, vId);
}

template<class V>
void MeshPolyline<V>::insertVertex(const Vertex& vertex)
{
    insertVertex(vertex);
}

template<class V>
void MeshPolyline<V>::insertVertex(const Index& pos, const Vertex& vertex)
{
    insertVertex(pos, vertex.id());
}

template<class V>
void MeshPolyline<V>::eraseBackVertex()
{
    this->vVertices.resize(this->vVertices.size() - 1);
}

template<class V>
void MeshPolyline<V>::eraseVertex(const Index& pos)
{
    assert(pos < this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.erase(this->vVertices.begin() + pos);
}

template<class V>
void MeshPolyline<V>::eraseVertex(const Vertex& vertex)
{
    eraseVertex(vertex.id());
}

template<class V>
typename MeshPolyline<V>::Container& MeshPolyline<V>::vertexIds()
{
    return vVertices;
}

template<class V>
const typename MeshPolyline<V>::Container& MeshPolyline<V>::vertexIds() const
{
    return vVertices;
}

template<class V>
typename MeshPolyline<V>::VertexId& MeshPolyline<V>::vertexId(const Index& pos)
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    return vVertices[pos];
}

template<class V>
const typename MeshPolyline<V>::VertexId& MeshPolyline<V>::vertexId(const Index& pos) const
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    return vVertices[pos];
}

template<class V>
void MeshPolyline<V>::setVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    vVertices[pos] = vId;
}

template<class V>
void MeshPolyline<V>::setVertex(const Index& pos, const Vertex& vertex)
{
    setVertexId(pos, vertex.id());
}


template<class V>
typename MeshPolyline<V>::VertexId& MeshPolyline<V>::nextVertexId(const Index& pos)
{
    assert(pos <= vVertices.size() - 1 && "Index exceed the polyline dimension.");

    return vVertices[(pos + 1) % vVertices.size()];
}

template<class V>
const typename MeshPolyline<V>::VertexId& MeshPolyline<V>::nextVertexId(const Index& pos) const
{
    assert(pos < vVertices.size() - 1 && "Index exceed the polyline dimension.");

    return vVertices[(pos + 1)];
}

template<class V>
void MeshPolyline<V>::setNextVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertices.size() - 1 && "Index exceed the polyline dimension.");

    vVertices[(pos + 1)] = vId;
}

template<class V>
void MeshPolyline<V>::setNextVertex(const Index& pos, const Vertex& vertex)
{
    setNextVertexId(pos, vertex.id());
}

template<class V>
template<class T, typename... Ts>
void MeshPolyline<V>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V>
void MeshPolyline<V>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    vVertices[pos] = vertex.id();
}

template<class V>
void MeshPolyline<V>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    vVertices[pos] = vertexId;
}

template<class V>
void MeshPolyline<V>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos >= 2 && "Polylines must be composed of at least 2 vertices.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V>
std::ostream& operator<<(std::ostream& output, const MeshPolyline<V>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshPolyline<V>::VertexId& vId : face.vertexIds()) {
        if (vId == NULL_ID) {
            output << "x";
        }
        else {
            output << vId;
        }

        output << " ";
    }
    return output;
}



}
