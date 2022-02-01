/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_COTANGENT_WEIGHTS_H
#define NVL_MATH_COTANGENT_WEIGHTS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

namespace nvl {

template<class T, class R = double>
R cotangentWeight(
    const T& point,
    const T& target,
    const T& lastPoint,
    const T& nextPoint);

}

#include "cotangent_weights.cpp"

#endif // NVL_MATH_COTANGENT_WEIGHTS_H
