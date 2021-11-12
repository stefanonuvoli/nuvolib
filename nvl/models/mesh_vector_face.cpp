#include "mesh_vector_face.h"

namespace nvl {

template<class V, class N, class M>
MeshVectorFace<V,N,M>::MeshVectorFace() : MeshFace<V,N,M,std::vector<typename V::VertexId>>()
{

}

template<class V, class N, class M>
template<typename... Ts>
MeshVectorFace<V,N,M>::MeshVectorFace(Ts... vertices) : MeshVectorFace<V,N,M>()
{
    setVertexIds(vertices...);
}

template<class V, class N, class M>
template<class T, typename... Ts>
void MeshVectorFace<V,N,M>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) >= 2, "Faces must be composed of at least 3 vertices.");
    this->vVertices.resize(sizeof...(vertices) + 1);
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::setVertexIds(const std::vector<VertexId>& vector)
{
    assert(vector.size() >= 3 && "Faces must be composed of at least 3 vertices.");

    this->vVertices.resize(vector.size());
    for (Index i = 0; i < vector.size(); ++i) {
        this->vVertices[i] = vector[i];
    }
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::resizeVertexNumber(Size vertexNumber)
{
    assert(vertexNumber >= 3 && "Faces must be composed of at least 3 vertices.");
    this->vVertices.resize(vertexNumber, MAX_INDEX);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::insertVertex(VertexId vId)
{
    this->vVertices.push_back(vId);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::insertVertex(const Index& pos, VertexId vId)
{
    assert(pos <= this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.insert(this->vVertices.begin() + pos, vId);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::insertVertex(const Vertex& vertex)
{
    insertVertex(vertex);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::insertVertex(const Index& pos, const Vertex& vertex)
{
    insertVertex(pos, vertex.id());
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::eraseLastVertex()
{
    this->vVertices.resize(this->vVertices.size() - 1);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::eraseVertex(const Index& pos)
{
    assert(pos < this->vVertices.size() && "Index exceed the face dimension.");

    this->vVertices.erase(this->vVertices.begin() + pos);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::eraseVertex(const Vertex& vertex)
{
    eraseVertex(vertex.id());
}

template<class V, class N, class M>
template<class T, typename... Ts>
void MeshVectorFace<V,N,M>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    this->vVertices[pos] = vertex.id();
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    this->vVertices[pos] = vertexId;
}

template<class V, class N, class M>
void MeshVectorFace<V,N,M>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos >= 3 && "Faces must be composed of at least 3 vertices.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V, class N, class M>
std::ostream& operator<<(std::ostream& output, const MeshVectorFace<V,N,M>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshVectorFace<V,N,M>::VertexId& vId : face.vertexIds()) {
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
