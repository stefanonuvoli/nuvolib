/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_PRINCIPAL_CURVATURES_H
#define NVL_MATH_PRINCIPAL_CURVATURES_H

#include <nvl/math/vector.h>

#include <vector>

namespace nvl {

template<class T>
struct PrincipalCurvatures {
    Vector3<T> d1;
    Vector3<T> d2;
    T k1;
    T k2;
};

}

#endif // NVL_MATH_PRINCIPAL_CURVATURES_H
