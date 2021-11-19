#ifndef NVL_MODELS_MESH_POLYLINE_H
#define NVL_MODELS_MESH_POLYLINE_H

#include <nvl/nuvolib.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class V>
class MeshPolyline
{

public:

    /* Typedefs */

    typedef Index Id;

    typedef V Vertex;
    typedef typename V::Id VertexId;

    typedef std::vector<Index> Container;


    /* Constructors */

    explicit MeshPolyline();
    template<typename... Ts>
    explicit MeshPolyline(Ts... vertexIds);


    /* Methods */

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    template<class T, typename... Ts>
    void setVertexIds(const T& vertex, Ts... vertexIds);
    void setVertexIds(const Container& vector);

    Size vertexNumber() const;
    void resizeVertexNumber(Size vertexNumber);

    void insertVertex(VertexId vId);
    void insertVertex(const Index& pos, const VertexId& vId);
    void insertVertex(const Vertex& vId);
    void insertVertex(const Index& pos, const Vertex& vertex);

    void eraseBackVertex();
    void eraseVertex(const Index& pos);
    void eraseVertex(const Vertex& vertex);

    Container& vertexIds();
    const Container& vertexIds() const;

    VertexId& vertexId(const Index& pos);
    const VertexId& vertexId(const Index& pos) const;
    void setVertexId(const Index& pos, const VertexId& vId);
    void setVertex(const Index& pos, const Vertex& vertex);

    VertexId& nextVertexId(const Index& pos);
    const VertexId& nextVertexId(const Index& pos) const;
    void setNextVertexId(const Index& pos, const VertexId& vId);
    void setNextVertex(const Index& pos, const Vertex& vertex);

protected:

    Size vId;
    Container vVertices;

private:

    template<class T, typename... Ts>
    void setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertexIds);
    void setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex);
    void setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId);
    void setVertexIdsVariadicHelper(const Index& pos);
};

template<class V>
std::ostream& operator<<(std::ostream& output, const MeshPolyline<V>& polyline);

}

#include "mesh_polyline.cpp"

#endif // NVL_MODELS_MESH_POLYLINE_H
