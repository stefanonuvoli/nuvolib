#ifndef NVL_MATH_HYPERPLANE_H
#define NVL_MATH_HYPERPLANE_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Geometry>

namespace nvl {

template<class T, EigenId D> using Hyperplane = Eigen::Hyperplane<T,D>;

}

#endif

#endif // NVL_MATH_HYPERPLANE_H
