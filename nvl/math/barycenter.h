/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_BARYCENTER_H
#define NVL_MATH_BARYCENTER_H

#include <nvl/math/point.h>
#include <nvl/math/matrix.h>

#include <vector>

namespace nvl {

template<class T, EigenId D1, EigenId D2>
Point<T, D2> barycenter(const Matrix<T, D1, D2>& points);

template<class T, EigenId D>
Point<T, D> barycenter(const std::vector<Point<T, D>>& points);

template<class T, EigenId D>
Point<T, D> barycenter(const Point<T, D>& p1, const Point<T, D>& p2, const Point<T, D>& p3);

}

#include "barycenter.cpp"

#endif // NVL_MATH_BARYCENTER_H
