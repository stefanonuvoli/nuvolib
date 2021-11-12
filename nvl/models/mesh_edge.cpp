#include "mesh_edge.h"

namespace nvl {

template<class V, class F>
MeshEdge<V,F>::MeshEdge() : vId(MAX_INDEX), vFaceId(MAX_INDEX)
{

}

template<class V, class F>
template<typename... Ts>
MeshEdge<V,F>::MeshEdge(Ts... vertices) : vId(MAX_INDEX), vFaceId(MAX_INDEX)
{
    setVertexIds(vertices...);
}

template<class V, class F>
typename MeshEdge<V,F>::EdgeId& MeshEdge<V,F>::id()
{
    return vId;
}

template<class V, class F>
const typename MeshEdge<V,F>::EdgeId& MeshEdge<V,F>::id() const
{
    return vId;
}

template<class V, class F>
void MeshEdge<V,F>::setId(const EdgeId& id)
{
    vId = id;
}

template<class V, class F>
template<class T, typename... Ts>
void MeshEdge<V,F>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) == 1, "Edges must be composed of exactly 2 vertices.");
    assert(sizeof...(vertices) == 1 && "Edges must be composed of exactly 2 vertices.");
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V, class F>
std::pair<typename MeshEdge<V,F>::VertexId, typename MeshEdge<V,F>::VertexId>& MeshEdge<V,F>::vertices()
{
    return vVertices;
}

template<class V, class F>
const std::pair<typename MeshEdge<V,F>::VertexId, typename MeshEdge<V,F>::VertexId>& MeshEdge<V,F>::vertices() const
{
    return vVertices;
}

template<class V, class F>
typename MeshEdge<V,F>::VertexId& MeshEdge<V,F>::firstVertexId()
{
    return vVertices.first;
}

template<class V, class F>
const typename MeshEdge<V,F>::VertexId& MeshEdge<V,F>::firstVertexId() const
{
    return vVertices.first;
}

template<class V, class F>
void MeshEdge<V,F>::setFirstVertexId(VertexId vId)
{
    vVertices.first = vId;
}

template<class V, class F>
void MeshEdge<V,F>::setFirstVertex(const Vertex& vertex)
{
    setFirstVertexId(vertex.id());
}

template<class V, class F>
typename MeshEdge<V,F>::VertexId& MeshEdge<V,F>::secondVertexId()
{
    return vVertices.second;
}

template<class V, class F>
const typename MeshEdge<V,F>::VertexId& MeshEdge<V,F>::secondVertexId() const
{
    return vVertices.second;
}

template<class V, class F>
void MeshEdge<V,F>::setSecondVertexId(VertexId vId)
{
    vVertices.second = vId;
}

template<class V, class F>
void MeshEdge<V,F>::setSecondVertex(const Vertex& vertex)
{
    setSecondVertexId(vertex.id());
}

template<class V, class F>
typename MeshEdge<V,F>::FaceId& MeshEdge<V,F>::faceId()
{
    return vFaceId;
}

template<class V, class F>
const typename MeshEdge<V,F>::FaceId& MeshEdge<V,F>::faceId() const
{
    return vFaceId;
}

template<class V, class F>
void MeshEdge<V,F>::setFaceId(const FaceId& faceId)
{
    vFaceId = faceId;
}

template<class V, class F>
void MeshEdge<V,F>::setFace(const Face& vertex)
{
    setFaceId(vertex.id());
}

template<class V, class F>
template<class T, typename... Ts>
void MeshEdge<V,F>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    assert(pos < 2 && "Edges must be composed of exactly 2 vertices.");
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V, class F>
void MeshEdge<V,F>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    assert(pos < 2 && "Edges must be composed of exactly 2 vertices.");
    if (pos == 0) {
        vVertices.first = vertex.id();
    }
    else {
        assert(pos == 1);
        vVertices.second = vertex.id();
    }
}

template<class V, class F>
void MeshEdge<V,F>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    assert(pos < 2 && "Edges must be composed of exactly 2 vertices.");
    if (pos == 0) {
        vVertices.first = vertexId;
    }
    else {
        assert(pos == 1);
        vVertices.second = vertexId;
    }
}

template<class V, class F>
void MeshEdge<V,F>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos == 2 && "Edges must be composed of exactly 2 vertices.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V, class F>
std::ostream& operator<<(std::ostream& output, const MeshEdge<V,F>& edge)
{
    output << "[" << edge.id() << "]\t";

    const typename MeshEdge<V,F>::VertexId& vId1 = edge.firstVertexId();
    if (vId1 == MAX_INDEX) {
        output << "x";
    }
    else {
        output << vId1;
    }

    output << " ";

    const typename MeshEdge<V,F>::VertexId& vId2 = edge.secondVertexId();
    vId2 = edge.secondVertexId();
    if (vId2 == MAX_INDEX) {
        output << "x";
    }
    else {
        output << vId2;
    }

    output << " - F: ";

    const typename MeshEdge<V,F>::VertexId& fId = edge.faceId();
    if (fId == MAX_INDEX) {
        output << "x";
    }
    else {
        output << fId;
    }

    return output;
}

}
