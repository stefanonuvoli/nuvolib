/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_TRANSLATION_H
#define NVL_MATH_TRANSLATION_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace nvl {

/* Typedefs */

template<class T, Size D> using Translation = Eigen::Translation<T,D>;


/* Common typedefs */

template<class T> using Translation2 = Translation<T,2>;
template<class T> using Translation3 = Translation<T,3>;

typedef Translation2<float> Translation2f;
typedef Translation2<double> Translation2d;
typedef Translation2<int> Translation2i;
typedef Translation2<long long int> Translation2l;

typedef Translation3<float> Translation3f;
typedef Translation3<double> Translation3d;
typedef Translation3<int> Translation3i;
typedef Translation3<long long int> Translation3l;

}

#endif

#endif // NVL_MATH_TRANSLATION_H
