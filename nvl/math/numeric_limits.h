/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
