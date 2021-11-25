#include "mesh_face.h"

namespace nvl {

template<class V, class C>
MeshFace<V,C>::MeshFace() :
    vId(NULL_ID)
{

}

template<class V, class C>
typename MeshFace<V,C>::Id& MeshFace<V,C>::id()
{
    return vId;
}

template<class V, class C>
const typename MeshFace<V,C>::Id& MeshFace<V,C>::id() const
{
    return vId;
}

template<class V, class C>
void MeshFace<V,C>::setId(const Id& id)
{
    vId = id;
}

template<class V, class C>
Size MeshFace<V,C>::vertexNumber() const
{
    return vVertexIds.size();
}

template<class V, class C>
typename MeshFace<V,C>::Container& MeshFace<V,C>::vertexIds()
{
    return vVertexIds;
}

template<class V, class C>
const typename MeshFace<V,C>::Container& MeshFace<V,C>::vertexIds() const
{
    return vVertexIds;
}

template<class V, class C>
void MeshFace<V,C>::setVertexIds(const Container& container)
{
    this->vVertexIds = container;
}

template<class V, class C>
typename MeshFace<V,C>::VertexId& MeshFace<V,C>::vertexId(const Index& pos)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[pos];
}

template<class V, class C>
const typename MeshFace<V,C>::VertexId& MeshFace<V,C>::vertexId(const Index& pos) const
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[pos];
}

template<class V, class C>
void MeshFace<V,C>::setVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    vVertexIds[pos] = vId;
}

template<class V, class C>
void MeshFace<V,C>::setVertex(const Index& pos, const Vertex& vertex)
{
    setVertexId(pos, vertex.id());
}

template<class V, class C>
typename MeshFace<V,C>::VertexId& MeshFace<V,C>::nextVertexId(const Index& pos)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[(pos + 1) % vVertexIds.size()];
}

template<class V, class C>
const typename MeshFace<V,C>::VertexId& MeshFace<V,C>::nextVertexId(const Index& pos) const
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    return vVertexIds[(pos + 1) % vVertexIds.size()];
}

template<class V, class C>
void MeshFace<V,C>::setNextVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertexIds.size() && "Index exceed the face dimension.");

    vVertexIds[(pos + 1) % vVertexIds.size()] = vId;
}

template<class V, class C>
void MeshFace<V,C>::setNextVertex(const Index& pos, const Vertex& vertex)
{
    setNextVertexId(pos, vertex.id());
}

template<class V, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,C>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshFace<V,C>::VertexId& vId : face.vertexIds()) {
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
