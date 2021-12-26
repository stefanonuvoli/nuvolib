#ifndef NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H
#define NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Basic typedefs */

template<class T, Index D> using ProjectiveTransformation = Eigen::Transform<T,D,Eigen::Projective>;

/* 3D Typedefs */

template<class T> using Projective3 = ProjectiveTransformation<T,3>;

/* Common 3D typedefs */

typedef Projective3<float> Projective3f;
typedef Projective3<double> Projective3d;
typedef Projective3<int> Projective3i;
typedef Projective3<long long int> Projective3l;

}

#endif

#endif // NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H
