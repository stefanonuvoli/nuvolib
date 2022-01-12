/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_LINE_H
#define NVL_MATH_LINE_H

#include <nvl/nuvolib.h>

#include <Eigen/Geometry>

#include <nvl/math/hyperplane.h>

namespace nvl {

template<class T> using Line = Hyperplane<T,2>;

/* Common typedefs */

typedef Line<float> Linef;
typedef Line<double> Lined;
typedef Line<int> Linei;
typedef Line<long> Linel;

}

#endif // NVL_MATH_LINE_H
