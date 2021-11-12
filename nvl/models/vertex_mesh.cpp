#include "vertex_mesh.h"

namespace nvl {

template<class V>
VertexMesh<V>::VertexMesh()
{
    this->vType = MeshType::VERTEX;
}

template<class V>
void VertexMesh<V>::compactAll()
{
    this->compactVertices();
}

template<class V>
std::vector<typename VertexMesh<V>::VertexId> VertexMesh<V>::compactVertices()
{
    std::vector<Index> vMap = this->vVertices.compact();

    //Update vertex ids
    VertexId newVertexId = 0;
    for (Vertex& vertex : this->vertices()) {
        vertex.setId(newVertexId);
        ++newVertexId;
    }

    return vMap;
}

template<class V>
void VertexMesh<V>::clear()
{
    this->clearVertices();
}

template<class V>
std::ostream& operator<<(std::ostream& output, const VertexMesh<V>& vertexMesh)
{
    output << "Vertices: " << vertexMesh.vertexNumber() << std::endl;
    for (const typename VertexMesh<V>::Vertex& vertex : vertexMesh.vertices()) {
        output << vertex << std::endl;
    }

    return output;
}

}
