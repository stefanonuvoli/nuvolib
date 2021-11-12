#include "mesh_face.h"

namespace nvl {

template<class V, class N, class M, class C>
MeshFace<V,N,M,C>::MeshFace() : vId(MAX_INDEX), vNormal(FaceNormal(0, 0, 0)), vFaceMaterialId(MAX_INDEX)
{

}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::FaceId& MeshFace<V,N,M,C>::id()
{
    return vId;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::FaceId& MeshFace<V,N,M,C>::id() const
{
    return vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setId(const FaceId& id)
{
    vId = id;
}

template<class V, class N, class M, class C>
Size MeshFace<V,N,M,C>::vertexNumber() const
{
    return vVertices.size();
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::Container& MeshFace<V,N,M,C>::vertexIds()
{
    return vVertices;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::Container& MeshFace<V,N,M,C>::vertexIds() const
{
    return vVertices;
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::vertexId(const Index& pos)
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    return vVertices[pos];
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::vertexId(const Index& pos) const
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    return vVertices[pos];
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setVertexId(const Index& pos, VertexId vId)
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    vVertices[pos] = vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setVertex(const Index& pos, const Vertex& vertex)
{
    setVertexId(pos, vertex.id());
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::nextVertexId(const Index& pos)
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    return vVertices[(pos + 1) % vVertices.size()];
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::VertexId& MeshFace<V,N,M,C>::nextVertexId(const Index& pos) const
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    return vVertices[(pos + 1) % vVertices.size()];
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setNextVertexId(const Index& pos, const VertexId& vId)
{
    assert(pos < vVertices.size() && "Index exceed the face dimension.");

    vVertices[(pos + 1) % vVertices.size()] = vId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setNextVertex(const Index& pos, const Vertex& vertex)
{
    setNextVertexId(pos, vertex.id());
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::FaceNormal& MeshFace<V,N,M,C>::normal()
{
    return vNormal;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::FaceNormal& MeshFace<V,N,M,C>::normal() const
{
    return vNormal;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setNormal(const FaceNormal& normal)
{
    vNormal = normal;
}

template<class V, class N, class M, class C>
bool MeshFace<V,N,M,C>::hasWedgeVertexNormals() const
{
    return !vWedgeVertexNormals.empty();
}


template<class V, class N, class M, class C>
const std::vector<typename MeshFace<V,N,M,C>::VertexNormal>& MeshFace<V,N,M,C>::wedgeVertexNormals() const
{
    return vWedgeVertexNormals;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setWedgeVertexNormals(const std::vector<VertexNormal>& wedgeVertexNormals)
{
    assert(vVertices.size() == wedgeVertexNormals.size() && "Custom vertex normals must as many as the number of vertices.");
    vWedgeVertexNormals = wedgeVertexNormals;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::clearWedgeVertexNormals()
{
    vWedgeVertexNormals.clear();
}

template<class V, class N, class M, class C>
bool MeshFace<V,N,M,C>::hasWedgeUV() const
{
    return !vWedgeUV.empty();
}

template<class V, class N, class M, class C>
const std::vector<typename MeshFace<V,N,M,C>::UV>& MeshFace<V,N,M,C>::wedgeUV() const
{
    return vWedgeUV;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setWedgeUV(const std::vector<UV>& wedgeUV)
{
    assert(vVertices.size() == wedgeUV.size() && "Custom UV coords must as many as the number of vertices.");
    vWedgeUV = wedgeUV;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::clearWedgeUV()
{
    vWedgeUV.clear();
}

template<class V, class N, class M, class C>
typename MeshFace<V,N,M,C>::MaterialId& MeshFace<V,N,M,C>::materialId()
{
    return vFaceMaterialId;
}

template<class V, class N, class M, class C>
const typename MeshFace<V,N,M,C>::MaterialId& MeshFace<V,N,M,C>::materialId() const
{
    return vFaceMaterialId;
}

template<class V, class N, class M, class C>
void MeshFace<V,N,M,C>::setMaterialId(const MaterialId& faceMaterialId)
{
    vFaceMaterialId = faceMaterialId;
}

template<class V, class N, class M, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,N,M,C>& face)
{
    output << "[" << face.id() << "]\t";
    for (const typename MeshFace<V,N,M,C>::VertexId& vId : face.vertexIds()) {
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
