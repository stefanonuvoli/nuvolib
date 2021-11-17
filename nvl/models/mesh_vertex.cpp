#include "mesh_vertex.h"

namespace nvl {

template<class P, class N, class U, class C>
MeshVertex<P,N,U,C>::MeshVertex() :
    vId(NULL_ID)
{

}

template<class P, class N, class U, class C>
template<typename... Ts>
MeshVertex<P,N,U,C>::MeshVertex(Ts... coordinates) : MeshVertex()
{
    setPoint(coordinates...);
}

template<class P, class N, class U, class C>
typename MeshVertex<P,N,U,C>::Id& MeshVertex<P,N,U,C>::id()
{
    return vId;
}

template<class P, class N, class U, class C>
const typename MeshVertex<P,N,U,C>::Id& MeshVertex<P,N,U,C>::id() const
{
    return vId;
}

template<class P, class N, class U, class C>
void MeshVertex<P,N,U,C>::setId(const Id& id)
{
    vId = id;
}

template<class P, class N, class U, class C>
typename MeshVertex<P,N,U,C>::Point& MeshVertex<P,N,U,C>::point()
{
    return vPoint;
}

template<class P, class N, class U, class C>
const typename MeshVertex<P,N,U,C>::Point& MeshVertex<P,N,U,C>::point() const
{
    return vPoint;
}

template<class P, class N, class U, class C>
void MeshVertex<P,N,U,C>::setPoint(const std::vector<Scalar>& vector)
{
    assert(vector.size() == P::RowsAtCompileTime && "Vector contains a different number of coordinates.");
    for (Index i = 0; i < vector.size(); ++i) {
        vPoint(static_cast<EigenId>(i)) = vector[i];
    }
}

template<class P, class N, class U, class C>
template<class T, typename... Ts>
void MeshVertex<P,N,U,C>::setPoint(const T& coordinate, Ts... coordinates)
{
    setPointVariadicHelper(0, coordinate, coordinates...);
}

template<class P, class N, class U, class C>
template<typename T, typename... Ts>
void MeshVertex<P,N,U,C>::setPointVariadicHelper(EigenId pos, const T& coordinate, Ts... coordinates)
{
    assert(pos < P::RowsAtCompileTime && "Index exceed the point dimension.");
    setPointVariadicBase(pos, coordinate);
    setPointVariadicHelper(pos, coordinates...);
}

template<class P, class N, class U, class C>
void MeshVertex<P,N,U,C>::setPointVariadicBase(EigenId& pos, const Scalar& coordinate)
{
    assert(pos < P::RowsAtCompileTime && "Index exceed the point dimension.");
    vPoint(pos) = coordinate;
    ++pos;
}

template<class P, class N, class U, class C>
void MeshVertex<P,N,U,C>::setPointVariadicBase(EigenId& pos, const Point& point)
{
    assert(pos == 0 && "You can insert the point as a single parameter.");
    vPoint = point;
    pos = P::RowsAtCompileTime;
}

template<class P, class N, class U, class C>
void MeshVertex<P,N,U,C>::setPointVariadicHelper(EigenId pos)
{
    assert(pos == P::RowsAtCompileTime && "Index exceed the point dimension.");
    NVL_SUPPRESS_UNUSEDVARIABLE(pos);
}

template<class P, class N, class U, class C>
std::ostream& operator<<(std::ostream& output, const MeshVertex<P,N,U,C>& vertex)
{
    output << "[" << vertex.id() << "]\t" << vertex.point().transpose();
    return output;
}

}
