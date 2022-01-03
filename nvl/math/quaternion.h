/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_QUATERNION_H
#define NVL_MATH_QUATERNION_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Dense>

namespace nvl {

template<class T> using Quaternion = Eigen::Quaternion<T>;

/* Common typedefs */

typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;
typedef Quaternion<int> Quaternioni;
typedef Quaternion<long long int> Quaternionl;


/* Missing operations of quaternions */

template<class T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2);

template<class T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2);

template<class T>
Quaternion<T> operator*(const Quaternion<T>& q, const T& s);

template<class T>
Quaternion<T> operator*(const T& s, const Quaternion<T>& q);

template<class T>
Quaternion<T> operator/(const Quaternion<T>& q, const T& s);

template<class T>
Quaternion<T> operator/(const T& s, const Quaternion<T>& q);

}

#endif

#include "quaternion.cpp"

#endif //NVL_MATH_QUATERNION_H
