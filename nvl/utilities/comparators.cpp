/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "comparators.h"

namespace nvl {

/**
 * @brief Less comparator for multiple values
 * @param v1 First value
 * @param v2 Second Value
 * @param values Other values
 * @return True if every element is less than the next values
 */
template<class T, typename... Ts>
bool pairwiseLessComparator(const T& v1, const T& v2, Ts... values)
{
    if (v1 < v2) {
        return true;
    }
    else if (v1 > v2) {
        return false;
    }
    else {
        return pairwiseLessComparator(values...);
    }
}

NVL_INLINE bool pairwiseLessComparator()
{
    return false;
}

}
