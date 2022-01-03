/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "normals.h"

#include <nvl/math/plane.h>
#include <nvl/math/fitting.h>
#include <nvl/math/comparisons.h>

namespace nvl {

template<class T>
Vector3<T> faceNormalPlanar(const Point3<T>& p1, const Point3<T>& p2, const Point3<T>& p3, const bool normalize)
{
    Vector3<T> vec = (p2 - p1).cross(p3 - p2);

    if (normalize)
        vec.normalize();

    return vec;
}

template<class T>
Vector3<T> faceNormalPlanar(const std::vector<Point3<T>>& points, const bool normalize)
{
    return faceNormalPlanar(points[0], points[1], points[2], normalize);
}

template<class T>
Vector3<T> faceNormalCovarianceFitting(const std::vector<Point3<T>>& points)
{
    Vector3<T> planar = faceNormalPlanar(points, false);

    Plane<T> plane = bestFittingPlaneCovariance(points);
    Vector3<T> vec = plane.normal();

    if (vec.dot(planar) < 0)
        vec = -vec;

    assert(epsEqual(vec.norm(), 1));

    return vec;

}

template<class T>
Vector3<T> faceNormalSVDFitting(const std::vector<Point3<T>>& points)
{
    Vector3<T> planar = faceNormalPlanar(points, false);

    Plane<T> plane = bestFittingPlaneSVD(points);
    Vector3<T> vec = plane.normal();

    if (vec.dot(planar) < 0)
        vec = -vec;

    assert(epsEqual(vec.norm(), 1));

    return vec;
}

}
