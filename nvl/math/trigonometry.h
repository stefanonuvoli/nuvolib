#ifndef NVL_MATH_TRIGONOMETRY_H
#define NVL_MATH_TRIGONOMETRY_H

namespace nvl {

template<class T, class R = T>
R sin(const T& value);

template<class T, class R = T>
R cos(const T& value);

template<class T, class R = T>
R asin(const T& value);

template<class T, class R = T>
R acos(const T& value);

}

#include "trigonometry.cpp"

#endif // NVL_MATH_TRIGONOMETRY_H
