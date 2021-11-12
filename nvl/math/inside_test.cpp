#include "inside_test.h"

#include <nvl/math/barycentric_coordinates.h>

#include <nvl/math/comparisons.h>

namespace nvl {

template<EigenId D, class T>
bool pointInsideTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        const bool strictly)
{
    std::vector<T> bc = barycentricCoordinates(p1, p2, p3, point);

    if (strictly) {
        return (bc[0] > 0.0) && (bc[1] > 0.0) && (bc[0] + bc[1] < 1.0);
    }
    else {
        return (bc[0] >= 0.0 || epsEqual(bc[0], 0.0)) && (bc[1] >= 0 || epsEqual(bc[1], 0.0)) && (bc[0] + bc[1] <= 1 || epsEqual(bc[0] + bc[1], 1.0));
    }
}

}
