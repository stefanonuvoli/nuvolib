#ifndef NVL_MATH_NUMERIC_LIMITS_H
#define NVL_MATH_NUMERIC_LIMITS_H

#include <nvl/nuvolib.h>

#include <limits>

namespace nvl {

template<class T>
T maxLimitValue();

template<class T>
T minLimitValue();

}

#include "numeric_limits.cpp"

#endif // NVL_MATH_NUMERIC_LIMITS_H
