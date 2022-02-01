/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_vertex_color_handler.h"

namespace nvl {

template<class V, class C>
MeshVertexColorHandler<V,C>::MeshVertexColorHandler()
{

}

template<class V, class C>
typename MeshVertexColorHandler<V,C>::Color& MeshVertexColorHandler<V,C>::vertexColor(const VertexId& id)
{
    assert(id < vVertexColors.realSize() && "Vertex colors not enabled or not properly initialized.");

    return vVertexColors[id];
}

template<class V, class C>
const typename MeshVertexColorHandler<V,C>::Color& MeshVertexColorHandler<V,C>::vertexColor(const VertexId& id) const
{
    assert(id < vVertexColors.realSize() && "Vertex colors not enabled or not properly initialized.");

    return vVertexColors[id];
}

template<class V, class C>
void MeshVertexColorHandler<V,C>::setVertexColor(const VertexId& id, const Color& color)
{
    assert(id < vVertexColors.realSize() && "Vertex colors not enabled or not properly initialized.");

    vVertexColors[id] = color;
}

template<class V, class C>
typename MeshVertexColorHandler<V,C>::Color& MeshVertexColorHandler<V,C>::vertexColor(const Vertex& vertex)
{
    return vertexColor(vertex.id());
}

template<class V, class C>
const typename MeshVertexColorHandler<V,C>::Color& MeshVertexColorHandler<V,C>::vertexColor(const Vertex& vertex) const
{
    return vertexColor(vertex.id());
}

template<class V, class C>
void MeshVertexColorHandler<V,C>::setVertexColor(const Vertex& vertex, const Color& color)
{
    setVertexColor(vertex.id(), color);
}

}
