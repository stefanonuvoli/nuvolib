/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "numeric_limits.h"

namespace nvl {

/**
 * @brief Max value of a type
 * @return Maximum value
 */
template<class T>
T maxLimitValue()
{
    return std::numeric_limits<T>::max();
}

/**
 * @brief Min value of a type
 * @return Minimum value
 */
template<class T>
T minLimitValue()
{
    return std::numeric_limits<T>::lowest();
}

}
