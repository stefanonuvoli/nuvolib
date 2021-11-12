#ifndef NVL_MATH_PLANE_H
#define NVL_MATH_PLANE_H

#include <nvl/nuvolib.h>

#include <Eigen/Geometry>

#include <nvl/math/hyperplane.h>

namespace nvl {

template<class T> using Plane = Hyperplane<T,3>;
typedef Plane<float> Planef;
typedef Plane<double> Planed;
typedef Plane<int> Planei;
typedef Plane<long> Planel;

}

#endif // NVL_MATH_PLANE_H
