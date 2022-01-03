/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_AFFINE_H
#define NVL_MATH_AFFINE_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Basic typedefs */

template<class T, Size D> using Affine = Eigen::Transform<T,D,Eigen::Affine>;


/* Common 2D typedefs */

template<class T> using Affine2 = Affine<T,2>;

typedef Affine2<float> Affine2f;
typedef Affine2<double> Affine2d;
typedef Affine2<int> Affine2i;
typedef Affine2<long long int> Affine2l;


/* Common 3D typedefs */

template<class T> using Affine3 = Affine<T,3>;

typedef Affine3<float> Affine3f;
typedef Affine3<double> Affine3d;
typedef Affine3<int> Affine3i;
typedef Affine3<long long int> Affine3l;

}

#endif

#endif // NVL_MATH_AFFINE_H
