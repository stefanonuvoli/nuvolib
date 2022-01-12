/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_ROTATION_H
#define NVL_MATH_ROTATION_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Typedefs */

template<class T, Size D> using Rotation = Eigen::RotationBase<T,D>;


/* Common typedefs */

template<class T> using Rotation2 = Eigen::Rotation2D<T>;
template<class T> using Rotation3 = Eigen::AngleAxis<T>;

typedef Rotation2<float> Rotation2f;
typedef Rotation2<double> Rotation2d;
typedef Rotation2<int> Rotation2i;
typedef Rotation2<long long int> Rotation2l;

typedef Rotation3<float> Rotation3f;
typedef Rotation3<double> Rotation3d;
typedef Rotation3<int> Rotation3i;
typedef Rotation3<long long int> Rotation3l;

}

#endif

#endif // NVL_MATH_ROTATION_H
