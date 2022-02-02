/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTITLIES_COLOR_UTILS_H
#define NVL_UTITLIES_COLOR_UTILS_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/utilities/color.h>

namespace nvl {

Color colorInterpolation(
        const Color& color1,
        const Color& color2,
        const double& alpha);

Color colorInterpolation(
        const std::vector<Color>& colors,
        const std::vector<double>& alphas);

Color colorRange(const Size& range, const Index& position, const int saturation = 131, const int value = 223);
Color colorRangeDifferent(const Size& range, const Index& position, const int saturation = 131, const int value = 223);

std::vector<Color> colorRange(const Size& number, const int saturation = 131, const int value = 223);
std::vector<Color> colorDifferent(const Size& number, const int saturation = 131, const int value = 223);

template<class T>
std::vector<Color> colorRangePerValue(const std::vector<T>& labels, const int saturation = 131, const int value = 223);
template<class T>
std::vector<Color> colorDifferentPerValue(const std::vector<T>& labels, const int saturation = 131, const int value = 223);

template<class T>
Color colorRampRedGreen(const T& value, const T& minValue = 0.0, const T& maxValue = 1.0, const float minIntensity = 0.0, const float maxIntensity = 1.0);
template<class T>
Color colorRampRedBlue(const T& value, const T& minValue = 0.0, const T& maxValue = 1.0, const float minIntensity = 0.0, const float maxIntensity = 1.0);
template<class T>
Color colorRampGreenBlue(const T& value, const T& minValue = 0.0, const T& maxValue = 1.0, const float minIntensity = 0.0, const float maxIntensity = 1.0);

}

#include "color_utils.cpp"

#endif // NVL_UTITLIES_COLOR_UTILS_H
