/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "numeric_limits.h"

namespace nvl {

template<class T>
T maxLimitValue()
{
    return std::numeric_limits<T>::max();
}

template<class T>
T minLimitValue()
{
    return std::numeric_limits<T>::lowest();
}

}
