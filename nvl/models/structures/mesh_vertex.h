/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_VERTEX_H
#define NVL_MODELS_MESH_VERTEX_H

#include <nvl/nuvolib.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class P>
class MeshVertex
{

public:

    /* Typedefs */

    typedef Index Id;
    typedef P Point;
    typedef typename Point::Scalar Scalar;


    /* Constructors */

    explicit MeshVertex();
    template<typename... Ts>
    explicit MeshVertex(Ts... coordinates);


    /* Methods */

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    Point& point();
    const Point& point() const;
    template<class T, typename... Ts>
    void setPoint(const T& coordinate, Ts... coordinates);
    void setPoint(const std::vector<Scalar>& vector);


protected:

    Size vId;
    Point vPoint;


private:

    template<typename T, typename... Ts>
    void setPointVariadicHelper(EigenId pos, const T& coordinate, Ts... coordinates);
    void setPointVariadicBase(EigenId& pos, const Scalar& coordinate);
    void setPointVariadicBase(EigenId& pos, const Point& point);
    void setPointVariadicHelper(EigenId pos);
};


template<class P>
std::ostream& operator<<(std::ostream& output, const MeshVertex<P>& vector);

}

#include "mesh_vertex.cpp"

#endif // NVL_MODELS_MESH_VERTEX_H
