#ifndef NVL_MATH_ALIGNEDBOX_H
#define NVL_MATH_ALIGNEDBOX_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Geometry>

namespace nvl {

template<class T, EigenId D> using AlignedBox = Eigen::AlignedBox<T,D>;

/* Common typedefs */

template<class T> using AlignedBox2 = AlignedBox<T,2>;
typedef AlignedBox<float,2> AlignedBox2f;
typedef AlignedBox<double,2> AlignedBox2d;
typedef AlignedBox<int,2> AlignedBox2i;
typedef AlignedBox<long,2> AlignedBox2l;

template<class T> using AlignedBox3 = AlignedBox<T,3>;
typedef AlignedBox<float,3> AlignedBox3f;
typedef AlignedBox<double,3> AlignedBox3d;
typedef AlignedBox<int,3> AlignedBox3i;
typedef AlignedBox<long,3> AlignedBox3l;

}

#endif

#endif // NVL_MATH_ALIGNEDBOX_H
