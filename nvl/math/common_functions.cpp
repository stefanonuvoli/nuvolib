#include "common_functions.h"

#include <math.h>
#include <algorithm>

#include <nvl/math/numeric_limits.h>

namespace nvl {

template<class T, class R>
R cos(const T& value)
{
    return static_cast<R>(std::cos(value));
}

template<class T, class R>
R sin(const T& value)
{
    return static_cast<R>(std::sin(value));
}

template<class T, class R>
R tan(const T& value)
{
    return static_cast<R>(std::tan(value));
}

template<class T, class R>
R acos(const T& value)
{
    return static_cast<R>(std::acos(value));
}

template<class T, class R>
R asin(const T& value)
{
    return static_cast<R>(std::asin(value));
}

template<class T, class R>
R atan(const T& value)
{
    return static_cast<R>(std::atan(value));
}

template<class T, class R>
R atan2(const T& value)
{
    return static_cast<R>(std::atan2(value));
}

template<class T, class R>
R abs(const T& value)
{
    return static_cast<R>(std::abs(value));
}

template<class T1, class T2, class R>
R pow(const T1& value, const T2& exponent)
{
    return static_cast<R>(std::pow(value, exponent));
}

template<class T, class R>
R squared(const T& value)
{
    return static_cast<R>(value * value);
}

template<class T, class R>
R sqrt(const T& value)
{
    return static_cast<R>(std::sqrt(value));
}

template<class T>
T max(const T& value1, const T& value2)
{
    return std::max(value1, value2);
}

template<class T>
T max(const std::vector<T>& vector)
{
    T maxValue = minLimitValue<T>();

    for (const T& value : vector) {
        maxValue = max(value, maxValue);
    }

    return maxValue;
}

template<class T>
T min(const T& value1, const T& value2)
{
    return std::min(value1, value2);
}

template<class T>
T min(const std::vector<T>& vector)
{
    T minValue = maxLimitValue<T>();

    for (const T& value : vector) {
        minValue = min(value, minValue);
    }

    return minValue;
}

template<class T, class R>
R sign(const T& value)
{
    if (value < T(0))
        return R(-1);
    else if (value > T(0))
        return R(1);
    return 0;
}

template<class T>
T sum(const std::vector<T>& vector)
{
    if (vector.empty())
        return 0;

    T sum = vector[0];

    for (Index i = 1; i < vector.size(); ++i) {
        sum += vector[i];
    }

    return sum;
}

}
