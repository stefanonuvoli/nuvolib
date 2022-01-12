/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "quaternion.h"

#ifdef NVL_EIGEN_LOADED

namespace nvl {

/**
 * @brief Operator overload +
 * @param q1 Quaternion 1
 * @param q2 Quaternion 2
 * @return Sum of the two quaternions
 */
template<class T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    Quaternion<T> sum(q1.coeffs() + q2.coeffs());
    return sum;
}

/**
 * @brief Operator overload -
 * @param q1 Quaternion 1
 * @param q2 Quaternion 2
 * @return Difference of the two quaternions
 */
template<class T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    Quaternion<T> difference(q1.coeffs() - q2.coeffs());
    return difference;
}

/**
 * @brief Operator overload *
 * @param q Quaternion 1
 * @param s Scalar
 * @return Multiplication of the quaternion for a scalar
 */
template<class T>
Quaternion<T> operator*(const Quaternion<T>& q, const T& s) {
    return Quaternion<T>(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
}

/**
 * @brief Operator overload *
 * @param s Scalar
 * @param q Quaternion 1
 * @return Multiplication of the quaternion for a scalar
 */
template<class T>
Quaternion<T> operator*(const T& s, const Quaternion<T>& q) {
    return Quaternion<T>(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
}

/**
 * @brief Operator overload /
 * @param q Quaternion 1
 * @param s Scalar
 * @return Division of the quaternion for a scalar
 */
template<class T>
Quaternion<T> operator/(const Quaternion<T>& q, const T& s) {
    return Quaternion<T>(q.w() / s, q.x() / s, q.y() / s, q.z() / s);
}

/**
 * @brief Operator overload /
 * @param s Scalar
 * @param q Quaternion 1
 * @return Division of the quaternion for a scalar
 */
template<class T>
Quaternion<T> operator/(const T& s, const Quaternion<T>& q) {
    return Quaternion<T>(q.w() / s, q.x() / s, q.y() / s, q.z() / s);
}

}

#endif
