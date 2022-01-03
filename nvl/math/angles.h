/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_ANGLES_H
#define NVL_MATH_ANGLES_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

namespace nvl {

template<class T, class R = T>
R angle(const T& cosine);

template<class T, EigenId D, class R = T>
R angle(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const bool alreadyNormalized = false);

template<class T, class R = T>
R radiansToDegree(const T& radians);

template<class T, class R = T>
R degreesToRadians(const T& degrees);

}

#include "angles.cpp"

#endif // NVL_MATH_ANGLES_H
