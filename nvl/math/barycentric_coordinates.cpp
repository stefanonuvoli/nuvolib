#include "barycentric_coordinates.h"

#include <nvl/math/comparisons.h>
#include <nvl/math/closest_point.h>

namespace nvl {

template<class T, EigenId D, class R>
std::vector<R> barycentricCoordinates(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        const bool useClosestPoint)
{
    Point<T,D> queryPoint = point;
    if (useClosestPoint) {
        queryPoint = closestPointOnTriangle(p1, p2, p3, point);
    }

    std::vector<R> bc(D);

    Vector<T,D> v0 = p1 - p3;
    Vector<T,D> v1 = p2 - p3;
    Vector<T,D> v2 = queryPoint - p3;

    R d00 = v0.dot(v0);
    R d01 = v0.dot(v1);
    R d11 = v1.dot(v1);
    R d20 = v2.dot(v0);
    R d21 = v2.dot(v1);

    R denom = d00 * d11 - d01 * d01;

    bc[0] = (d11 * d20 - d01 * d21) / denom;
    bc[1] = (d00 * d21 - d01 * d20) / denom;

    //Avoid numerical errors
    if (epsEqual(bc[0], 0.0)) {
        bc[0] = 0.0;
    }
    else if (epsEqual(bc[0], 1.0)) {
        bc[0] = 1.0;
    }

    if (epsEqual(bc[1], 0.0)) {
        bc[1] = 0.0;
    }
    else if (epsEqual(bc[1], 1.0)) {
        bc[1] = 1.0;
    }

    bc[2] = 1.0f - bc[0] - bc[1];

    return bc;
}

}
