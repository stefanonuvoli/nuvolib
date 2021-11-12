#ifndef NVL_MATH_CLOSEST_POINT_H
#define NVL_MATH_CLOSEST_POINT_H

#include <nvl/math/point.h>
#include <nvl/math/line.h>
#include <nvl/math/plane.h>

#include <vector>

namespace nvl {

template<class T, EigenId D>
Point<T,D> closestPointOnLine(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point);

template<class T, EigenId D>
Point<T,D> closestPointOnLine(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point,
        double& t);

template<class T, EigenId D>
Point<T,D> closestPointOnSegment(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point);

template<class T, EigenId D>
Point<T,D> closestPointOnSegment(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& point,
        double& t);

template<class T, EigenId D>
Point<T,D> closestPointOnTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point);

template<class T, EigenId D>
Point<T,D> closestPointOnTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        Point<T,D>& planeClosestPoint);

template<class T, EigenId D>
Point<T,D> closestPointOnPlane(
        const Plane<T>& plane,
        const Point<T,D>& point);

}

#include "closest_point.cpp"

#endif // NVL_MATH_CLOSEST_POINT_H
