#ifndef NVL_MODELS_MESH_FACE_H
#define NVL_MODELS_MESH_FACE_H

#include <nvl/nuvolib.h>

#include <array>
#include <vector>
#include <ostream>

namespace nvl {

template<class V, class N, class M, class C>
class MeshFace
{

public:

    /* Typedefs */

    typedef Index FaceId;

    typedef V Vertex;
    typedef typename V::VertexId VertexId;

    typedef N FaceNormal;

    typedef typename Vertex::VertexNormal VertexNormal;
    typedef typename Vertex::UV UV;

    typedef M Material;
    typedef typename Material::MaterialId MaterialId;
    typedef typename Material::Color MaterialColor;

    typedef C Container;


    /* Constructors */

    explicit MeshFace();


    /* Methods */

    FaceId& id();
    const FaceId& id() const;
    void setId(const FaceId& id);

    Size vertexNumber() const;

    Container& vertexIds();
    const Container& vertexIds() const;

    VertexId& vertexId(const Index& pos);
    const VertexId& vertexId(const Index& pos) const;
    void setVertexId(const Index& pos, VertexId vId);
    void setVertex(const Index& pos, const Vertex& vId);

    VertexId& nextVertexId(const Index& pos);
    const VertexId& nextVertexId(const Index& pos) const;
    void setNextVertexId(const Index& pos, const VertexId& vId);
    void setNextVertex(const Index& pos, const Vertex& vId);

    FaceNormal& normal();
    const FaceNormal& normal() const;
    void setNormal(const FaceNormal& normalId);

    bool hasWedgeVertexNormals() const;
    const std::vector<VertexNormal>& wedgeVertexNormals() const;
    void setWedgeVertexNormals(const std::vector<VertexNormal>& wedgeVertexNormals);
    void clearWedgeVertexNormals();

    bool hasWedgeUV() const;
    const std::vector<UV>& wedgeUV() const;
    void setWedgeUV(const std::vector<UV>& wedgeUV);
    void clearWedgeUV();

    MaterialId& materialId();
    const MaterialId& materialId() const;
    void setMaterialId(const MaterialId& materialId);


protected:

    FaceId vId;

    Container vVertices;

    FaceNormal vNormal;

    std::vector<VertexNormal> vWedgeVertexNormals;
    std::vector<UV> vWedgeUV;

    MaterialId vFaceMaterialId;
};


template<class V, class N, class M, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,N,M,C>& face);

}

#include "mesh_face.cpp"

#endif // NVL_MODELS_MESH_FACE_H
