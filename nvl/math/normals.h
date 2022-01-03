/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_NORMALS_H
#define NVL_MATH_NORMALS_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>
#include <nvl/math/vector.h>

#include <vector>

namespace nvl {

template<class T>
Vector3<T> faceNormalPlanar(const Point3<T>& p1, const Point3<T>& p2, const Point3<T>& p3, const bool normalize = true);

template<class T>
Vector3<T> faceNormalPlanar(const std::vector<Point3<T>>& points, const bool normalize = true);

template<class T>
Vector3<T> faceNormalSVDFitting(const std::vector<Point3<T>>& points);

template<class T>
Vector3<T> faceNormalCovarianceFitting(const std::vector<Point3<T>>& points);

}

#include "normals.cpp"

#endif // NVL_MATH_NORMALS_H
