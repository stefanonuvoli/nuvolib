/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "comparisons.h"

#include <nvl/math/common_functions.h>

namespace nvl {

/**
 * @brief Check if two values are equal given some threshold
 * @param obj1 Object 1
 * @param obj2 Object 2
 * @param epsilon Threshold value
 * @return True if equal, false otherwise
 */
template<class T1, class T2, class R>
bool epsEqual(const T1& obj1, const T2& obj2, const R& epsilon) {
    R absDiff = abs(obj1 - obj2);

    return absDiff < epsilon;
}

/**
 * @brief Check if two collection of values are equal given some threshold
 * @param vec1 Vector of values 1
 * @param vec2 Vector of values 2
 * @param epsilon Threshold value
 * @return True if equal, false otherwise
 */
template<class T, EigenId D, class R>
bool epsEqual(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const R& epsilon) {
    for (EigenId i = 0; i < D; ++i) {
        if (!epsEqual(vec1(i), vec2(i), epsilon)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Check if two matrices are equal given some threshold
 * @param mat1 Matrix 1
 * @param mat2 Matrix 2
 * @param epsilon Threshold value
 * @return True if equal, false otherwise
 */
template<class T, EigenId D1, EigenId D2, class R>
bool epsEqual(const Matrix<T,D1,D2>& mat1, const Matrix<T,D1,D2>& mat2, const R& epsilon) {
    for (EigenId i = 0; i < D1; ++i) {
        for (EigenId j = 0; j < D2; ++j) {
            if (!epsEqual(mat1(i, j), mat2(i, j), epsilon)) {
                return false;
            }
        }
    }

    return true;
}

}
