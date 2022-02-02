/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "color_utils.h"

#include <nvl/math/interpolation.h>

namespace nvl {

/**
 * @brief Interpolate color given two colors and a alpha value
 * @param color1 Color 1
 * @param color2 Color 2
 * @param alpha Alpha value for interpolation
 * @return Interpolated color
 */
NVL_INLINE Color colorInterpolation(
        const Color& color1,
        const Color& color2,
        const double& alpha)
{
    Color color;

    color.setRedF(interpolateLinear(color1.redF(), color2.redF(), alpha));
    color.setGreenF(interpolateLinear(color1.greenF(), color2.greenF(), alpha));
    color.setBlueF(interpolateLinear(color1.blueF(), color2.blueF(), alpha));
    color.setAlphaF(interpolateLinear(color1.alphaF(), color2.alphaF(), alpha));

    return color;
}

/**
 * @brief Interpolate color given two colors and a alpha value
 * @param color1 Color 1
 * @param color2 Color 2
 * @param alpha Alpha value for interpolation
 * @return Interpolated color
 */
NVL_INLINE Color colorInterpolation(
        const std::vector<Color>& colors,
        const std::vector<double>& alphas)
{
    Color color;

    std::vector<float> redValues(colors.size());
    std::vector<float> greenValues(colors.size());
    std::vector<float> blueValues(colors.size());
    std::vector<float> alphaValues(colors.size());

    for (Index i = 0; i < colors.size(); ++i) {
        redValues[i] = colors[i].redF();
        greenValues[i] = colors[i].greenF();
        blueValues[i] = colors[i].blueF();
        alphaValues[i] = colors[i].alphaF();
    }

    color.setRedF(interpolateLinear(redValues, alphas));
    color.setGreenF(interpolateLinear(greenValues, alphas));
    color.setBlueF(interpolateLinear(blueValues, alphas));
    color.setAlphaF(interpolateLinear(alphaValues, alphas));

    return color;
}


/**
 * @brief Generate different colors, blending from red to blue
 * @param range Number of possible values
 * @param position Value in the range
 * @param saturation Satuation
 * @param value Value
 * @return Generated color for the given position
 */
NVL_INLINE Color colorRange(const Size& range, const Index& position, const int saturation, const int value)
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
NVL_INLINE Color colorRangeDifferent(const Size& range, const Index& position, const int saturation, const int value)
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

    return colorRange(range, position, saturation, value);
}

/**
 * @brief Generate different colors, blending from red to blue
 * @param number Number of colors
 * @param saturation Satuation
 * @param value Value
 * @return Generated colors
 */
NVL_INLINE std::vector<Color> colorRange(const Size& number, const int saturation, const int value)
{
    std::vector<Color> result(number);
    if (number > 0) {
        for (Index i = 0; i < number; ++i) {
            result[i] = colorRange(number + 1, i, saturation, value);
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
NVL_INLINE std::vector<Color> colorDifferent(const Size& number, const int saturation, const int value)
{
    std::vector<Color> result(number);
    if (number > 0) {
        for (Index i = 0; i < number; ++i) {
            result[i] = colorRangeDifferent(number, i, saturation, value);
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
std::vector<Color> colorRangePerValue(const std::vector<T>& labels, const int saturation, const int value)
{
    std::vector<Color> result(labels.size());

    T maxLabel = -1;
    for (const T& l : labels) {
        maxLabel = std::max(maxLabel, l);
    }

    if (maxLabel >= 0) {
        for (Index i = 0; i < labels.size(); ++i) {
            result[i] = colorRange(maxLabel, labels[i], saturation, value);
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
std::vector<Color> colorDifferentPerValue(const std::vector<T>& labels, const int saturation, const int value)
{
    std::vector<Color> result(labels.size());

    T maxLabel = -1;
    for (const T& l : labels) {
        maxLabel = std::max(maxLabel, l);
    }

    if (maxLabel >= 0) {
        for (Index i = 0; i < labels.size(); ++i) {
            result[i] = colorRangeDifferent(maxLabel + 1, labels[i], saturation, value);
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
Color colorRampRedGreen(const T& value, const T& minValue, const T& maxValue, const float minIntensity, const float maxIntensity)
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
Color colorRampRedBlue(const T& value, const T& minValue, const T& maxValue, const float minIntensity, const float maxIntensity)
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

/**
 * @brief Blend a color from green to blue given a value
 * @param value Value
 * @param minValue Min value
 * @param maxValue Max value
 * @param minIntensity Min intensity color
 * @param maxIntensity Max intensity color
 * @return Generated color
 */
template<class T>
Color colorRampGreenBlue(const T& value, const T& minValue, const T& maxValue, const float minIntensity, const float maxIntensity)
{
    Color color(minIntensity, minIntensity, minIntensity);

    double normalizedValue = (value - minValue) / (maxValue - minValue);

    if (normalizedValue <= 0) {
        color.setGreenF(maxIntensity);
    }
    else if (normalizedValue >= 1) {
        color.setBlueF(maxIntensity);
    }
    else {
        color.setGreenF(minIntensity + (1.0 - normalizedValue) * (maxIntensity - minIntensity));
        color.setBlueF(minIntensity + normalizedValue * (maxIntensity - minIntensity));
    }

    return color;
}

}
