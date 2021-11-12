#ifndef NVL_MATH_CONVERSIONS_H
#define NVL_MATH_CONVERSIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/matrix.h>
#include <nvl/math/point.h>

#include <vector>

namespace nvl {

template<class T>
MatrixFX<T,3> pointsToMatrix(const std::vector<Point3<T>>& points);

}

#include "conversions.cpp"

#endif // NVL_MATH_CONVERSIONS_H
