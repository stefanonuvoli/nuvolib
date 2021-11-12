#ifndef NVL_MODELS_MESH_VERTEX_H
#define NVL_MODELS_MESH_VERTEX_H

#include <nvl/nuvolib.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class P, class N, class U, class C>
class MeshVertex
{

public:

    /* Typedefs */

    typedef Index VertexId;

    typedef N VertexNormal;

    typedef C VertexColor;

    typedef U UV;

    typedef P Point;
    typedef typename P::Scalar Scalar;

    /* Constructors */

    explicit MeshVertex();
    template<typename... Ts>
    explicit MeshVertex(Ts... coordinates);


    /* Methods */

    VertexId& id();
    const VertexId& id() const;
    void setId(VertexId id);

    Point& point();
    const Point& point() const;
    template<class T, typename... Ts>
    void setPoint(const T& coordinate, Ts... coordinates);
    void setPoint(const std::vector<Scalar>& vector);

    VertexNormal& normal();
    const VertexNormal& normal() const;
    void setNormal(const VertexNormal& normal);

    UV& uvCoords();
    const UV& uvCoords() const;
    void setUV(const UV& uvCoords);

    VertexColor& color();
    const VertexColor& color() const;
    void setColor(const VertexColor& color);

protected:

    Size vId;
    Point vPoint;

    VertexNormal vNormal;
    UV vUV;
    VertexColor vColor;

private:

    template<typename T, typename... Ts>
    void setPointVariadicHelper(EigenId pos, const T& coordinate, Ts... coordinates);
    void setPointVariadicBase(EigenId& pos, const Scalar& coordinate);
    void setPointVariadicBase(EigenId& pos, const Point& point);
    void setPointVariadicHelper(EigenId pos);
};


template<class P, class N, class U, class C>
std::ostream& operator<<(std::ostream& output, const MeshVertex<P,N,U,C>& vector);

}

#include "mesh_vertex.cpp"

#endif // NVL_MODELS_MESH_VERTEX_H
