/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "barycenter.h"

namespace nvl {

/**
 * @brief Barycenter of input points
 * @param points Input points
 * @return Barycenter
 */
template<class T, EigenId D1, EigenId D2>
Point<T, D2> barycenter(const Matrix<T, D1, D2>& points)
{
    Point<T, D2> barycenter = points.rowwise().mean();

    return barycenter;
}

/**
 * @brief Barycenter of input points
 * @param points Input points
 * @return Barycenter
 */
template<class T, EigenId D>
Point<T, D> barycenter(const std::vector<Point<T, D>>& points)
{
    if (points.empty())
        return Point<T, D>::Zero();

    Point<T, D> barycenter = points[0];
    for (Index i = 1; i < points.size(); ++i) {
        barycenter += points[i];
    }
    barycenter /= points.size();

    return barycenter;
}

/**
 * @brief Barycenter of 3 points
 * @param p1 Input point 1
 * @param p2 Input point 2
 * @param p3 Input point 3
 * @return Barycenter
 */
template<class T, EigenId D>
Point<T, D> barycenter(const Point<T, D>& p1, const Point<T, D>& p2, const Point<T, D>& p3)
{
    return (p1 + p2 + p3) / static_cast<T>(3);
}

}
