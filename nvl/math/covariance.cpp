#include "covariance.h"

#include <nvl/math/barycenter.h>

#include <Eigen/Core>

namespace nvl {

template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const bool normalize)
{
    return getCovarianceMatrix(points, barycenter(points), normalize);
}

template<class T>
Matrix33<T> getCovarianceMatrix(const std::vector<Point3<T>>& points, const Point3<T>& barycenter, const bool normalize)
{
    return getCovarianceMatrix(pointsToMatrix(points), barycenter, normalize);
}

template<class T>
Matrix33<T> getCovarianceMatrix(const MatrixXF<T,3>& points, const bool normalize)
{
    return getCovarianceMatrix(points, barycenter(points), normalize);
}

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
