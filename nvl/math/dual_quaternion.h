#ifndef NVL_MATH_DUAL_QUATERNION_H
#define NVL_MATH_DUAL_QUATERNION_H

#include <nvl/nuvolib.h>

#include <nvl/math/quaternion.h>
#include <nvl/math/vector.h>
#include <nvl/math/point.h>
#include <nvl/math/transformations.h>

namespace nvl {

template<class T>
class DualQuaternion {

public:

    typedef T Scalar;

    DualQuaternion();
    DualQuaternion(const Quaternion<T>& real, const Quaternion<T>& dual);
    DualQuaternion(const Quaternion<T>& rotation, const Vector3<T>& translate);
    DualQuaternion(const Rotation3<T>& rotation, const Vector3<T>& translate);
    DualQuaternion(const Quaternion<T>& rotation, const Translation3<T>& translate);
    DualQuaternion(const Rotation3<T>& rotation, const Translation3<T>& translate);
    DualQuaternion(const Affine3<T>& affine);

    Quaternion<T> real() const;
    void setReal(const Quaternion<T>& real);

    Quaternion<T> dual() const;
    void setDual(const Quaternion<T>& dual);

    DualQuaternion<T> conjugate() const;
    DualQuaternion<T> inverse() const;

    T norm() const;
    void normalize();
    DualQuaternion<T> normalized() const;

    Vector<T,8> coeffs();

    void setZero();

    Quaternion<T> rotation() const;
    Translation3<T> translation() const;
    Matrix33<T> matrix() const;
    Affine3<T> affineTransformation() const;

    DualQuaternion<T> operator+(const DualQuaternion<T>& s) const;
    DualQuaternion<T> operator-(const DualQuaternion<T>& s) const;
    DualQuaternion<T> operator*(const DualQuaternion<T>& s) const;
    DualQuaternion<T> operator*(const Scalar& s) const;
    DualQuaternion<T> operator/(const Scalar& s) const;
    DualQuaternion<T> operator+=(const DualQuaternion<T>& s);
    DualQuaternion<T> operator-=(const DualQuaternion<T>& s);
    DualQuaternion<T> operator*=(const DualQuaternion<T>& s);
    DualQuaternion<T> operator*=(const Scalar& s);
    DualQuaternion<T> operator/=(const Scalar& s);

    Point3<Scalar> operator*(const Point3<Scalar>& point) const;
    template<class Matrix>
    Matrix operator*(const Matrix& mat) const;

    static DualQuaternion<T> Identity();

private:

    Quaternion<T> vReal;
    Quaternion<T> vDual;

};

template<class T>
DualQuaternion<T> operator*(const typename DualQuaternion<T>::Scalar& s, const DualQuaternion<T>& dualQuaternion);
template<class T>
DualQuaternion<T> operator*(const DualQuaternion<T>& dualQuaternion, const typename DualQuaternion<T>::Scalar& s);

/* Common typedefs */

typedef DualQuaternion<float> DualQuaternionf;
typedef DualQuaternion<double> DualQuaterniond;
typedef DualQuaternion<int> DualQuaternioni;
typedef DualQuaternion<long long int> DualQuaternionl;

}

#include "dual_quaternion.cpp"

#endif //NVL_MATH_DUAL_QUATERNION_H
