/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
