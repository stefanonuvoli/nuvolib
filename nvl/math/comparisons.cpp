#include "comparisons.h"

#include <nvl/math/common_functions.h>

namespace nvl {

template<class T1, class T2, class R>
bool epsEqual(const T1& obj1, const T2& obj2, const R& epsilon) {
    R absDiff = abs(obj1 /*
                 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
                 * This Source Code Form is subject to the terms of the GNU GPL 3.0
                 *
                 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
                 */
- obj2);

    return absDiff < epsilon;
}

template<class T, EigenId D, class R>
bool epsEqual(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const R& epsilon) {
    for (EigenId i = 0; i < D; ++i) {
        if (!epsEqual(vec1(i), vec2(i), epsilon)) {
            return false;
        }
    }

    return true;
}

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
