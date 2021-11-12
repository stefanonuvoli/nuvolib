#include "dual_quaternion.h"

namespace nvl {

template<class T>
DualQuaternion<T>::DualQuaternion()
    : vReal(Quaternion<T>()), vDual(Quaternion<T>())
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& real, const Quaternion<T>& dual)
    : vReal(real), vDual(dual)
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& rotation, const Vector3<T>& translate)
    : vReal(rotation),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Rotation3<T>& rotation, const Vector3<T>& translate)
    : vReal(Quaternion<T>(rotation)),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Quaternion<T>& rotation, const Translation3<T>& translate)
    : vReal(rotation),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Rotation3<T>& rotation, const Translation3<T>& translate)
    : vReal(Quaternion<T>(rotation)),
      vDual(0.5 * Quaternion<T>(0.0, translate.x(), translate.y(), translate.z()) * rotation)
{

}

template<class T>
DualQuaternion<T>::DualQuaternion(const Affine3<T>& affine)
    : DualQuaternion(nvl::Quaterniond(affine.rotation()), nvl::Translation3<T>(affine.translation()))
{

}

template<class T>
Quaternion<T> DualQuaternion<T>::real() const
{
    return vReal;
}

template<class T>
void DualQuaternion<T>::setReal(const Quaternion<T>& real)
{
    vReal = real;
}

template<class T>
Quaternion<T> DualQuaternion<T>::dual() const
{
    return vDual;
}

template<class T>
void DualQuaternion<T>::setDual(const Quaternion<T>& dual)
{
    vDual = dual;
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::conjugate() const
{
    return DualQuaternion<T>(vReal.conjugate(), -1.0 * vDual.conjugate());
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::inverse() const
{
    return DualQuaternion<T>(vReal.inverse(), -1.0 * vDual * vReal.inverse());
}

template<class T>
T DualQuaternion<T>::norm() const
{
    T realNorm = vReal.norm();
    return realNorm + vReal.coeffs().dot(vDual.coeffs()) / realNorm;
}


template<class T>
void DualQuaternion<T>::normalize()
{
    T length = vReal.norm();
    vReal = vReal / length;
    vDual = vDual / length;
    vDual = vDual - (vReal.coeffs().dot(vDual.coeffs()) * vReal);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::normalized() const
{
    DualQuaternion<T> dq = *this;
    dq.normalize();

    return dq;
}

template<class T>
Vector<T,8> DualQuaternion<T>::coeffs()
{
    Vector<T,8> coeffs;
    coeffs << vReal.coeffs(), vDual.coeffs();
    return coeffs;
}

template<class T>
void DualQuaternion<T>::setZero()
{
    vDual = vReal = Quaternion<T>(0.0, 0.0, 0.0, 0.0);
}

template<class T>
Quaternion<T> DualQuaternion<T>::rotation() const
{
    return vReal;
}

template<class T>
Translation3<T> DualQuaternion<T>::translation() const
{
    Quaternion<T> translateVec = 2.0 * vDual * vReal.inverse();
    return Translation3<T>(translateVec.x(), translateVec.y(), translateVec.z());
}

template<class T>
Matrix33<T> DualQuaternion<T>::matrix() const
{
    return rotation() * translation() * Matrix33<T>::Identity();
}

template<class T>
Affine3<T> DualQuaternion<T>::affineTransformation() const
{
    return rotation() * translation() * Affine3<T>::Identity();
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator+(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal + other.vReal, vDual + other.vDual);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator-(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal - other.vReal, vDual - other.vDual);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*(const DualQuaternion<T>& other) const
{
    return DualQuaternion<T>(vReal * other.vReal, vReal * other.vDual + vDual * other.vReal);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*(const Scalar& s) const
{
    return DualQuaternion<T>(s * vReal, s * vDual);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator/(const Scalar& s) const
{
    return DualQuaternion<T>(s * vReal, s * vDual);
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator+=(const DualQuaternion<T>& other)
{
    vReal += other.vReal;
    vDual += other.vDual;

    return *this;
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator-=(const DualQuaternion<T>& other)
{
    vReal -= other.vReal;
    vDual -= other.vDual;

    return *this;
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*=(const DualQuaternion<T>& other)
{
    vDual = vReal * other.vDual + vDual * other.vReal;
    vReal *= other.vReal;

    return *this;

}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator*=(const Scalar& s)
{
    vReal *= s;
    vDual *= s;

    return *this;
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::operator/=(const Scalar& s)
{
    vReal /= s;
    vDual /= s;

    return *this;
}

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

template<class T>
template<class Matrix>
Matrix DualQuaternion<T>::operator*(const Matrix& mat) const
{
    return this->matrix() * mat;
}

template<class T>
DualQuaternion<T> DualQuaternion<T>::Identity()
{
    return DualQuaternion<T>(
        Quaternion<T>::Identity(),
        Quaternion<T>::Identity());
}

template<class T>
DualQuaternion<T> operator*(const typename DualQuaternion<T>::Scalar& s, const DualQuaternion<T>& dualQuaternion)
{
    return DualQuaternion<T>(s * dualQuaternion.real(), s * dualQuaternion.dual());
}

template<class T>
DualQuaternion<T> operator*(const DualQuaternion<T>& dualQuaternion, const typename DualQuaternion<T>::Scalar& s)
{
    return DualQuaternion<T>(s * dualQuaternion.real(), s * dualQuaternion.dual());
}



}

