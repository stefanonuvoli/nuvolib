/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_COVARIANCE_H
#define NVL_MATH_COVARIANCE_H

#include <nvl/nuvolib.h>

#include <nvl/math/matrix.h>
#include <nvl/math/point.h>
#include <nvl/math/conversions.h>

#include <vector>

namespace nvl {

template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const bool normalize = true);

template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const Point3<T>& getBarycenter, const bool normalize = true);

template<class T>
Matrix33<T> getCovarianceMatrix(const MatrixXF<T,3>& points, const bool normalize = true);

template<class T>
Matrix33<T> getCovarianceMatrix(const MatrixXF<T,3>& points, const Point3<T>& getBarycenter, const bool normalize = true);

}

#include "covariance.cpp"

#endif // NVL_MATH_COVARIANCE_H
