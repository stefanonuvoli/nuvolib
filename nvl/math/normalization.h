#ifndef NVL_MATH_NORMALIZATION_H
#define NVL_MATH_NORMALIZATION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T>
void normalize(std::vector<T>& vector);
template<class T>
std::vector<T> normalized(const std::vector<T>& vector);

}

#include "normalization.cpp"

#endif // NVL_MATH_NORMALIZATION_H
