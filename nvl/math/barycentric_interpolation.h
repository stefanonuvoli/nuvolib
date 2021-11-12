#ifndef NVL_MATH_BARYCENTRIC_INTERPOLATION_H
#define NVL_MATH_BARYCENTRIC_INTERPOLATION_H

#include <nvl/math/point.h>

#include <vector>

namespace nvl {

/* Triangles */

template<class V, class R>
V barycentricInterpolation(
        const V& v1,
        const V& v2,
        const V& v3,
        const std::vector<R>& bc);

template<class T, class V, EigenId D, class R = T>
V barycentricInterpolation(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        const V& v1,
        const V& v2,
        const V& v3,
        const bool useClosestPoint = false);


/* Polygons */

template<class T, class V, EigenId D, class R = T>
V barycentricInterpolationBarycenterSubdivision(
        const std::vector<Point<T,D>>& polygon,
        const Point<T,D>& point,
        const std::vector<V>& values,
        const bool useClosestPoint = false);

template<class T, class V, EigenId D, class R = T>
V barycentricInterpolationDiagonalSubdivision(
        const std::vector<Point<T,D>>& polygon,
        const Point<T,D>& point,
        const std::vector<V>& values,
        const bool useClosestPoint = false);

}

#include "barycentric_interpolation.cpp"

#endif // NVL_MATH_BARYCENTRIC_INTERPOLATION_H
