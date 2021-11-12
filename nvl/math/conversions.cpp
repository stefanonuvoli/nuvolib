#include "conversions.h"

namespace nvl {

template<class T>
MatrixFX<T,3> pointsToMatrix(const std::vector<Point3<T>>& points)
{
    MatrixFX<T,3> matrix;

    matrix.resize(3, points.size());

    EigenId id = 0;

    #pragma omp parallel for
    for (Index i = 0; i < points.size(); ++i) {
        matrix.col(id) = points[i];
        ++id;
    }

    return matrix;
}

}
