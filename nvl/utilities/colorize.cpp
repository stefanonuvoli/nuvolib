/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "colorize.h"

namespace nvl {

/**
 * @brief Generate different colors, blending from red to blue
 * @param range Number of possible values
 * @param position Value in the range
 * @param saturation Satuation
 * @param value Value
 * @return Generated color for the given position
 */
NVL_INLINE Color getRangeColor(const Size& range, const Index& position, const int saturation, const int value)
{
    Index pos = position;

    if (pos >= range)
        pos = pos % range;

    int subd = 255 / static_cast<int>(range);

    Color color;

    color.setHsv(subd * pos, saturation, value);

    return color;
}

/**
 * @brief Generate colors maximizing the difference between similar positions
 * @param range Number of possible values
 * @param position Value in the range
 * @param saturation Satuation
 * @param value Value
 * @return Generated color for the given position
 */
NVL_INLINE Color getRangeDifferentColor(const Size& range, const Index& position, const int saturation, const int value)
{
    Index pos = position;

    if (pos >= range)
        pos = pos % range;

    int b, k, m = range;

    for (b = 0, k = 1; k < static_cast<int>(range); k <<= 1) {
        if (static_cast<int>(pos) << 1 >= m) {
            b += k;
            pos -= (m+1) >> 1;
            m >>= 1;
        }
        else {
            m = (m+1) >> 1;
        }
    }

    pos = b;

    return getRangeColor(range, position, saturation, value);
}

/**
 * @brief Generate different colors, blending from red to blue
 * @param number Number of colors
 * @param saturation Satuation
 * @param value Value
 * @return Generated colors
 */
NVL_INLINE std::vector<Color> getRangeColors(const Size& number, const int saturation, const int value)
{
    std::vector<Color> result(number);
    if (number > 0) {
        for (Index i = 0; i < number; ++i) {
            result[i] = getRangeColor(number + 1, i, saturation, value);
        }
    }
    return result;
}

/**
 * @brief Generate colors maximizing the difference between similar positions
 * @param number Number of colors
 * @param saturation Satuation
 * @param value Value
 * @return Generated colors
 */
NVL_INLINE std::vector<Color> getDifferentColors(const Size& number, const int saturation, const int value)
{
    std::vector<Color> result(number);
    if (number > 0) {
        for (Index i = 0; i < number; ++i) {
            result[i] = getRangeDifferentColor(number, i, saturation, value);
        }
    }
    return result;
}

/**
 * @brief Generate colors given labels, blending from red to blue
 * @param labels Labels
 * @param saturation Satuation
 * @param value Value
 * @return Generated colors
 */
template<class T>
std::vector<Color> getRangeColorPerValue(const std::vector<T>& labels, const int saturation, const int value)
{
    std::vector<Color> result(labels.size());

    T maxLabel = -1;
    for (const T& l : labels) {
        maxLabel = std::max(maxLabel, l);
    }

    if (maxLabel >= 0) {
        for (Index i = 0; i < labels.size(); ++i) {
            result[i] = getRangeColor(maxLabel, labels[i], saturation, value);
        }
    }

    return result;
}

/**
 * @brief Generate colors given labels, maximizing the difference between similar positions
 * @param labels Labels
 * @param saturation Satuation
 * @param value Value
 * @return Generated colors
 */
template<class T>
std::vector<Color> getDifferentColorPerValue(const std::vector<T>& labels, const int saturation, const int value)
{
    std::vector<Color> result(labels.size());

    T maxLabel = -1;
    for (const T& l : labels) {
        maxLabel = std::max(maxLabel, l);
    }

    if (maxLabel >= 0) {
        for (Index i = 0; i < labels.size(); ++i) {
            result[i] = getRangeDifferentColor(maxLabel + 1, labels[i], saturation, value);
        }
    }
    return result;
}

/**
 * @brief Blend a color from red to green given a value
 * @param value Value
 * @param minValue Min value
 * @param maxValue Max value
 * @param minIntensity Min intensity color
 * @param maxIntensity Max intensity color
 * @return Generated color
 */
template<class T>
Color getRampRedGreen(const T& value, const T& minValue, const T& maxValue, const float minIntensity, const float maxIntensity)
{
    Color color(minIntensity, minIntensity, minIntensity);

    double normalizedValue = (value - minValue) / (maxValue - minValue);

    if (normalizedValue <= 0) {
        color.setRedF(maxIntensity);
    }
    else if (normalizedValue >= 1) {
        color.setGreenF(maxIntensity);
    }
    else {
        color.setRedF(minIntensity + (1.0 - normalizedValue) * (maxIntensity - minIntensity));
        color.setGreenF(minIntensity + normalizedValue * (maxIntensity - minIntensity));
    }

    return color;
}

/**
 * @brief Blend a color from red to blue given a value
 * @param value Value
 * @param minValue Min value
 * @param maxValue Max value
 * @param minIntensity Min intensity color
 * @param maxIntensity Max intensity color
 * @return Generated color
 */
template<class T>
Color getRampRedBlue(const T& value, const T& minValue, const T& maxValue, const float minIntensity, const float maxIntensity)
{
    Color color(minIntensity, minIntensity, minIntensity);

    double normalizedValue = (value - minValue) / (maxValue - minValue);

    if (normalizedValue <= 0) {
        color.setRedF(maxIntensity);
    }
    else if (normalizedValue >= 1) {
        color.setBlueF(maxIntensity);
    }
    else if (normalizedValue <= 0.5f) {
        double doubleValue = normalizedValue * 2.0;
        color.setRedF(minIntensity + (1.0 - doubleValue) * (maxIntensity - minIntensity));
        color.setGreenF(minIntensity + doubleValue * (maxIntensity - minIntensity));
    }
    else {
        double doubleValue = (normalizedValue - 0.5) * 2.0;
        color.setGreenF(minIntensity + (1.0 - doubleValue) * (maxIntensity - minIntensity));
        color.setBlueF(minIntensity + doubleValue * (maxIntensity - minIntensity));
    }

    return color;
}

}
