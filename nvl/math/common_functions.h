#ifndef NVL_MATH_COMMON_FUNCTIONS_H
#define NVL_MATH_COMMON_FUNCTIONS_H

namespace nvl {

template<class T, class R = T>
R cos(const T& value);

template<class T, class R = T>
R sin(const T& value);

template<class T, class R = T>
R tan(const T& value);

template<class T, class R = T>
R acos(const T& value);

template<class T, class R = T>
R asin(const T& value);

template<class T, class R = T>
R atan(const T& value);

template<class T, class R = T>
R atan2(const T& value);

template<class T, class R = T>
R abs(const T& value);

template<class T1, class T2, class R = T1>
R pow(const T1& value, const T2& exponent);

template<class T, class R = T>
R squared(const T& value);

template<class T, class R = T>
R sqrt(const T& value);

template<class T>
T max(const T& value1, const T& value2);

template<class T>
T min(const T& value1, const T& value2);

template<class T, class R = int>
R sign(const T& value);

}

#include "common_functions.cpp"

#endif // NVL_MATH_COMMON_FUNCTIONS_H
