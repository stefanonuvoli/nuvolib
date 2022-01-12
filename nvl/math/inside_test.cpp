/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "inside_test.h"

#include <nvl/math/barycentric_coordinates.h>

#include <nvl/math/comparisons.h>

namespace nvl {

/**
 * @brief Test if point is inside a triangle
 * @param p1 Triangle point 1
 * @param p2 Triangle point 2
 * @param p3 Triangle point 3
 * @param point Point
 * @param strictly Check if the point is strictly inside
 * @return True if the point lies inside, false otherwise
 */
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
