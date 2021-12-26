/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTILITIES_COMPARATORS_H
#define NVL_UTILITIES_COMPARATORS_H

#include <nvl/nuvolib.h>

namespace nvl {

template<class T, typename... Ts>
bool pairwiseComparator(const T& v1, const T& v2, Ts... values);

bool pairwiseComparator();

}

#include "comparators.cpp"

#endif // NVL_UTILITIES_COMPARATORS_H
