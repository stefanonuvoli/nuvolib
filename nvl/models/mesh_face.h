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

    typedef Index Id;

    typedef V Vertex;
    typedef N Normal;
    typedef M Material;
    typedef C VertexContainer;

    typedef typename Vertex::Normal VertexNormal;
    typedef typename Vertex::UV UV;
    typedef typename V::Id VertexId;


    /* Constructors */

    explicit MeshFace();


    /* Methods */

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    Size vertexNumber() const;

    VertexContainer& vertexIds();
    const VertexContainer& vertexIds() const;

    VertexId& vertexId(const Index& pos);
    const VertexId& vertexId(const Index& pos) const;
    void setVertexId(const Index& pos, VertexId vId);
    void setVertex(const Index& pos, const Vertex& vId);

    VertexId& nextVertexId(const Index& pos);
    const VertexId& nextVertexId(const Index& pos) const;
    void setNextVertexId(const Index& pos, const VertexId& vId);
    void setNextVertex(const Index& pos, const Vertex& vId);


protected:

    Id vId;

    VertexContainer vVertexIds;
};


template<class V, class N, class M, class C>
std::ostream& operator<<(std::ostream& output, const MeshFace<V,N,M,C>& face);

}

#include "mesh_face.cpp"

#endif // NVL_MODELS_MESH_FACE_H
