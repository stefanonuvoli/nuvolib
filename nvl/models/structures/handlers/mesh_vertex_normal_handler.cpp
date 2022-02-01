/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_vertex_normal_handler.h"

namespace nvl {

template<class V, class N>
MeshVertexNormalHandler<V,N>::MeshVertexNormalHandler()
{

}

template<class V, class N>
typename MeshVertexNormalHandler<V,N>::Normal& MeshVertexNormalHandler<V,N>::vertexNormal(const VertexId& id)
{
    assert(id < vVertexNormals.realSize() && "Vertex normals not enabled or not properly initialized.");

    return vVertexNormals[id];
}

template<class V, class N>
const typename MeshVertexNormalHandler<V,N>::Normal& MeshVertexNormalHandler<V,N>::vertexNormal(const VertexId& id) const
{
    assert(id < vVertexNormals.realSize() && "Vertex normals not enabled or not properly initialized.");

    return vVertexNormals[id];
}

template<class V, class N>
void MeshVertexNormalHandler<V,N>::setVertexNormal(const VertexId& id, const Normal& normal)
{
    assert(id < vVertexNormals.realSize() && "Vertex normals not enabled or not properly initialized.");

    vVertexNormals[id] = normal;
}

template<class V, class N>
typename MeshVertexNormalHandler<V,N>::Normal& MeshVertexNormalHandler<V,N>::vertexNormal(const Vertex& vertex)
{
    return vertexNormal(vertex.id());
}

template<class V, class N>
const typename MeshVertexNormalHandler<V,N>::Normal& MeshVertexNormalHandler<V,N>::vertexNormal(const Vertex& vertex) const
{
    return vertexNormal(vertex.id());
}

template<class V, class N>
void MeshVertexNormalHandler<V,N>::setVertexNormal(const Vertex& vertex, const Normal& normal)
{
    setVertexNormal(vertex.id(), normal);
}

}
