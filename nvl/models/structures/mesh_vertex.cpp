/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_vertex.h"

namespace nvl {

template<class P>
MeshVertex<P>::MeshVertex() :
    vId(NULL_ID)
{

}

template<class P>
template<typename... Ts>
MeshVertex<P>::MeshVertex(Ts... coordinates) : MeshVertex()
{
    setPoint(coordinates...);
}

template<class P>
typename MeshVertex<P>::Id& MeshVertex<P>::id()
{
    return vId;
}

template<class P>
const typename MeshVertex<P>::Id& MeshVertex<P>::id() const
{
    return vId;
}

template<class P>
void MeshVertex<P>::setId(const Id& id)
{
    vId = id;
}

template<class P>
typename MeshVertex<P>::Point& MeshVertex<P>::point()
{
    return vPoint;
}

template<class P>
const typename MeshVertex<P>::Point& MeshVertex<P>::point() const
{
    return vPoint;
}

template<class P>
void MeshVertex<P>::setPoint(const std::vector<Scalar>& vector)
{
    assert(vector.size() == P::RowsAtCompileTime && "Vector contains a different number of coordinates.");
    for (Index i = 0; i < vector.size(); ++i) {
        vPoint(static_cast<EigenId>(i)) = vector[i];
    }
}

template<class P>
template<class T, typename... Ts>
void MeshVertex<P>::setPoint(const T& coordinate, Ts... coordinates)
{
    setPointVariadicHelper(0, coordinate, coordinates...);
}

template<class P>
template<typename T, typename... Ts>
void MeshVertex<P>::setPointVariadicHelper(EigenId pos, const T& coordinate, Ts... coordinates)
{
    assert(pos < P::RowsAtCompileTime && "Index exceed the point dimension.");
    setPointVariadicBase(pos, coordinate);
    setPointVariadicHelper(pos, coordinates...);
}

template<class P>
void MeshVertex<P>::setPointVariadicBase(EigenId& pos, const Scalar& coordinate)
{
    assert(pos < P::RowsAtCompileTime && "Index exceed the point dimension.");
    vPoint(pos) = coordinate;
    ++pos;
}

template<class P>
void MeshVertex<P>::setPointVariadicBase(EigenId& pos, const Point& point)
{
    assert(pos == 0 && "You can insert the point as a single parameter.");
    vPoint = point;
    pos = P::RowsAtCompileTime;
}

template<class P>
void MeshVertex<P>::setPointVariadicHelper(EigenId pos)
{
    assert(pos == P::RowsAtCompileTime && "Index exceed the point dimension.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class P>
std::ostream& operator<<(std::ostream& output, const MeshVertex<P>& vertex)
{
    output << "[" << vertex.id() << "]\t" << vertex.point().transpose();
    return output;
}

}
