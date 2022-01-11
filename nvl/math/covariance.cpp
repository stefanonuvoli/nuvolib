/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "covariance.h"

#include <nvl/math/barycenter.h>

#include <Eigen/Core>

namespace nvl {

/**
 * @brief Covariance matrix of a collection of points
 * @param points Points
 * @param normalize Normalize covariance matrix (default true)
 * @return Covariance matrix
 */
template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const bool normalize)
{
    return getCovarianceMatrix(points, barycenter(points), normalize);
}

/**
 * @brief Covariance matrix of a collection of points
 * @param points Points
 * @param barycenter Barycenter of the points
 * @param normalize Normalize covariance matrix (default true)
 * @return Covariance matrix
 */
template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const Point3<T>& barycenter, const bool normalize)
{
    return getCovarianceMatrix(pointsToRowMatrix(points), barycenter, normalize);
}

/**
 * @brief Covariance matrix of a collection of points
 * @param points Points in a matrix notation
 * @param normalize Normalize covariance matrix (default true)
 * @return Covariance matrix
 */
template<class T>
Matrix33<T> getCovarianceMatrix(const MatrixXF<T,3>& points, const bool normalize)
{
    return getCovarianceMatrix(points, barycenter(points), normalize);
}

/**
 * @brief Covariance matrix of a collection of points
 * @param points Points in a matrix notation
 * @param barycenter Barycenter of the points
 * @param normalize Normalize covariance matrix (default true)
 * @return Covariance matrix
 */
template<class T>
Matrix33<T> getCovarianceMatrix(const MatrixXF<T,3>& points, const Point3<T>& barycenter, const bool normalize)
{
    MatrixXF<T,3> centered = points.rowwise() - barycenter;
    Matrix33<T> covarianceMatrix = (centered.adjoint() * centered);

    if (normalize)
        covarianceMatrix /= double(points.rows() - 1);

    return covarianceMatrix;
}

}
