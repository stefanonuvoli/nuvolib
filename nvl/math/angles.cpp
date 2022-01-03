/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "angles.h"

#include <nvl/math/constants.h>
#include <nvl/math/common_functions.h>

namespace nvl {

/**
 * @brief Get angle from cosine value
 * @param cosine Cosine
 * @return Angle in radiants
 */
template<class T, class R>
R angle(const T& cosine)
{
    assert(cosine >= -1 && cosine <= 1 && "Cosine must be between -1 and 1.");

    return acos(cosine);
}

/**
 * @brief Get angle between two vectors
 * @param vec1 Vector 1
 * @param vec2 Vector 2
 * @param alreadyNormalized If false the vector will be normalized
 * @return Angle in radiants
 */
template<class T, EigenId D, class R>
R angle(const Vector<T,D>& vec1, const Vector<T,D>& vec2, const bool alreadyNormalized)
{
    if (!alreadyNormalized) {
        return angle(dot(vec1.normalized(), vec2.normalized()));
    }
    else {
        return angle(vec1, vec2);
    }
}

/**
 * @brief Convert an angle from radians to degrees
 * @param radians Angle radians
 * @return Angle degrees
 */
template<class T, class R>
R radiansToDegree(const T& radians)
{
    return radians / PI * 180;
}

/**
 * @brief Convert an angle from degrees to radiants
 * @param radians Angle degrees
 * @return Angle radiants
 */
template<class T, class R>
R degreesToRadians(const T& degrees)
{
    return degrees / PI * M_PI;
}

}
