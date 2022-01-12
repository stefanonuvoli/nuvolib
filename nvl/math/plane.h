/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_PLANE_H
#define NVL_MATH_PLANE_H

#include <nvl/nuvolib.h>

#include <Eigen/Geometry>

#include <nvl/math/hyperplane.h>

namespace nvl {

template<class T> using Plane = Hyperplane<T,3>;

/* Common typedefs */

typedef Plane<float> Planef;
typedef Plane<double> Planed;
typedef Plane<int> Planei;
typedef Plane<long> Planel;

}

#endif // NVL_MATH_PLANE_H
