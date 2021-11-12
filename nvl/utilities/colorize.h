#ifndef NVL_UTILITIES_COLORIZE_H
#define NVL_UTILITIES_COLORIZE_H

#include <nvl/nuvolib.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

Color getRangeColor(const Size& range, const Index& position, const int saturation = 131, const int value = 223);
Color getRangeDifferentColor(const Size& range, const Index& position, const int saturation = 131, const int value = 223);

std::vector<Color> getColors(const Size& number, const int saturation = 131, const int value = 223);
std::vector<Color> getDifferentColors(const Size& number, const int saturation = 131, const int value = 223);

template<class T>
std::vector<Color> getColorPerValue(const std::vector<T>& labels, const int saturation = 131, const int value = 223);
template<class T>
std::vector<Color> getDifferentColorPerValue(const std::vector<T>& labels, const int saturation = 131, const int value = 223);

template<class T>
Color getRampRedGreen(const T& value, const T& minValue = 0.0, const T& maxValue = 1.0, const float minIntensity = 0.0, const float maxIntensity = 1.0);
template<class T>
Color getRampRedBlue(const T& value, const T& minValue = 0.0, const T& maxValue = 1.0, const float minIntensity = 0.0, const float maxIntensity = 1.0);

}

#include "colorize.cpp"

#endif // NVL_UTILITIES_COLORIZE_H
