#ifndef NVL_MATH_TRIANGULATE_H
#define NVL_MATH_TRIANGULATE_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

#include <vector>

namespace nvl {

template<EigenId D, class T>
std::vector<std::vector<Index>> triangulateConvexPolygon(const std::vector<Point<T,D>>& points);

}

#include "triangulate.cpp"

#endif // NVL_MATH_RIANGULATE_H
