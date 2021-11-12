#include "mesh_polyline.h"

namespace nvl {

template<class V, class C>
MeshPolyline<V,C>::MeshPolyline() : vId(MAX_INDEX), vColor(PolylineColor(63, 63, 63))
{

}

template<class V, class C>
template<typename... Ts>
MeshPolyline<V,C>::MeshPolyline(Ts... vertices) : MeshPolyline()
{
    setVertexIds(vertices...);
}

template<class V, class C>
typename MeshPolyline<V,C>::PolylineId& MeshPolyline<V,C>::id()
{
    return vId;
}

template<class V, class C>
const typename MeshPolyline<V,C>::PolylineId& MeshPolyline<V,C>::id() const
{
    return vId;
}

template<class V, class C>
void MeshPolyline<V,C>::setId(const PolylineId& id)
{
    vId = id;
}


template<class V, class C>
template<class T, typename... Ts>
void MeshPolyline<V,C>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) >= 1, "Polylines must be composed of at least 2 vertices.");
    assert(sizeof...(vertices) >= 1 && "Polylines must be composed of at least 2 vertices.");
    vVertices.resize(sizeof...(vertices) + 1);
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V, class C>
void MeshPolyline<V,C>::setVertexIds(const std::vector<VertexId>& vector)
{
    assert(vector.size() >= 2 && "Polylines must be composed of at least 2 vertices.");
    this->vVertices.resize(vector.size());
    for (Index i = 0; i < vector.size(); ++i) {
        this->vVertices[i] = vector[i];
    }
}

template<class V, class C>
Size MeshPolyline<V,C>::vertexNumber() const
{
    return vVertices.size();
}

template<class V, class C>
void MeshPolyline<V,C>::resizeVertexNumber(Size vertexNumber)
{
    assert(vertexNumber >= 2 && "Polyline must be composed of at least 2 vertices.");
    vVertices.resize(vertexNumber, MAX_INDEX);
}

template<class V, class C>
void MeshPolyline<V,C>::insertVertex(VertexId vId)
{
    this->vVertices.push_back(vId);
}

template<class V, class C>
void MeshPolyline<V,C>::insertVertex(const Index& pos, VertexId vId)
{
    assert(pos <= this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.insert(this->vVertices.begin() + pos, vId);
}

template<class V, class C>
void MeshPolyline<V,C>::insertVertex(const Vertex& vertex)
{
    insertVertex(vertex);
}

template<class V, class C>
void MeshPolyline<V,C>::insertVertex(const Index& pos, const Vertex& vertex)
{
    insertVertex(pos, vertex.id());
}

template<class V, class C>
void MeshPolyline<V,C>::eraseBackVertex()
{
    this->vVertices.resize(this->vVertices.size() - 1);
}

template<class V, class C>
void MeshPolyline<V,C>::eraseVertex(const Index& pos)
{
    assert(pos < this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.erase(this->vVertices.begin() + pos);
}

template<class V, class C>
void MeshPolyline<V,C>::eraseVertex(const Vertex& vertex)
{
    eraseVertex(vertex.id());
}

template<class V, class C>
std::vector<typename MeshPolyline<V,C>::VertexId>& MeshPolyline<V,C>::vertexIds()
{
    return vVertices;
}

template<class V, class C>
const std::vector<typename MeshPolyline<V,C>::VertexId>& MeshPolyline<V,C>::vertexIds() const
{
    return vVertices;
}

template<class V, class C>
typename MeshPolyline<V,C>::VertexId& MeshPolyline<V,C>::vertexId(const Index& pos)
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    return vVertices[pos];
}

template<class V, class C>
const typename MeshPolyline<V,C>::VertexId& MeshPolyline<V,C>::vertexId(const Index& pos) const
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    return vVertices[pos];
}

template<class V, class C>
void MeshPolyline<V,C>::setVertexId(const Index& pos, VertexId vId)
{
    assert(pos < vVertices.size() && "Index exceed the polyline dimension.");

    vVertices[pos] = vId;
}

template<class V, class C>
void MeshPolyline<V,C>::setVertex(const Index& pos, const Vertex& vertex)
{
    setVertexId(pos, vertex.id());
}


template<class V, class C>
typename MeshPolyline<V,C>::VertexId& MeshPolyline<V,C>::nextVertexId(const Index& pos)
{
    assert(pos <= vVertices.size() - 1 && "Index exceed the polyline dimension.");

    return vVertices[(pos + 1) % vVertices.size()];
}

template<class V, class C>
const typename MeshPolyline<V,C>::VertexId& MeshPolyline<V,C>::nextVertexId(const Index& pos) const
{
    assert(pos < vVertices.size() - 1 && "Index exceed the polyline dimension.");

    return vVertices[(pos + 1)];
}

template<class V, class C>
void MeshPolyline<V,C>::setNextVertexId(const Index& pos, VertexId vId)
{
    assert(pos < vVertices.size() - 1 && "Index exceed the polyline dimension.");

    vVertices[(pos + 1)] = vId;
}

template<class V, class C>
void MeshPolyline<V,C>::setNextVertex(const Index& pos, const Vertex& vertex)
{
    setNextVertexId(pos, vertex.id());
}

template<class V, class C>
template<class T, typename... Ts>
void MeshPolyline<V,C>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V, class C>
void MeshPolyline<V,C>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    vVertices[pos] = vertex.id();
}

template<class V, class C>
void MeshPolyline<V,C>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    vVertices[pos] = vertexId;
}

template<class V, class C>
void MeshPolyline<V,C>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos >= 2 && "Polylines must be composed of at least 2 vertices.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V, class C>
typename MeshPolyline<V,C>::PolylineColor& MeshPolyline<V,C>::color()
{
    return vColor;
}

template<class V, class C>
const typename MeshPolyline<V,C>::PolylineColor& MeshPolyline<V,C>::color() const
{
    return vColor;
}

template<class V, class C>
void MeshPolyline<V,C>::setColor(const PolylineColor& color)
{
    vColor = color;
}

template<class V, class C>
std::ostream& operator<<(std::ostream& output, const MeshPolyline<V,C>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshPolyline<V,C>::VertexId& vId : face.vertexIds()) {
        if (vId == MAX_INDEX) {
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
