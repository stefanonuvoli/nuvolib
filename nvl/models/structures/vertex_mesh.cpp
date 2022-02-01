/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vertex_mesh.h"

namespace nvl {

template<class VT>
VertexMesh<VT>::VertexMesh() :
    vVertexNormalsEnabled(false),
    vVertexColorsEnabled(false),
    vVertexUVsEnabled(false)
{
    this->vType = MeshType::VERTEX;
}

template<class VT>
typename VertexMesh<VT>::VertexId VertexMesh<VT>::addVertex(const Vertex& vertex)
{
    VertexId newVId = MeshVertexHandler<Vertex>::addVertex(vertex);
    updateComponents();
    return newVId;
}

template<class VT>
typename VertexMesh<VT>::VertexId VertexMesh<VT>::addVertex(const Point& point)
{
    VertexId newVId = MeshVertexHandler<Vertex>::addVertex(point);
    updateComponents();
    return newVId;
}

template<class VT>
template<class... Ts>
typename VertexMesh<VT>::VertexId VertexMesh<VT>::addVertex(const Ts... points)
{
    VertexId newVId = MeshVertexHandler<Vertex>::addVertex(points...);
    updateComponents();
    return newVId;
}

template<class VT>
typename VertexMesh<VT>::VertexId VertexMesh<VT>::allocateVertices(const Size& n)
{
    return allocateVertices(n, Vertex());
}

template<class VT>
typename VertexMesh<VT>::VertexId VertexMesh<VT>::allocateVertices(const Size& n, const Vertex& vertex)
{
    VertexId newVId = MeshVertexHandler<Vertex>::allocateVertices(n, vertex);
    updateComponents();
    return newVId;
}

template<class VT>
typename VertexMesh<VT>::Point& VertexMesh<VT>::vertexPoint(const VertexId& id)
{
    return MeshVertexHandler<Vertex>::vertex(id).point();
}

template<class VT>
const typename VertexMesh<VT>::Point& VertexMesh<VT>::vertexPoint(const VertexId& id) const
{
    return MeshVertexHandler<Vertex>::vertex(id).point();
}

template<class VT>
void VertexMesh<VT>::setVertexPoint(const VertexId& id, const Point& point)
{
    MeshVertexHandler<Vertex>::vertex(id).setPoint(point);
}

template<class VT>
bool VertexMesh<VT>::hasVertexNormals() const
{
    return vVertexNormalsEnabled;
}

template<class VT>
void VertexMesh<VT>::enableVertexNormals()
{
    vVertexNormalsEnabled = true;
    updateVertexNormals();
}

template<class VT>
void VertexMesh<VT>::disableVertexNormals()
{
    vVertexNormalsEnabled = false;
    updateVertexNormals();
}

template<class VT>
bool VertexMesh<VT>::hasVertexColors() const
{
    return vVertexColorsEnabled;
}

template<class VT>
void VertexMesh<VT>::enableVertexColors()
{
    vVertexColorsEnabled = true;
    updateVertexColors();
}

template<class VT>
void VertexMesh<VT>::disableVertexColors()
{
    vVertexColorsEnabled = false;
    updateVertexColors();
}

template<class VT>
bool VertexMesh<VT>::hasVertexUVs() const
{
    return vVertexUVsEnabled;
}

template<class VT>
void VertexMesh<VT>::enableVertexUVs()
{
    vVertexUVsEnabled = true;
    updateVertexUVs();
}

template<class VT>
void VertexMesh<VT>::disableVertexUVs()
{
    vVertexUVsEnabled = false;
    updateVertexUVs();
}

template<class VT>
void VertexMesh<VT>::compactAll()
{
    this->compactVertices();
}

template<class VT>
std::vector<typename VertexMesh<VT>::VertexId> VertexMesh<VT>::compactVertices()
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

template<class VT>
void VertexMesh<VT>::clearVertices()
{
    MeshVertexHandler<Vertex>::clearVertices();
    this->vVertexNormals.clear();
    this->vVertexColors.clear();
    this->vVertexUVs.clear();
}

template<class VT>
void VertexMesh<VT>::clear()
{
    this->clearVertices();
}

template<class VT>
void VertexMesh<VT>::updateComponents()
{
    updateVertexNormals();
    updateVertexColors();
    updateVertexUVs();
}

template<class VT>
void VertexMesh<VT>::updateVertexNormals()
{
    if (vVertexNormalsEnabled) {
        this->vVertexNormals.resize(this->nextVertexId(), VertexNormal::Zero());
    }
    else {
        this->vVertexNormals.clear();
    }
}

template<class VT>
void VertexMesh<VT>::updateVertexColors()
{
    if (vVertexColorsEnabled) {
        this->vVertexColors.resize(this->nextVertexId(), VertexColor(0.8, 0.8, 0.8));
    }
    else {
        this->vVertexColors.clear();
    }
}

template<class VT>
void VertexMesh<VT>::updateVertexUVs()
{
    if (vVertexUVsEnabled) {
        this->vVertexUVs.resize(this->nextVertexId(), VertexUV::Zero());
    }
    else {
        this->vVertexUVs.clear();
    }
}

template<class VT>
std::ostream& operator<<(std::ostream& output, const VertexMesh<VT>& vertexMesh)
{
    output << "Vertices: " << vertexMesh.vertexNumber() << std::endl;
    for (const typename VertexMesh<VT>::Vertex& vertex : vertexMesh.vertices()) {
        output << vertex << std::endl;
    }

    return output;
}

}
