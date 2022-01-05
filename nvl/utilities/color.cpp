/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "color.h"

#include <nvl/utilities/comparators.h>

namespace nvl {

/**
 * @brief Default constructor. Default color: white.
 */
NVL_INLINE Color::Color() : Color(1.0, 1.0, 1.0, 1.0)
{

}

/**
 * @brief Constructor with int values.
 * @param red Red color, from 0 to 255
 * @param green Green color, from 0 to 255
 * @param blue Blue color, from 0 to 255
 * @param alpha Alpha value, from 0 to 255
 */
NVL_INLINE Color::Color(const int red, const int green, const int blue, const int alpha)
{
    setRedI(red);
    setGreenI(green);
    setBlueI(blue);
    setAlphaI(alpha);
}

/**
 * @brief Constructor with float values.
 * @param red Red color, from 0 to 1
 * @param green Green color, from 0 to 1
 * @param blue Blue color, from 0 to 1
 * @param alpha Alpha value, from 0 to 1
 */
NVL_INLINE Color::Color(const float red, const float green, const float blue, const float alpha)
{
    setRedF(red);
    setGreenF(green);
    setBlueF(blue);
    setAlphaF(alpha);
}

/**
 * @brief Constructor with double values.
 * @param red Red color, from 0 to 1
 * @param green Green color, from 0 to 1
 * @param blue Blue color, from 0 to 1
 * @param alpha Alpha value, from 0 to 1
 */
NVL_INLINE Color::Color(const double red, const double green, const double blue, const double alpha)
{
    setRedF(static_cast<float>(red));
    setGreenF(static_cast<float>(green));
    setBlueF(static_cast<float>(blue));
    setAlphaF(static_cast<float>(alpha));
}

#ifdef NVL_QT
/**
 * @brief Consttructor with QColor
 * @param color QColor
 */
NVL_INLINE Color::Color(const QColor& color) : Color(color.red(), color.green(), color.blue(), color.alpha())
{

}
#endif

/**
 * @brief Integer red getter
 * @return Red integer value
 */
NVL_INLINE int Color::redI() const
{
    return vRed * 255;
}

/**
 * @brief Integer red setter
 * @param red Integer red color value, from 0 to 255
 */
NVL_INLINE void Color::setRedI(const int red)
{
    assert(red >= 0 && red <= 255);
    vRed = red / 255.0f;
}

/**
 * @brief Integer green getter
 * @return Green integer value
 */
NVL_INLINE int Color::greenI() const
{
    return vGreen * 255;
}

/**
 * @brief Integer green setter
 * @param green Integer green color value, from 0 to 255
 */
NVL_INLINE void Color::setGreenI(const int green)
{
    assert(green >= 0 && green <= 255);
    vGreen = green / 255.0f;
}

/**
 * @brief Integer blue getter
 * @return Blue integer value
 */
NVL_INLINE int Color::blueI() const
{
    return vBlue * 255;
}

/**
 * @brief Integer blue setter
 * @param blue Integer blue color value, from 0 to 255
 */
NVL_INLINE void Color::setBlueI(const int blue)
{
    assert(blue >= 0 && blue <= 255);
    vBlue = blue / 255.0f;
}

/**
 * @brief Integer alpha getter
 * @return Alpha integer value
 */
NVL_INLINE int Color::alphaI() const
{
    return vAlpha * 255;
}

/**
 * @brief Integer alpha setter
 * @param alpha Integer alpha color value, from 0 to 255
 */
NVL_INLINE void Color::setAlphaI(const int alpha)
{
    assert(alpha >= 0 && alpha <= 255);
    vAlpha = alpha / 255.0f;
}

/**
 * @brief Float red getter
 * @return Red float value
 */
NVL_INLINE float Color::redF() const
{
    return vRed;
}

/**
 * @brief Float alpha setter
 * @param red Float red color value, from 0 to 1
 */
NVL_INLINE void Color::setRedF(const float red)
{
    assert(red >= 0.0 && red <= 1.0);
    vRed = red;
}

/**
 * @brief Float green getter
 * @return Green float value
 */
NVL_INLINE float Color::greenF() const
{
    return vGreen;
}

/**
 * @brief Float green setter
 * @param green Float green color value, from 0 to 1
 */
NVL_INLINE void Color::setGreenF(const float green)
{
    assert(green >= 0.0 && green <= 1.0);
    vGreen = green;
}

/**
 * @brief Float blue getter
 * @return Blue float value
 */
NVL_INLINE float Color::blueF() const
{
    return vBlue;
}

/**
 * @brief Float blue setter
 * @param blue Float blue color value, from 0 to 1
 */
NVL_INLINE void Color::setBlueF(const float blue)
{
    assert(blue >= 0.0 && blue <= 1.0);
    vBlue = blue;
}

/**
 * @brief Float alpha getter
 * @return Alpha float value
 */
NVL_INLINE float Color::alphaF() const
{
    return vAlpha;
}

/**
 * @brief Float alpha setter
 * @param alpha Float alpha color value, from 0 to 1
 */
NVL_INLINE void Color::setAlphaF(const float alpha)
{
    assert(alpha >= 0.0 && alpha <= 1.0);
    vAlpha = alpha;
}

/**
 * @brief Set color given hue, saturation, value integer values
 * @param h Hue
 * @param s Saturation
 * @param v Value
 * @param alpha Alpha value
 */
NVL_INLINE void Color::setHsv(const int h, const int s, const int v, const int alpha)
{
    assert(h >= 0 && h <= 360);
    assert(s >= 0 && s <= 255);
    assert(v >= 0 && v <= 255);
    assert(alpha >= 0 && alpha <= 255);

    vAlpha = alpha;
    if (s == 0) {
        vRed = v / 255.0f;
        vGreen = v / 255.0f;
        vBlue = v / 255.0f;
    }
    else {
        int region, remainder, p, q, t;
        region = h / 43;
        remainder = (h - (region * 43)) * 6;

        p = (v * (255 - s)) >> 8;
        q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

        switch (region) {
            case 0:
                vRed = v / 255.0f;
                vGreen = t / 255.0f;
                vBlue = p / 255.0f;
                break;
            case 1:
                vRed = q / 255.0f;
                vGreen = v / 255.0f;
                vBlue = p / 255.0f;
                break;
            case 2:
                vRed = p / 255.0f;
                vGreen = v / 255.0f;
                vBlue = t / 255.0f;
                break;
            case 3:
                vRed = p / 255.0f;
                vGreen = q / 255.0f;
                vBlue = v / 255.0f;
                break;
            case 4:
                vRed = t / 255.0f;
                vGreen = p / 255.0f;
                vBlue = v / 255.0f;
                break;
            default:
                vRed = v / 255.0f;
                vGreen = p / 255.0f;
                vBlue = q / 255.0f;
                break;
        }
    }
}

/**
 * @brief Set color given hue, saturation, value float values
 * @param h Hue
 * @param s Saturation
 * @param v Value
 * @param alpha Alpha value
 */
NVL_INLINE void Color::setHsvF(const float h, const float s, const float v, const float alpha)
{
    assert(h >= 0.0 && h <= 1.0);
    assert(s >= 0.0 && s <= 1.0);
    assert(v >= 0.0 && v <= 1.0);
    assert(alpha >= 0.0 && alpha <= 1.0);

    setHsv(static_cast<int>(h * 360), static_cast<int>(s * 255), static_cast<int>(v * 255), static_cast<int>(alpha * 255));
}

#ifdef NVL_QT
/**
 * @brief Get the QColor
 * @return Corresponding QColor
 */
NVL_INLINE QColor Color::toQColor()
{
    return QColor(redI(), greenI(), blueI(), alphaI());
}
#endif

/**
 * @brief Less comparator
 * @param color Color to compare
 * @return True if the current color is less than the given color
 */
NVL_INLINE bool Color::operator<(const Color& color) const
{
    return pairwiseComparator(this->vRed, color.vRed, this->vGreen, color.vGreen, this->vBlue, color.vBlue, this->vAlpha, color.vAlpha);
}

/**
 * @brief Equality comparator
 * @param color Color to compare
 * @return True if the color are equal
 */
NVL_INLINE bool Color::operator==(const Color& color) const
{
    return this->vRed == color.vRed && this->vGreen == color.vGreen && this->vBlue == color.vBlue && this->vAlpha == color.vAlpha;
}

/**
 * @brief Inequality comparator
 * @param color Color to compare
 * @return True if the color are not equal
 */
NVL_INLINE bool Color::operator!=(const Color& color) const
{
    return !(*this == color);
}


}
