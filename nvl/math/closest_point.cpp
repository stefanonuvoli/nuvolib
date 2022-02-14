/*
* This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
* This Source Code Form is subject to the terms of the GNU GPL 3.0
*
* @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
*/
#include "closest_point.h"

#include <nvl/math/normals.h>
#include <nvl/math/inside_test.h>

namespace nvl {

/**
 * @brief Closest point on a given line
 * @param p1 Line start point
 * @param p2 Line end point
 * @param point Target point
 * @return Closest point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnLine(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point)
{
    double t;
    return closestPointOnLine(p1, p2, point, t);
}

template<class T, EigenId D>
Point<T,D> closestPointOnLine(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point,
        double& t)
{
    Vector3<T> segmentVec = p2 - p1;

    T numerator = - (p1 - point).dot(segmentVec);
    T denominator = segmentVec.norm();
    denominator = denominator * denominator;

    t = numerator / denominator;

    Point<T,D> result = p1 + segmentVec * t;

    return result;
}

/**
 * @brief Closest point on a given segment
 * @param p1 Segment start point
 * @param p2 segment end point
 * @param point Target point
 * @return Closest point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnSegment(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point)
{
    double t;
    return closestPointOnSegment(p1, p2, point, t);
}


/**
 * @brief Closest point on a given segment
 * @param p1 Segment start point
 * @param p2 segment end point
 * @param point Target point
 * @param t Output parameterization on the segment
 * @return Closest point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnSegment(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point,
        double& t)
{
    Vector3<T> segmentVec = p2 - p1;

    T numerator = - (p1 - point).dot(segmentVec);
    T denominator = segmentVec.norm();
    denominator = denominator * denominator;

    t = numerator / denominator;

    t = max(min(t, static_cast<T>(1.0)), static_cast<T>(0.0));

    Point<T,D> result = p1 + segmentVec * t;

    return result;
}

/**
 * @brief Closest point on a given triangle
 * @param p1 Vertex 1
 * @param p2 Vertex 2
 * @param p3 Vertex 3
 * @param point Target point
 * @return Closest point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point)
{
    Point<T,D> planeClosestPoint;

    return closestPointOnTriangle(p1, p2, p3, point, planeClosestPoint);
}

template<class T, EigenId D>
Point<T,D> closestPointOnTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        Point<T,D>& planeClosestPoint)
{
    Plane<T> plane(faceNormalPlanar(p1, p2, p3), barycenter(p1, p2, p3));

    planeClosestPoint = closestPointOnPlane(plane, point);

    bool isInside = pointInsideTriangle(p1, p2, p3, planeClosestPoint);

    if (isInside) {
        return planeClosestPoint;
    }
    else {
        Point<T,D> c1 = closestPointOnSegment(p2, p1, planeClosestPoint);
        Point<T,D> c2 = closestPointOnSegment(p3, p2, planeClosestPoint);
        Point<T,D> c3 = closestPointOnSegment(p1, p3, planeClosestPoint);

        T d1 = (c1 - planeClosestPoint).norm();
        T d2 = (c2 - planeClosestPoint).norm();
        T d3 = (c3 - planeClosestPoint).norm();

        if (d1 <= d2 && d1 <= d3) {
            return c1;
        }
        else if (d2 <= d1 && d2 <= d3) {
            return c2;
        }
        else {
            return c3;
        }
    }
}

/**
 * @brief Closest point lying on a given plane
 * @param plane Plane
 * @param point Target point
 * @return Closest point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnPlane(
        const Plane<T>& plane,
        const Point<T,D>& point)
{
    return plane.projection(point);
}


/**
 * @brief Closest point on a polygon, using barycenter subdivision
 * @param polygon Polygon vertices
 * @param point Target point
 * @return Closest Point
 */
template<class T, EigenId D>
Point<T,D> closestPointOnPolygonBarycenterSubdivision(
        const std::vector<Point<T,D>>& polygon,
        const Point<T,D>& point)
{
    Point<T,D> closestPoint;

    if (polygon.size() == 3) { //Triangle
        closestPoint = closestPointOnTriangle(
            polygon[0],
            polygon[1],
            polygon[2],
            point);
    }
    else { //Polygon
        assert(polygon.size() > 3);

        Point<T,D> queryPoint = point;

        //Get barycenter
        Point<T,D> pB = barycenter(polygon);

        //Calculate the closest point on the best triangle
        T minDistance = maxLimitValue<T>();
        for (Index j = 0; j < polygon.size(); ++j) {
            const Index& v1 = j;
            const Index& v2 = (j + 1) % polygon.size();

            Point<T,D> currentPoint = closestPointOnTriangle(polygon[v1], polygon[v2], pB, point);
            T distance = (point - closestPoint).norm();
            if (distance < minDistance) {
                closestPoint = closestPointOnTriangle(polygon[v1], polygon[v2], pB, point);
                minDistance = distance;
            }
        }
    }

    return closestPoint;
}


}
