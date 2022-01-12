/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_EULER_ANGLES_H
#define NVL_MATH_EULER_ANGLES_H

#include <nvl/nuvolib.h>

#include <nvl/math/quaternion.h>
#include <nvl/math/rotation.h>
#include <nvl/math/vector.h>

namespace nvl {

template<class T, class A = T>
Vector3<A> eulerAnglesXYZFromQuaternion(const Quaternion<T>& q);
template<class A, class T = A>
Quaternion<T> eulerAnglesToQuaternionXYZ(const Vector3<A>& eulerAngles);

template<class T, class A = T>
Vector3<A> eulerAnglesXYZFromRotation(const Rotation3<T>& r);
template<class A, class T = A>
Rotation3<T> eulerAnglesToRotationXYZ(const Vector3<A>& eulerAngles);

}

#include "euler_angles.cpp"

#endif // NVL_MATH_EULER_ANGLES_H
