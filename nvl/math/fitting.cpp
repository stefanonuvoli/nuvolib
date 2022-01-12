/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "fitting.h"

#ifdef NVL_EIGEN_LOADED

#include <nvl/math/barycenter.h>
#include <nvl/math/covariance.h>
#include <nvl/math/conversions.h>
#include <nvl/math/matrix.h>

namespace nvl {

/**
 * @brief Get the best fitting plane using covariance matrix
 * @param points Points
 * @return Best fitting plane
 */
template<class T>
Plane<T> bestFittingPlaneCovariance(const std::vector<Point3<T>>& points)
{
    MatrixFX<T,3> matrix = pointsToRowMatrix(points);

    Point3<T> b = barycenter(matrix);

    Matrix33<T> covarianceMatrix = getCovarianceMatrix(matrix, b, false);

    Eigen::SelfAdjointEigenSolver<Matrix33<T>> eigenSolver(covarianceMatrix);
    Vector3<T> eigenvalues = eigenSolver.eigenvalues().cwiseAbs();
    Matrix33<T> eigenvector = eigenSolver.eigenvectors();

    EigenId minRow;
    eigenvalues.minCoeff(&minRow);

    Vector3<T> planeNormal = eigenvector.col(minRow);

    Plane<T> plane(planeNormal, b);

    return plane;
}

/**
 * @brief Get the best fitting plane using SVD decomposition
 * @param points Points
 * @return Best fitting plane
 */
template<class T>
Plane<T> bestFittingPlaneSVD(const std::vector<Point3<T>>& points)
{
    MatrixFX<T,3> matrix = pointsToRowMatrix(points);

    Point3<T> b = barycenter(matrix);

    MatrixFX<T,3> centered = (matrix.colwise() - b);

    Eigen::JacobiSVD<MatrixFX<T,3>> svd = centered.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeThinV);
    Vector3<T> planeNormal = svd.matrixU().col(2);

    Plane<T> plane(planeNormal, b);

    return plane;
}

}

#endif
