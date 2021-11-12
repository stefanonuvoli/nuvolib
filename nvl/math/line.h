#ifndef NVL_MATH_LINE_H
#define NVL_MATH_LINE_H

#include <nvl/nuvolib.h>

#include <Eigen/Geometry>

#include <nvl/math/hyperplane.h>

namespace nvl {

template<class T> using Line = Hyperplane<T,2>;
typedef Line<float> Linef;
typedef Line<double> Lined;
typedef Line<int> Linei;
typedef Line<long> Linel;

}

#endif // NVL_MATH_LINE_H
