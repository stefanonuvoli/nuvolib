#include "quaternion.h"

#ifdef NVL_EIGEN_LOADED

namespace nvl {

template<class T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    Quaternion<T> sum(q1.coeffs() + q2.coeffs());
    return sum;
}

template<class T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    Quaternion<T> difference(q1.coeffs() - q2.coeffs());
    return difference;
}

template<class T>
Quaternion<T> operator*(const Quaternion<T>& q, const T& s) {
    return Quaternion<T>(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
}

template<class T>
Quaternion<T> operator*(const T& s, const Quaternion<T>& q) {
    return Quaternion<T>(q.w() * s, q.x() * s, q.y() * s, q.z() * s);
}

template<class T>
Quaternion<T> operator/(const Quaternion<T>& q, const T& s) {
    return Quaternion<T>(q.w() / s, q.x() / s, q.y() / s, q.z() / s);
}

template<class T>
Quaternion<T> operator/(const T& s, const Quaternion<T>& q) {
    return Quaternion<T>(q.w() / s, q.x() / s, q.y() / s, q.z() / s);
}

}

#endif
