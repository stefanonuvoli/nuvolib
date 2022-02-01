/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "cotangent_weights.h"

#include <nvl/math/angles.h>
#include <nvl/math/comparisons.h>

namespace nvl {

template<class T, class R>
R cotangentWeight(
    const T& point,
    const T& target,
    const T& targetPrev,
    const T& targetNext)
{
    Vector3<typename T::Scalar> v1 = point - targetPrev;
    Vector3<typename T::Scalar> v2 = target - targetPrev;
    Vector3<typename T::Scalar> v3 = point - targetNext;
    Vector3<typename T::Scalar> v4 = target - targetNext;

    R cotan_alpha = cotan(angle(v1, v2));
    R cotan_beta  = cotan(angle(v3, v4));

    R wij = 0.5 * (cotan_alpha + cotan_beta);

    if (isnan(wij)) {
        wij = 0.0;
    }

    const R cotan_infinite = cos(EPSILON)/sin(EPSILON);
    wij = clamp(wij, -cotan_infinite, cotan_infinite);

    return wij;
}

}
