#include "vertex_mesh.h"

namespace nvl {

template<class V>
VertexMesh<V>::VertexMesh() :
    vVertexNormalsEnabled(false),
    vVertexColorsEnabled(false),
    vVertexUVsEnabled(false)
{
    this->vType = MeshType::VERTEX;
}

template<class V>
typename VertexMesh<V>::VertexId VertexMesh<V>::addVertex(const Vertex& vertex)
{
    VertexId newVId = MeshVertexHandler<V>::addVertex(vertex);
    updateComponents();
    return newVId;
}

template<class V>
typename VertexMesh<V>::VertexId VertexMesh<V>::addVertex(const Point& point)
{
    VertexId newVId = MeshVertexHandler<V>::addVertex(point);
    updateComponents();
    return newVId;
}

template<class V>
template<class... Ts>
typename VertexMesh<V>::VertexId VertexMesh<V>::addVertex(const Ts... points)
{
    VertexId newVId = MeshVertexHandler<V>::addVertex(points...);
    updateComponents();
    return newVId;
}

template<class V>
typename VertexMesh<V>::VertexId VertexMesh<V>::allocateVertices(const Size& n)
{
    return allocateVertices(n, Vertex());
}

template<class V>
typename VertexMesh<V>::VertexId VertexMesh<V>::allocateVertices(const Size& n, const Vertex& vertex)
{
    VertexId newVId = MeshVertexHandler<V>::allocateVertices(n, vertex);
    updateComponents();
    return newVId;
}

template<class V>
typename VertexMesh<V>::Point& VertexMesh<V>::vertexPoint(const VertexId& id)
{
    return MeshVertexHandler<V>::vertex(id).point();
}

template<class V>
const typename VertexMesh<V>::Point& VertexMesh<V>::vertexPoint(const VertexId& id) const
{
    return MeshVertexHandler<V>::vertex(id).point();
}

template<class V>
void VertexMesh<V>::setVertexPoint(const VertexId& id, const Point& point)
{
    MeshVertexHandler<V>::vertex(id).setPoint(point);
}

template<class V>
bool VertexMesh<V>::hasVertexNormals() const
{
    return vVertexNormalsEnabled;
}

template<class V>
void VertexMesh<V>::enableVertexNormals()
{
    vVertexNormalsEnabled = true;
    updateVertexNormals();
}

template<class V>
void VertexMesh<V>::disableVertexNormals()
{
    vVertexNormalsEnabled = false;
    updateVertexNormals();
}

template<class V>
bool VertexMesh<V>::hasVertexColors() const
{
    return vVertexColorsEnabled;
}

template<class V>
void VertexMesh<V>::enableVertexColors()
{
    vVertexColorsEnabled = true;
    updateVertexColors();
}

template<class V>
void VertexMesh<V>::disableVertexColors()
{
    vVertexColorsEnabled = false;
    updateVertexColors();
}

template<class V>
bool VertexMesh<V>::hasVertexUVs() const
{
    return vVertexUVsEnabled;
}

template<class V>
void VertexMesh<V>::enableVertexUVs()
{
    vVertexUVsEnabled = true;
    updateVertexUVs();
}

template<class V>
void VertexMesh<V>::disableVertexUVs()
{
    vVertexUVsEnabled = false;
    updateVertexUVs();
}

template<class V>
void VertexMesh<V>::compactAll()
{
    this->compactVertices();
}

template<class V>
std::vector<typename VertexMesh<V>::VertexId> VertexMesh<V>::compactVertices()
{
    //Compact vertex normals
    if (this->hasVertexNormals()) {
        for (VertexId vId = 0; vId < this->nextVertexId(); vId++) {
            if (this->isVertexDeleted(vId)) {
                this->vVertexNormals.erase(vId);
            }
        }
        this->vVertexNormals.compact();
    }
    else {
        this->vVertexNormals.clear();
    }

    //Compact vertex colors
    if (this->hasVertexColors()) {
        for (VertexId vId = 0; vId < this->nextVertexId(); vId++) {
            if (this->isVertexDeleted(vId)) {
                this->vVertexColors.erase(vId);
            }
        }
        this->vVertexColors.compact();
    }
    else {
        this->vVertexColors.clear();
    }

    //Compact vertex UVs
    if (this->hasVertexUVs()) {
        for (VertexId vId = 0; vId < this->nextVertexId(); vId++) {
            if (this->isVertexDeleted(vId)) {
                this->vVertexUVs.erase(vId);
            }
        }
        this->vVertexUVs.compact();
    }
    else {
        this->vVertexUVs.clear();
    }

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
void VertexMesh<V>::clearVertices()
{
    MeshVertexHandler<V>::clearVertices();
    this->vVertexNormals.clear();
    this->vVertexColors.clear();
    this->vVertexUVs.clear();
}

template<class V>
void VertexMesh<V>::clear()
{
    this->clearVertices();
}

template<class V>
void VertexMesh<V>::updateComponents()
{
    updateVertexNormals();
    updateVertexColors();
    updateVertexUVs();
}

template<class V>
void VertexMesh<V>::updateVertexNormals()
{
    if (vVertexNormalsEnabled) {
        this->vVertexNormals.resize(this->nextVertexId(), VertexNormal::Zero());
    }
    else {
        this->vVertexNormals.clear();
    }
}

template<class V>
void VertexMesh<V>::updateVertexColors()
{
    if (vVertexColorsEnabled) {
        this->vVertexColors.resize(this->nextVertexId(), VertexColor(0.7, 0.7, 0.7));
    }
    else {
        this->vVertexColors.clear();
    }
}

template<class V>
void VertexMesh<V>::updateVertexUVs()
{
    if (vVertexUVsEnabled) {
        this->vVertexUVs.resize(this->nextVertexId(), UV::Zero());
    }
    else {
        this->vVertexUVs.clear();
    }
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
