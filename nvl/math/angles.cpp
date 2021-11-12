#include "angles.h"

#include <nvl/math/constants.h>
#include <nvl/math/trigonometry.h>

namespace nvl {

template<class T, class R>
R angle(const T& cosine)
{
    assert(cosine >= -1 && cosine <= 1 && "Cosine must be between -1 and 1.");

    return acos(cosine);
}

template<class T, class R>
R radiansToDegree(const T& radians)
{
    return radians / PI * 180;
}

template<class T, class R>
R degreesToRadians(const T& degrees)
{
    return degrees / PI * M_PI;
}

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

}
