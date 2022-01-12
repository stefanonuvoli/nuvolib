/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "inverse_function.h"

namespace nvl {

/**
 * @brief Inverse function of a vector function
 * @param map Function
 * @param size Size of the inverse function (default max value in the co-image)
 * @return Inverse function
 */
template<class T>
std::vector<T> inverseFunction(const std::vector<T>& map, const long long int size)
{
    long long int maxVId = -1;

    if (size > 0) {
        maxVId = size - 1;
    }
    else {
        for (const T& value : map) {
            maxVId = std::max(maxVId, static_cast<long long int>(value));
        }
    }

    if (maxVId < 0)
        return std::vector<T>();

    std::vector<Index> inverse(static_cast<Size>(maxVId + 1), NULL_ID);
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == NULL_ID || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

/**
 * @brief Inverse function of a map
 * @param map Function
 * @return Inverse function
 */
template<class T>
std::map<T,T> inverseFunction(const std::map<T,T>& map)
{
    std::map<T,T> inverse;
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == NULL_ID || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

/**
 * @brief Inverse function of a map
 * @param map Function
 * @return Inverse function
 */
template<class T>
std::unordered_map<T,T> inverseFunction(const std::unordered_map<T,T>& map)
{
    std::map<T,T> inverse;
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == NULL_ID || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

}
