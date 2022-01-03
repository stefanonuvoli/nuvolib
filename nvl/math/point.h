/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_POINT_H
#define NVL_MATH_POINT_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <Eigen/Dense>

namespace nvl {

template<class T, EigenId D> using Point = Vector<T,D>;

/* Common typedefs */

template<class T> using Point2 = Point<T,2>;
typedef Point2<float> Point2f;
typedef Point2<double> Point2d;
typedef Point2<int> Point2i;
typedef Point2<long long int> Point2l;

template<class T> using Point3 = Point<T,3>;
typedef Point3<float> Point3f;
typedef Point3<double> Point3d;
typedef Point3<int> Point3i;
typedef Point3<long long int> Point3l;

template<class T> using Point4 = Point<T,4>;
typedef Point4<float> Point4f;
typedef Point4<double> Point4d;
typedef Point4<int> Point4i;
typedef Point4<long long int> Point4l;

}

#endif //NVL_MATH_POINT_H
