#ifndef NVL_MATH_FITTING_H
#define NVL_MATH_FITTING_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <nvl/math/plane.h>
#include <nvl/math/point.h>

#include <vector>

namespace nvl {

template<class T>
Plane<T> bestFittingPlaneCovariance(const std::vector<Point3<T>>& points);

template<class T>
Plane<T> bestFittingPlaneSVD(const std::vector<Point3<T>>& points);

}

#endif

#include "fitting.cpp"

#endif // NVL_MATH_FITTING_H
