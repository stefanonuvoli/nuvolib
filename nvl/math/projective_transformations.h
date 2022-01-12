/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H
#define NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

template<class T, Index D> using ProjectiveTransformation = Eigen::Transform<T,D,Eigen::Projective>;

/* Common typedefs */

template<class T> using Projective3 = ProjectiveTransformation<T,3>;

typedef Projective3<float> Projective3f;
typedef Projective3<double> Projective3d;
typedef Projective3<int> Projective3i;
typedef Projective3<long long int> Projective3l;

}

#endif

#endif // NVL_MATH_PROJECTIVE_TRANSFORMATIONS_H
