#ifndef NVL_MODELS_MESH_EDGE_H
#define NVL_MODELS_MESH_EDGE_H

#include <nvl/nuvolib.h>

#include <utility>

#include <ostream>

namespace nvl {

template<class V, class F>
class MeshEdge
{

public:

    /* Typedefs */

    typedef Index EdgeId;

    typedef V Vertex;
    typedef typename V::VertexId VertexId;

    typedef F Face;
    typedef typename F::FaceId FaceId;


    /* Constructors */

    explicit MeshEdge();
    template<typename... Ts>
    explicit MeshEdge(Ts... vertices);


    /* Methods */

    EdgeId& id();
    const EdgeId& id() const;
    void setId(const EdgeId& id);

    template<class T, typename... Ts>
    void setVertexIds(const T& vertex, Ts... vertices);

    std::pair<VertexId, VertexId>& vertices();
    const std::pair<VertexId, VertexId>& vertices() const;

    VertexId& firstVertexId();
    const VertexId& firstVertexId() const;
    void setFirstVertexId(VertexId vId);
    void setFirstVertex(const Vertex& vId);

    VertexId& secondVertexId();
    const VertexId& secondVertexId() const;
    void setSecondVertexId(VertexId vId);
    void setSecondVertex(const Vertex& vId);

    FaceId& faceId();
    const FaceId& faceId() const;
    void setFaceId(const FaceId& faceId);
    void setFace(const Face& vId);

protected:

    Size vId;

    FaceId vFaceId;

    std::pair<VertexId, VertexId> vVertices;

private:

    template<class T, typename... Ts>
    void setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertices);
    void setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex);
    void setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId);
    void setVertexIdsVariadicHelper(const Index& pos);
};

template<class V, class F>
std::ostream& operator<<(std::ostream& output, const MeshEdge<V,F>& edge);

}

#include "mesh_edge.cpp"

#endif // NVL_MODELS_MESH_EDGE_H
