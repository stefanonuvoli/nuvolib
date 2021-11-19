#include "mesh_vector_face.h"

namespace nvl {

template<class V>
MeshVectorFace<V>::MeshVectorFace() : MeshFace<V,std::vector<typename V::Id>>()
{

}

template<class V>
template<typename... Ts>
MeshVectorFace<V>::MeshVectorFace(Ts... vertices) : MeshVectorFace<V>()
{
    setVertexIds(vertices...);
}

template<class V>
template<class T, typename... Ts>
void MeshVectorFace<V>::setVertexIds(const T& vertex, Ts... vertices)
{
    static_assert(sizeof...(vertices) >= 2, "Faces must be composed of at least 3 vertices.");
    this->vVertexIds.resize(sizeof...(vertices) + 1);
    setVertexIdsVariadicHelper(0, vertex, vertices...);
}

template<class V>
void MeshVectorFace<V>::setVertexIds(const std::vector<VertexId>& vector)
{
    assert(vector.size() >= 3 && "Faces must be composed of at least 3 vertices.");

    this->vVertexIds.resize(vector.size());
    for (Index i = 0; i < vector.size(); ++i) {
        this->vVertexIds[i] = vector[i];
    }
}

template<class V>
void MeshVectorFace<V>::resizeVertexNumber(Size vertexNumber)
{
    assert(vertexNumber >= 3 && "Faces must be composed of at least 3 vertices.");
    this->vVertexIds.resize(vertexNumber, NULL_ID);
}

template<class V>
void MeshVectorFace<V>::insertVertex(VertexId vId)
{
    this->vVertexIds.push_back(vId);
}

template<class V>
void MeshVectorFace<V>::insertVertex(const Index& pos, const VertexId& vId)
{
    assert(pos <= this->vVertexIds.size() && "Index exceed the face dimension.");

    this->vVertexIds.insert(this->vVertexIds.begin() + pos, vId);
}

template<class V>
void MeshVectorFace<V>::insertVertex(const Vertex& vertex)
{
    insertVertex(vertex);
}

template<class V>
void MeshVectorFace<V>::insertVertex(const Index& pos, const Vertex& vertex)
{
    insertVertex(pos, vertex.id());
}

template<class V>
void MeshVectorFace<V>::eraseLastVertex()
{
    this->vVertexIds.resize(this->vVertexIds.size() - 1);
}

template<class V>
void MeshVectorFace<V>::eraseVertex(const Index& pos)
{
    assert(pos < this->vVertexIds.size() && "Index exceed the face dimension.");

    this->vVertexIds.erase(this->vVertexIds.begin() + pos);
}

template<class V>
void MeshVectorFace<V>::eraseVertex(const Vertex& vertex)
{
    eraseVertex(vertex.id());
}

template<class V>
template<class T, typename... Ts>
void MeshVectorFace<V>::setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices)
{
    setVertexIdsVariadicBase(pos, vertex);
    setVertexIdsVariadicHelper(pos+1, vertices...);
}

template<class V>
void MeshVectorFace<V>::setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex)
{
    this->vVertexIds[pos] = vertex.id();
}

template<class V>
void MeshVectorFace<V>::setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId)
{
    this->vVertexIds[pos] = vertexId;
}

template<class V>
void MeshVectorFace<V>::setVertexIdsVariadicHelper(const Index& pos)
{
    assert(pos >= 3 && "Faces must be composed of at least 3 vertices.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class V>
std::ostream& operator<<(std::ostream& output, const MeshVectorFace<V>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshVectorFace<V>::VertexId& vId : face.vertexIds()) {
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
