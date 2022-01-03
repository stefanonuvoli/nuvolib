/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_INSIDE_TEST_H
#define NVL_MATH_INSIDE_TEST_H

#include <nvl/math/point.h>

#include <vector>

namespace nvl {

template<EigenId D, class T>
bool pointInsideTriangle(
        const Point<T,D>& p1,
        const Point<T,D>& p2,
        const Point<T,D>& p3,
        const Point<T,D>& point,
        const bool strictly = false);

}

#include "inside_test.cpp"

#endif // NVL_MATH_INSIDE_TEST_H
