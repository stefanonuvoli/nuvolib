/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "dual_quaternion.h"

namespace nvl {

/**
 * @brief Default constructor
 */
template<class T>
DualQuaternion<T>::DualQuaternion()
    : vReal(Quaternion<T>()), vDual(Quaternion<T>())
{

}

/**
 * @brief Constructor with real and dual parts
 * @param real Real part
 * @param dual Dual part
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& real, const Quaternion<T>& dual)
    : vReal(real), vDual(dual)
{

}

/**
 * @brief Constructor with rotation and translation
 * @param rotation Rotation part
 * @param translate Translation part
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& rotation, const Vector3<T>& translate)
    : vReal(rotation),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

/**
 * @brief Constructor with rotation and translation
 * @param rotation Rotation part
 * @param translate Translation part
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Rotation3<T>& rotation, const Vector3<T>& translate)
    : vReal(Quaternion<T>(rotation)),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

/**
 * @brief Constructor with rotation and translation
 * @param rotation Rotation part
 * @param translate Translation part
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& rotation, const Translation3<T>& translate)
    : vReal(rotation),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

/**
 * @brief Constructor with rotation and translation
 * @param rotation Rotation part
 * @param translate Translation part
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Rotation3<T>& rotation, const Translation3<T>& translate)
    : vReal(Quaternion<T>(rotation)),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

/**
 * @brief Constructor with affine transformation (scaling will be ignored)
 * @param affine Affine transformation
 */
template<class T>
DualQuaternion<T>::DualQuaternion(const Affine3<T>& affine)
    : DualQuaternion(Quaterniond(affine.rotation()), Translation3<T>(affine.translation()))
{

}

/**
 * @brief Get real part
 * @return Real part
 */
template<class T>
Quaternion<T> DualQuaternion<T>::real() const
{
    return vReal;
}

/**
 * @brief Set real part
 * @param real Real part
 */
template<class T>
void DualQuaternion<T>::setReal(const Quaternion<T>& real)
{
    vReal = real;
}

/**
 * @brief Get dual part
 * @return Dual part
 */
template<class T>
Quaternion<T> DualQuaternion<T>::dual() const
{
    return vDual;
}

/**
 * @brief Set dual part
 * @param real dual part
 */
template<class T>
void DualQuaternion<T>::setDual(const Quaternion<T>& dual)
{
    vDual = dual;
}

/**
 * @brief Conjugate of the dual quaternion
 * @return Conjugate
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::conjugate() const
{
    return DualQuaternion<T>(vReal.conjugate(), -1.0 * vDual.conjugate());
}

/**
 * @brief Inverse of the dual quaternion
 * @return Inverse
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::inverse() const
{
    return DualQuaternion<T>(vReal.inverse(), -1.0 * vDual * vReal.inverse());
}

/**
 * @brief Norm of the dual quaternion
 * @return Norm
 */
template<class T>
T DualQuaternion<T>::norm() const
{
    T realNorm = vReal.norm();
    return realNorm + vReal.coeffs().dot(vDual.coeffs()) / realNorm;
}

/**
 * @brief Normalize the dual quaternion
 */
template<class T>
void DualQuaternion<T>::normalize()
{
    T length = vReal.norm();
    vReal = vReal / length;
    vDual = vDual / length;
    vDual = vDual - (vReal.coeffs().dot(vDual.coeffs()) * vReal);
}

/**
 * @brief Return the normalized dual quaternion
 * @return Normalized dual quaternion
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::normalized() const
{
    DualQuaternion<T> dq = *this;
    dq.normalize();

    return dq;
}

/**
 * @brief Coefficient of the dual quaternions
 * @return Coefficients
 */
template<class T>
Vector<T,8> DualQuaternion<T>::coeffs()
{
    Vector<T,8> coeffs;
    coeffs << vReal.coeffs(), vDual.coeffs();
    return coeffs;
}

/**
 * @brief Set real and dual part to zero
 */
template<class T>
void DualQuaternion<T>::setZero()
{
    vDual = vReal = Quaternion<T>(0.0, 0.0, 0.0, 0.0);
}

/**
 * @brief Rotation of the dual quaternion
 * @return Rotation part
 */
template<class T>
Quaternion<T> DualQuaternion<T>::rotation() const
{
    return vReal;
}

/**
 * @brief Translation of the dual quaternion
 * @return Translation part
 */
template<class T>
Translation3<T> DualQuaternion<T>::translation() const
{
    Quaternion<T> translateVec = 2.0 * vDual * vReal.inverse();
    return Translation3<T>(translateVec.x(), translateVec.y(), translateVec.z());
}

/**
 * @brief Transformation matrix of the the dual quaternion
 * @return Transformation matrix
 */
template<class T>
Matrix33<T> DualQuaternion<T>::matrix() const
{
    return translation() * rotation() * Matrix33<T>::Identity();
}

/**
 * @brief Affine transformation of the dual quaternion
 * @return Affine transformation
 */
template<class T>
Affine3<T> DualQuaternion<T>::affineTransformation() const
{
    return translation() * rotation() * Affine3<T>::Identity();
}

/**
 * @brief Operator overload +
 * @param other Other dual quaternion
 * @return Sum of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator+(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal + other.vReal, vDual + other.vDual);
}

/**
 * @brief Operator overload -
 * @param other Other dual quaternion
 * @return Difference of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator-(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal - other.vReal, vDual - other.vDual);
}

/**
 * @brief Operator overload *
 * @param other Other dual quaternion
 * @return Multiplication of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal * other.vReal, vReal * other.vDual + vDual * other.vReal);
}

/**
 * @brief Operator overload *
 * @param s Scalar value
 * @return Multiplication of the dual quaternion for a scalar
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*(const Scalar& s) const
{
    return DualQuaternion<T>(s * vReal, s * vDual);
}

/**
 * @brief Operator overload /
 * @param s Scalar value
 * @return Division of the dual quaternion for a scalar
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator/(const Scalar& s) const
{
    return DualQuaternion<T>((1.0/s) * vReal, (1.0/s) * vDual);
}

/**
 * @brief Operator overload +=
 * @param other Other dual quaternion
 * @return Sum of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator+=(const DualQuaternion<T>& other)
{
    vReal += other.vReal;
    vDual += other.vDual;

    return *this;
}

/**
 * @brief Operator overload -=
 * @param other Other dual quaternion
 * @return Difference of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator-=(const DualQuaternion<T>& other)
{
    vReal -= other.vReal;
    vDual -= other.vDual;

    return *this;
}

/**
 * @brief Operator overload -=
 * @param other Other dual quaternion
 * @return Multiplication of the dual quaternions
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*=(const DualQuaternion<T>& other)
{
    vDual = vReal * other.vDual + vDual * other.vReal;
    vReal *= other.vReal;

    return *this;

}

/**
 * @brief Operator overload *=
 * @param s Scalar value
 * @return Multplication of the dual quaternion for a scalar
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*=(const Scalar& s)
{
    vReal *= s;
    vDual *= s;

    return *this;
}

/**
 * @brief Operator overload /=
 * @param s Scalar value
 * @return Division of the dual quaternion for a scalar
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::operator/=(const Scalar& s)
{
    vReal /= s;
    vDual /= s;

    return *this;
}

/**
 * @brief Operator overload *
 * @param point Point
 * @return Transformed point by the dual quaternion
 */
template<class T>
Point3<typename DualQuaternion<T>::Scalar> DualQuaternion<T>::operator*(const Point3<Scalar>& point) const
{
    DualQuaternion<T> dq(
                Quaternion<T>(1, 1, 1, 1),
                Quaternion<T>(0, point.x(), point.y(), point.z()));

    dq = (*this) * dq;
    dq = dq * this->conjugate();

    Point3<Scalar> result(dq.dual().x(), dq.dual().y(), dq.dual().z());

    return result;
}

/**
 * @brief Operator overload *
 * @param mat Matrix
 * @return Transformed matrix by the dual quaternion
 */
template<class T>
template<class Matrix>
Matrix DualQuaternion<T>::operator*(const Matrix& mat) const
{
    return this->matrix() * mat;
}

/**
 * @brief Dual quaternion identity
 * @return Identity
 */
template<class T>
DualQuaternion<T> DualQuaternion<T>::Identity()
{
    return DualQuaternion<T>(
        Quaternion<T>::Identity(),
        Translation3<T>::Identity());
}

/**
 * @brief Operator overload *
 * @param s Scalar
 * @param dualQuaternion Dual quaternion
 * @return Multiplication of the dual quaternion for the scalar
 */
template<class T>
DualQuaternion<T> operator*(const typename DualQuaternion<T>::Scalar& s, const DualQuaternion<T>& dualQuaternion)
{
    return DualQuaternion<T>(s * dualQuaternion.real(), s * dualQuaternion.dual());
}

/**
 * @brief Operator overload *
 * @param dualQuaternion Dual quaternion
 * @param s Scalar
 * @return Multiplication of the dual quaternion for the scalar
 */
template<class T>
DualQuaternion<T> operator*(const DualQuaternion<T>& dualQuaternion, const typename DualQuaternion<T>::Scalar& s)
{
    return DualQuaternion<T>(s * dualQuaternion.real(), s * dualQuaternion.dual());
}

}

