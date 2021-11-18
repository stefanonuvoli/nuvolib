#include "mesh_array_face.h"

namespace nvl {

template<class V, class N, class M, Size D>
MeshArrayFace<V,N,M,D>::MeshArrayFace() : MeshFace<V,N,M,std::array<typename V::Id,D>>()
{

}

template<class V, class N, class M, Size D>
template<typename... Ts>
MeshArrayFace<V,N,M,D>::MeshArrayFace(Ts... vertices) : MeshArrayFace<V,N,M,D>()
{
    setVertexIds(vertices...);
}

template<class V, class N, class M, Size D>
template<class T, typename... Ts>
void MeshArrayFace<V,N,M,D>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) == D - 1, "Wrong number of elements in face array setter.");
    assert(sizeof...(vertices) == D - 1 && "Wrong number of elements in face array setter.");
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V, class N, class M, Size D>
void MeshArrayFace<V,N,M,D>::setVertexIds(const std::vector<VertexId>& vector)
{
    assert(vector.size() == D && "Vector contains a different number of coordinates for the face array setter.");
    for (Index i = 0; i < vector.size(); ++i) {
        this->vVertexIds[i] = vector[i];
    }
}

template<class V, class N, class M, Size D>
template<class T, typename... Ts>
void MeshArrayFace<V,N,M,D>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V, class N, class M, Size D>
void MeshArrayFace<V,N,M,D>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    this->vVertexIds[pos] = vertex.id();
}

template<class V, class N, class M, Size D>
void MeshArrayFace<V,N,M,D>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    this->vVertexIds[pos] = vertexId;
}

template<class V, class N, class M, Size D>
void MeshArrayFace<V,N,M,D>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos == D && "Wrong number of elements in face array setter.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V, class N, class M, Size D>
std::ostream& operator<<(std::ostream& output, const MeshArrayFace<V,N,M,D>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshArrayFace<V,N,M,D>::VertexId& vId : face.vertexIds()) {
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
