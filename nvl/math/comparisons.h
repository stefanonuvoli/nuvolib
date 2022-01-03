/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_COMPARISONS_H
#define NVL_MATH_COMPARISONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/constants.h>

#include <nvl/math/vector.h>
#include <nvl/math/matrix.h>

namespace nvl {

template<class T1, class T2, class R = double>
bool epsEqual(const T1& obj1, const T2& obj2, const R& epsilon = EPSILON);

template<class T, EigenId D, class R = T>
bool epsEqual(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const R& epsilon = EPSILON);

template<class T, EigenId D1, EigenId D2, class R = T>
bool epsEqual(const Matrix<T,D1,D2>& mat1, const Matrix<T,D1,D2>& mat2, const R& epsilon = EPSILON);

}

#include "comparisons.cpp"

#endif // NVL_MATH_COMPARISONS_H
