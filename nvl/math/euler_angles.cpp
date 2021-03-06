/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "euler_angles.h"

#include <nvl/math/common_functions.h>

namespace nvl {

/**
 * @brief Euler angles (XYZ notation) from a quaternion
 * @param q Quaternion
 * @return Euler angles
 */
template<class T, class A>
Vector3<A> eulerAnglesXYZFromQuaternion(const Quaternion<T>& q) {
    Vector3<A> angles;

    //Roll (x-axis rotation)
    A sinr_cosp = 2 * (q.w() * q.x() + q.y() * q.z());
    A cosr_cosp = 1 - 2 * (q.x() * q.x() + q.y() * q.y());
    angles.x() = std::atan2(sinr_cosp, cosr_cosp);

    //Pitch (y-axis rotation)
    A sinp = 2 * (q.w() * q.y() - q.z() * q.x());
    if (std::abs(sinp) >= 1)
        angles.y() = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.y() = std::asin(sinp);

    //Yaw (z-axis rotation)
    A siny_cosp = 2 * (q.w() * q.z() + q.x() * q.y());
    A cosy_cosp = 1 - 2 * (q.y() * q.y() + q.z() * q.z());
    angles.z() = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

/**
 * @brief Quaternion from euler angles (XYZ notation)
 * @param eulerAngles Euler angles
 * @return Quaternion
 */
template<class A, class T>
Quaternion<T> eulerAnglesToQuaternionXYZ(const Vector3<A>& eulerAngles)
{
    //Yaw (z-axis rotation)
    T cy = cos(eulerAngles.z() * 0.5);
    T sy = sin(eulerAngles.z() * 0.5);

    //Pitch (y-axis rotation)
    T cp = cos(eulerAngles.y() * 0.5);
    T sp = sin(eulerAngles.y() * 0.5);

    //Roll (x-axis rotation)
    T cr = cos(eulerAngles.x() * 0.5);
    T sr = sin(eulerAngles.x() * 0.5);

    Quaternion<T> q(
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy);

    return q;
}

/**
 * @brief Euler angles (XYZ notation) from a rotation
 * @param r Rotation
 * @return Euler angles
 */
template<class T, class A>
Vector3<A> eulerAnglesXYZFromRotation(const Rotation3<T>& r)
{
    return eulerAnglesXYZFromQuaternion(Quaternion<T>(r));
}

/**
 * @brief Rotation from euler angles (XYZ notation)
 * @param eulerAngles Euler angles
 * @return Rotation
 */
template<class A, class T>
Rotation3<T> eulerAnglesToRotationXYZ(const Vector3<A>& eulerAngles)
{
    return Rotation3<T>(eulerAnglesToQuaternionXYZ(eulerAngles));
}


}
