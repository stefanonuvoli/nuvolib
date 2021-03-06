/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_array_face.h"

namespace nvl {

template<class V, Size D>
MeshArrayFace<V,D>::MeshArrayFace() : MeshFace<V,std::array<typename V::Id,D>>()
{

}

template<class V, Size D>
template<typename... Ts>
MeshArrayFace<V,D>::MeshArrayFace(Ts... vertices) : MeshArrayFace<V,D>()
{
    setVertexIds(vertices...);
}

template<class V, Size D>
template<class T, typename... Ts>
void MeshArrayFace<V,D>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) == D - 1, "Wrong number of elements in face array setter.");
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V, Size D>
void MeshArrayFace<V,D>::setVertexIds(const std::vector<VertexId>& vector)
{
    assert(vector.size() == D && "Wrong number of elements in face array setter.");
    for (Index pos = 0; pos < D; ++pos) {
        this->vVertexIds[pos] = vector[pos];
    }
}

template<class V, Size D>
void MeshArrayFace<V,D>::setVertexIds(const Container& container)
{
    MeshFace<V,Container>::setVertexIds(container);
}

template<class V, Size D>
template<class T, typename... Ts>
void MeshArrayFace<V,D>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V, Size D>
void MeshArrayFace<V,D>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    this->vVertexIds[pos] = vertex.id();
}

template<class V, Size D>
void MeshArrayFace<V,D>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    assert(pos < D && "Wrong number of elements in face array setter.");
    this->vVertexIds[pos] = vertexId;
}

template<class V, Size D>
void MeshArrayFace<V,D>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos == D && "Wrong number of elements in face array setter.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V, Size D>
std::ostream& operator<<(std::ostream& output, const MeshArrayFace<V,D>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshArrayFace<V,D>::VertexId& vId : face.vertexIds()) {
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
