/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "barycentric_interpolation.h"

#include <nvl/math/numeric_limits.h>
#include <nvl/math/closest_point.h>
#include <nvl/math/barycentric_coordinates.h>

namespace nvl {

/**
 * @brief Barycentric interpolation in a triangle
 * @param v1 Triangle value 1
 * @param v2 Triangle value 2
 * @param v3 Triangle value 3
 * @param bc Barycentric coordinates
 * @return Interpolated value
 */
template<class V, class R>
V barycentricInterpolation(
        const V& v1,
        const V& v2,
        const V& v3,
        const std::vector<R>& bc)
{
    V p =
        v1 * bc[0] +
        v2 * bc[1] +
        v3 * bc[2];

    return p;
}

/**
 * @brief Barycentric interpolation in a triangle
 * @param p1 Triangle vertex 1
 * @param p2 Triangle vertex 2
 * @param p3 Triangle vertex 3
 * @param point Target point
 * @param v1 Triangle vertex 1
 * @param v2 Triangle vertex 2
 * @param v3 Triangle vertex 3
 * @param useClosestPoint Use closest point if the point does not lie in the triangle (default false)
 * @return Interpolated value
 */
template<class T, class V, EigenId D, class R>
V barycentricInterpolation(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        const V& v1,
        const V& v2,
        const V& v3,
        const bool useClosestPoint)
{
    const std::vector<R> bc = barycentricCoordinates(p1, p2, p3, point, useClosestPoint);
    return barycentricInterpolation(v1, v2, v3, bc);
}

/**
 * @brief Barycentric interpolation on a polygon, using barycenter subdivision
 * @param polygon Polygon vertices
 * @param point Target point
 * @param values Values for each vertex
 * @param useClosestPoint Use closest point if the point does not lie in the triangle (default false)
 * @return Interpolated value
 */
template<class T, class V, EigenId D, class R>
V barycentricInterpolationBarycenterSubdivision(
        const std::vector<Point<T,D>>& polygon,
        const Point<T,D>& point,
        const std::vector<V>& values,
        const bool useClosestPoint)
{
    //Return value
    V value;

    if (polygon.size() == 3) { //Triangle
        return barycentricInterpolation(
            polygon[0],
            polygon[1],
            polygon[2],
            point,
            values[0],
            values[1],
            values[2],
            useClosestPoint);
    }
    else { //Polygon
        assert(polygon.size() > 3);

        Point<T,D> queryPoint = point;

        //Get barycenter and average value
        Point<T,D> pB = polygon[0];
        V vB = values[0];
        for (Index j = 1; j < polygon.size(); ++j) {
            pB += polygon[j];
            vB += values[j];
        }
        pB /= polygon.size();
        vB /= polygon.size();

        //Calculate the interpolated value on the best triangle
        T minDistance = maxLimitValue<T>();
        Index bestV1;
        Index bestV2;
        for (Index j = 0; j < polygon.size(); ++j) {
            const Index& v1 = j;
            const Index& v2 = (j + 1) % polygon.size();

            Point<T,D> planeClosestPoint;
            Point<T,D> closestPoint = closestPointOnTriangle(polygon[v1], polygon[v2], pB, point, planeClosestPoint);

            T distance = (point - closestPoint).norm();
            if (distance < minDistance) {
                bestV1 = v1;
                bestV2 = v2;

                if (useClosestPoint) {
                    queryPoint = closestPointOnTriangle(polygon[v1], polygon[v2], pB, point);
                }
                else {
                    queryPoint = planeClosestPoint;
                }

                minDistance = distance;
            }
        }

        value = barycentricInterpolation(
            polygon[bestV1],
            polygon[bestV2],
            pB,
            queryPoint,
            values[bestV1],
            values[bestV2],
            vB);
    }

    return value;
}

/**
 * @brief Barycentric interpolation on a polygon, using subdivision on diagonal
 * @param polygon Polygon vertices
 * @param point Target point
 * @param values Values for each vertex
 * @param useClosestPoint Use closest point if the point does not lie in the triangle (default false)
 * @return Interpolated value
 */
template<class T, class V, EigenId D, class R>
V barycentricInterpolationDiagonalSubdivision(
        const std::vector<Point<T,D>>& polygon,
        const Point<T,D>& point,
        const std::vector<V>& values,
        const bool useClosestPoint)
{
    //Return value
    V value;

    if (polygon.size() == 3) { //Triangle
        return barycentricInterpolation(
            polygon[0],
            polygon[1],
            polygon[2],
            point,
            values[0],
            values[1],
            values[2],
            useClosestPoint);
    }
    else { //Polygon
        assert(polygon.size() > 3);        

        Point<T,D> queryPoint = point;

        //Calculate the interpolated value on the best triangle
        T minDistance = maxLimitValue<T>();
        Index bestV1;
        Index bestV2;
        Index bestV3;

        for (Index j = 0; j < polygon.size() - 2; ++j) {
            const Index& v1 = 0;
            const Index& v2 = j + 1;
            const Index& v3 = j + 2;

            Point<T,D> planeClosestPoint;
            Point<T,D> closestPoint = closestPointOnTriangle(polygon[v1], polygon[v2], polygon[v3], point, planeClosestPoint);

            T distance = (point - closestPoint).norm();
            if (distance < minDistance) {
                bestV1 = v1;
                bestV2 = v2;
                bestV3 = v3;

                if (useClosestPoint) {
                    queryPoint = closestPointOnTriangle(polygon[v1], polygon[v2], polygon[v3], point);
                }
                else {
                    queryPoint = planeClosestPoint;
                }

                minDistance = distance;
            }
        }

        value = barycentricInterpolation(
            polygon[bestV1],
            polygon[bestV2],
            polygon[bestV3],
            queryPoint,
            values[bestV1],
            values[bestV2],
            values[bestV3]);
    }

    return value;
}


}
