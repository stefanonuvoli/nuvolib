#include "mesh_vertex_uv_handler.h"

namespace nvl {

template<class V, class U>
MeshVertexUVHandler<V,U>::MeshVertexUVHandler()
{

}

template<class V, class U>
typename MeshVertexUVHandler<V,U>::UV& MeshVertexUVHandler<V,U>::vertexUV(const VertexId& id)
{
    assert(id < vVertexUVs.realSize() && "Vertex uvs not enabled or not properly initialized.");

    return vVertexUVs[id];
}

template<class V, class U>
const typename MeshVertexUVHandler<V,U>::UV& MeshVertexUVHandler<V,U>::vertexUV(const VertexId& id) const
{
    assert(id < vVertexUVs.realSize() && "Vertex uvs not enabled or not properly initialized.");

    return vVertexUVs[id];
}

template<class V, class U>
void MeshVertexUVHandler<V,U>::setVertexUV(const VertexId& id, const UV& uv)
{
    assert(id < vVertexUVs.realSize() && "Vertex uvs not enabled or not properly initialized.");

    vVertexUVs[id] = uv;
}

template<class V, class U>
typename MeshVertexUVHandler<V,U>::UV& MeshVertexUVHandler<V,U>::vertexUV(const Vertex& vertex)
{
    return vertexUV(vertex.id());
}

template<class V, class U>
const typename MeshVertexUVHandler<V,U>::UV& MeshVertexUVHandler<V,U>::vertexUV(const Vertex& vertex) const
{
    return vertexUV(vertex.id());
}

template<class V, class U>
void MeshVertexUVHandler<V,U>::setVertexUV(const Vertex& vertex, const UV& uv)
{
    setVertexUV(vertex.id(), uv);
}

}
