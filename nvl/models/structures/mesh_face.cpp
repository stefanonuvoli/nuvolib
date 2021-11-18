#include "mesh_face.h"

namespace nvl {

template<class V, class N, class M, class C>
MeshFace<V,N,M,C>::MeshFace() :
    vId(NULL_ID)
{

}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::Id& MeshFace<V,N,M,C>::id()
{
    return vId;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::Id& MeshFace<V,N,M,C>::id() const
{
    return vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setId(const Id& id)
{
    vId = id;
}

template<class V, class N, class M, class C>
Size MeshFace<V,N,M,C>::vertexNumber() const
{
    return vVertexIds.size();
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::VertexContainer& MeshFace<V,N,M,C>::vertexIds()
{
    return vVertexIds;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::VertexContainer& MeshFace<V,N,M,C>::vertexIds() const
{
    return vVertexIds;
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::vertexId(const Index& pos)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[pos];
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::vertexId(const Index& pos) const
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[pos];
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setVertexId(const Index& pos, VertexId vId)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    vVertexIds[pos] = vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setVertex(const Index& pos, const Vertex& vertex)
{
    setVertexId(pos, vertex.id());
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::nextVertexId(const Index& pos)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[(pos + 1) % vVertexIds.size()];
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::nextVertexId(const Index& pos) const
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[(pos + 1) % vVertexIds.size()];
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setNextVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    vVertexIds[(pos + 1) % vVertexIds.size()] = vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setNextVertex(const Index& pos, const Vertex& vertex)
{
    setNextVertexId(pos, vertex.id());
}

template<class V, class N, class M, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,N,M,C>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshFace<V,N,M,C>::VertexId& vId : face.vertexIds()) {
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
