/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "normalization.h"

#include <nvl/math/comparisons.h>

namespace nvl {

/**
 * @brief Normalize values in a vector
 * @param vector Vector
 */
template<class T>
void normalize(std::vector<T>& vector)
{
    T sum = 0;

    for (const T& v : vector) {
        sum += v;
    }

    if (!epsEqual(sum, 0)) {
        for (T& v : vector) {
            v /= sum;
        }
    }
}

/**
 * @brief Normalized values in a vector
 * @param vector Vector
 * @return Normalized vector
 */
template<class T>
std::vector<T> normalized(const std::vector<T>& vector)
{
    std::vector<T> normalizedVector = vector;

    normalize(normalizedVector);

    return normalizedVector;
}

}
