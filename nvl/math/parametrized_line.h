/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_PARAMETRIZEDLINE_H
#define NVL_MATH_PARAMETRIZEDLINE_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Geometry>

#include <nvl/math/hyperplane.h>

namespace nvl {

template<class T, Size D> using ParametrizedLine = Eigen::ParametrizedLine<T,D>;

template<class T> using ParametrizedLine2 = ParametrizedLine<T,2>;
typedef ParametrizedLine2<float> ParametrizedLine2f;
typedef ParametrizedLine2<double> ParametrizedLine2d;
typedef ParametrizedLine2<int> ParametrizedLine2i;
typedef ParametrizedLine2<long long int> ParametrizedLine2l;

template<class T> using ParametrizedLine3 = ParametrizedLine<T,3>;
typedef ParametrizedLine3<float> ParametrizedLine3f;
typedef ParametrizedLine3<double> ParametrizedLine3d;
typedef ParametrizedLine3<int> ParametrizedLine3i;
typedef ParametrizedLine3<long long int> ParametrizedLine3l;

template<class T> using ParametrizedLine4 = ParametrizedLine<T,4>;
typedef ParametrizedLine4<float> ParametrizedLine4f;
typedef ParametrizedLine4<double> ParametrizedLine4d;
typedef ParametrizedLine4<int> ParametrizedLine4i;
typedef ParametrizedLine4<long long int> ParametrizedLine4l;

}

#endif

#endif // NVL_MATH_PARAMETRIZEDLINE_H
