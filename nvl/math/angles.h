#ifndef NVL_MATH_ANGLES_H
#define NVL_MATH_ANGLES_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

namespace nvl {

template<class T, class R = T>
R angle(const T& cosine);

template<class T, class R = T>
R radiansToDegree(const T& radians);

template<class T, class R = T>
R degreesToRadians(const T& degrees);

template<class T, EigenId D, class R = T>
R angle(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const bool alreadyNormalized = false);

}

#include "angles.cpp"

#endif // NVL_MATH_ANGLES_H
