/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "common_functions.h"

#include <math.h>
#include <algorithm>

#include <nvl/math/numeric_limits.h>

namespace nvl {

/**
 * @brief Cosine of a value
 * @param value Angle in radiants
 * @return Cosine
 */
template<class T, class R>
R cos(const T& value)
{
    return static_cast<R>(std::cos(value));
}

/**
 * @brief Sine of a value
 * @param value Angle in radiants
 * @return Sine
 */
template<class T, class R>
R sin(const T& value)
{
    return static_cast<R>(std::sin(value));
}

/**
 * @brief Tangent of a value
 * @param value Angle in radiants
 * @return Tangent
 */
template<class T, class R>
R tan(const T& value)
{
    return static_cast<R>(std::tan(value));
}

/**
 * @brief Arcosine of a value
 * @param value Cosine value
 * @return Angle in radiants
 */
template<class T, class R>
R acos(const T& value)
{
    return static_cast<R>(std::acos(value));
}

/**
 * @brief Arcsine of a value
 * @param value Sine value
 * @return Angle in radiants
 */
template<class T, class R>
R asin(const T& value)
{
    return static_cast<R>(std::asin(value));
}

/**
 * @brief Arctangent of a value
 * @param value Tangent value
 * @return Angle in radiants
 */
template<class T, class R>
R atan(const T& value)
{
    return static_cast<R>(std::atan(value));
}

/**
 * @brief Arctangent (2) of a value
 * @param value Tangent value
 * @return Angle in radiants
 */
template<class T, class R>
R atan2(const T& value)
{
    return static_cast<R>(std::atan2(value));
}

/**
 * @brief Absolute of a given value
 * @param value Value
 * @return Absolute value
 */
template<class T, class R>
R abs(const T& value)
{
    return static_cast<R>(std::abs(value));
}

/**
 * @brief Power of a value for a given exponent
 * @param value Value
 * @param exponent Exponent
 * @return Power
 */
template<class T1, class T2, class R>
R pow(const T1& value, const T2& exponent)
{
    return static_cast<R>(std::pow(value, exponent));
}

/**
 * @brief Power of 2 of a value for a given exponent
 * @param value Value
 * @return Power of 2
 */
template<class T, class R>
R squared(const T& value)
{
    return static_cast<R>(value * value);
}

/**
 * @brief Square root of a value
 * @param value Value
 * @return Square root
 */
template<class T, class R>
R sqrt(const T& value)
{
    return static_cast<R>(std::sqrt(value));
}

/**
 * @brief Max between two values
 * @param value1 Value 1
 * @param value2 Value 2
 * @return Maximum value
 */
template<class T>
T max(const T& value1, const T& value2)
{
    return std::max(value1, value2);
}

/**
 * @brief Max of given values
 * @param values Collection of values
 * @return Maximum value
 */
template<class T>
T max(const std::vector<T>& values)
{
    T maxValue = minLimitValue<T>();

    for (const T& value : values) {
        maxValue = max(value, maxValue);
    }

    return maxValue;
}

/**
 * @brief Min between two values
 * @param value1 Value 1
 * @param value2 Value 2
 * @return Minimum value
 */
template<class T>
T min(const T& value1, const T& value2)
{
    return std::min(value1, value2);
}

/**
 * @brief Min of given values
 * @param values Collection of values
 * @return Minimum value
 */
template<class T>
T min(const std::vector<T>& values)
{
    T minValue = maxLimitValue<T>();

    for (const T& value : values) {
        minValue = min(value, minValue);
    }

    return minValue;
}

/**
 * @brief Sign of a value
 * @param value Value
 * @return +1 if positive, -1 if negative, 0 if zero
 */
template<class T, class R>
R sign(const T& value)
{
    if (value < T(0))
        return R(-1);
    else if (value > T(0))
        return R(1);
    return 0;
}

/**
 * @brief Sum of a collection of values
 * @param vector Values
 * @return Sum of the values
 */
template<class T>
T sum(const std::vector<T>& values)
{
    if (values.empty())
        return 0;

    T sum = values[0];

    for (Index i = 1; i < values.size(); ++i) {
        sum += values[i];
    }

    return sum;
}

/**
 * @brief fmod Module of a value
 * @param value Value
 * @param mod Module
 * @return Module
 */
template<class T, class M, class R>
R mod(const T& value, const M& mod)
{
    return value % mod;
}

/**
 * @brief fmod Decimal module of a value
 * @param value Value
 * @param mod Module
 * @return Decimal module
 */
template<class T, class M, class R>
R fmod(const T& value, const M& mod)
{
    return std::fmod(value, mod);
}

/**
 * @brief Round value to its nearest integer value
 * @param value Value
 * @return Rounded value
 */
template<class T, class R>
R round(const T& value)
{
    return std::round(value);
}

/**
 * @brief Ceil value to next integer value
 * @param value Value
 * @return Ceil value
 */
template<class T, class R>
R ceil(const T& value)
{
    return std::ceil(value);
}

/**
 * @brief Floor value to its previous integer value
 * @param value Value
 * @return Floor value
 */
template<class T, class R>
R floor(const T& value)
{
    return std::floor(value);
}

}
