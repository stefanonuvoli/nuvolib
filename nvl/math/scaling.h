#ifndef NVL_MATH_SCALING_H
#define NVL_MATH_SCALING_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Typedefs */

template<class T, Size D> using Scaling = Eigen::DiagonalMatrix<T,D>;


/* Common 2D typedefs */

template<class T> using Scaling2 = Scaling<T,2>;

typedef Scaling2<float> Scaling2f;
typedef Scaling2<double> Scaling2d;
typedef Scaling2<int> Scaling2i;
typedef Scaling2<long long int> Scaling2l;


/* Common 3D typedefs */

template<class T> using Scaling3 = Scaling<T,3>;

typedef Scaling3<float> Scaling3f;
typedef Scaling3<double> Scaling3d;
typedef Scaling3<int> Scaling3i;
typedef Scaling3<long long int> Scaling3l;

}

#endif

#endif // NVL_MATH_SCALING_H
