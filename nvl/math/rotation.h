#ifndef NVL_MATH_ROTATION_H
#define NVL_MATH_ROTATION_H

#include <nvl/nuvolib.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Typedefs */

template<class T, Size D> using Rotation = Eigen::RotationBase<T,D>;


/* Common 2D typedefs */

template<class T> using Rotation2 = Eigen::Rotation2D<T>;

typedef Rotation2<float> Rotation2f;
typedef Rotation2<double> Rotation2d;
typedef Rotation2<int> Rotation2i;
typedef Rotation2<long long int> Rotation2l;


/* Common 3D typedefs */

template<class T> using Rotation3 = Eigen::AngleAxis<T>;

typedef Rotation3<float> Rotation3f;
typedef Rotation3<double> Rotation3d;
typedef Rotation3<int> Rotation3i;
typedef Rotation3<long long int> Rotation3l;

}

#endif // NVL_MATH_ROTATION_H
