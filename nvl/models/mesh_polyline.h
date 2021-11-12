#ifndef NVL_MODELS_MESH_POLYLINE_H
#define NVL_MODELS_MESH_POLYLINE_H

#include <nvl/nuvolib.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class V, class C>
class MeshPolyline
{

public:

    /* Typedefs */

    typedef Index PolylineId;

    typedef V Vertex;
    typedef typename V::VertexId VertexId;

    typedef C PolylineColor;


    /* Constructors */

    explicit MeshPolyline();
    template<typename... Ts>
    explicit MeshPolyline(Ts... vertexIds);


    /* Methods */

    PolylineId& id();
    const PolylineId& id() const;
    void setId(const PolylineId& id);

    template<class T, typename... Ts>
    void setVertexIds(const T& vertex, Ts... vertexIds);
    void setVertexIds(const std::vector<VertexId>& vector);

    Size vertexNumber() const;
    void resizeVertexNumber(Size vertexNumber);

    void insertVertex(VertexId vId);
    void insertVertex(const Index& pos, VertexId vId);
    void insertVertex(const Vertex& vId);
    void insertVertex(const Index& pos, const Vertex& vertex);

    void eraseBackVertex();
    void eraseVertex(const Index& pos);
    void eraseVertex(const Vertex& vertex);

    std::vector<VertexId>& vertexIds();
    const std::vector<VertexId>& vertexIds() const;

    VertexId& vertexId(const Index& pos);
    const VertexId& vertexId(const Index& pos) const;
    void setVertexId(const Index& pos, VertexId vId);
    void setVertex(const Index& pos, const Vertex& vertex);

    VertexId& nextVertexId(const Index& pos);
    const VertexId& nextVertexId(const Index& pos) const;
    void setNextVertexId(const Index& pos, VertexId vId);
    void setNextVertex(const Index& pos, const Vertex& vertex);

    PolylineColor& color();
    const PolylineColor& color() const;
    void setColor(const PolylineColor& color);

protected:

    Size vId;
    std::vector<VertexId> vVertices;

    PolylineColor vColor;

private:

    template<class T, typename... Ts>
    void setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertexIds);
    void setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex);
    void setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId);
    void setVertexIdsVariadicHelper(const Index& pos);
};

template<class V, class C>
std::ostream& operator<<(std::ostream& output, const MeshPolyline<V,C>& polyline);

}

#include "mesh_polyline.cpp"

#endif // NVL_MODELS_MESH_POLYLINE_H
