/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_VECTOR_H
#define NVL_MATH_VECTOR_H

#include <nvl/nuvolib.h>

#include <nvl/math/matrix.h>

namespace nvl {

template<class T, EigenId D>
class Vector : public Matrix<T,D,1> {
public:
    Vector();

    template<typename... Ts>
    Vector(Ts... args);

    template<typename OtherDerived>
    Vector(const Matrix<T,D,1>& other);

    template<typename OtherDerived>
    Vector& operator=(const Matrix<T,D,1>& other);

    bool operator<(const Vector<T,D>& vec) const;
    bool operator<=(const Vector<T,D>& vec) const;
    bool operator>(const Vector<T,D>& vec) const;
    bool operator>=(const Vector<T,D>& vec) const;
};

/* Common typedefs */

template<class T> using Vector2 = Vector<T,2>;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;
typedef Vector2<long long int> Vector2l;

template<class T> using Vector3 = Vector<T,3>;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;
typedef Vector3<long long int> Vector3l;

template<class T> using Vector4 = Vector<T,4>;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int> Vector4i;
typedef Vector4<long long int> Vector4l;

}

#include "vector.cpp"

#endif //NVL_MATH_VECTOR_H
