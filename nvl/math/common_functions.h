/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_COMMON_FUNCTIONS_H
#define NVL_MATH_COMMON_FUNCTIONS_H

#include <vector>

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
R cotan(const T& value);

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
T max(const std::vector<T>& values);

template<class T>
T min(const T& value1, const T& value2);

template<class T>
T min(const std::vector<T>& values);

template<class T, class R = int>
R sign(const T& value);

template<class T>
T sum(const std::vector<T>& values);

template<class T, class M, class R = T>
R mod(const T& value, const M& mod);

template<class T, class M, class R = T>
R fmod(const T& value, const M& mod);

template<class T, class R = T>
R round(const T& value);

template<class T, class R = T>
R ceil(const T& value);

template<class T, class R = T>
R floor(const T& value);

template<class T>
T clamp(const T& value, const T& minValue, const T& maxValue);

template<class T>
bool isnan(const T& value);

}

#include "common_functions.cpp"

#endif // NVL_MATH_COMMON_FUNCTIONS_H
