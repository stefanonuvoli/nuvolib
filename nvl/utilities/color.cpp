#include "color.h"

#include <nvl/utilities/comparators.h>

namespace nvl {

NVL_INLINE Color::Color() : Color(255, 255, 255, 255)
{

}

NVL_INLINE Color::Color(int red, int green, int blue, int alpha) :
    vRed(red), vGreen(green), vBlue(blue), vAlpha(alpha)
{

}

NVL_INLINE Color::Color(float red, float green, float blue, float alpha)
{
    setRedF(red);
    setGreenF(green);
    setBlueF(blue);
    setAlphaF(alpha);
}

NVL_INLINE Color::Color(double red, double green, double blue, double alpha)
{
    setRedF(static_cast<float>(red));
    setGreenF(static_cast<float>(green));
    setBlueF(static_cast<float>(blue));
    setAlphaF(static_cast<float>(alpha));
}

#ifdef NVL_QT
NVL_INLINE Color::Color(const QColor& color) : Color(color.red(), color.green(), color.blue(), color.alpha())
{

}
#endif

NVL_INLINE int Color::redI() const
{
    return vRed;
}

NVL_INLINE void Color::setRedI(int red)
{
    assert(red >= 0 && red <= 255);
    vRed = red;
}

NVL_INLINE int Color::greenI() const
{
    return vGreen;
}
NVL_INLINE void Color::setGreenI(int green)
{
    assert(green >= 0 && green <= 255);
    vGreen = green;
}

NVL_INLINE int Color::blueI() const
{
    return vBlue;
}

NVL_INLINE void Color::setBlueI(int blue)
{
    assert(blue >= 0 && blue <= 255);
    vBlue = blue;
}

NVL_INLINE int Color::alphaI() const
{
    return vAlpha;
}

NVL_INLINE void Color::setAlphaI(int alpha)
{
    assert(alpha >= 0 && alpha <= 255);
    vAlpha = alpha;
}

NVL_INLINE float Color::redF() const
{
    return vRed / 255.0f;
}

NVL_INLINE void Color::setRedF(float red)
{
    assert(red >= 0.0 && red <= 1.0);
    vRed = static_cast<int>(red * 255);
}

NVL_INLINE float Color::greenF() const
{
    return vGreen / 255.0f;
}

NVL_INLINE void Color::setGreenF(float green)
{
    assert(green >= 0.0 && green <= 1.0);
    vGreen = static_cast<int>(green * 255);
}

NVL_INLINE float Color::blueF() const
{
    return vBlue / 255.0f;
}

NVL_INLINE void Color::setBlueF(float blue)
{
    assert(blue >= 0.0 && blue <= 1.0);
    vBlue = static_cast<int>(blue * 255);
}

NVL_INLINE float Color::alphaF() const
{
    return vAlpha / 255.0f;
}

NVL_INLINE void Color::setAlphaF(float alpha)
{
    assert(alpha >= 0.0 && alpha <= 1.0);
    vAlpha = static_cast<int>(alpha * 255);
}

NVL_INLINE void Color::setHsv(int h, int s, int v, int alpha)
{
    assert(h >= 0 && h <= 255);
    assert(s >= 0 && s <= 255);
    assert(v >= 0 && v <= 255);
    assert(alpha >= 0 && alpha <= 255);

    vAlpha = alpha;
    if (s == 0) {
        vRed = v;
        vGreen = v;
        vBlue = v;
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
                vRed = v;
                vGreen = t;
                vBlue = p;
                break;
            case 1:
                vRed = q;
                vGreen = v;
                vBlue = p;
                break;
            case 2:
                vRed = p;
                vGreen = v;
                vBlue = t;
                break;
            case 3:
                vRed = p;
                vGreen = q;
                vBlue = v;
                break;
            case 4:
                vRed = t;
                vGreen = p;
                vBlue = v;
                break;
            default:
                vRed = v;
                vGreen = p;
                vBlue = q;
                break;
        }

    }
}

NVL_INLINE void Color::setHsvF(float h, float s, float v, float alpha)
{
    assert(h >= 0.0 && h <= 1.0);
    assert(s >= 0.0 && s <= 1.0);
    assert(v >= 0.0 && v <= 1.0);
    assert(alpha >= 0.0 && alpha <= 1.0);

    setHsv(static_cast<int>(h * 360), static_cast<int>(s * 255), static_cast<int>(v * 255), static_cast<int>(alpha * 255));
}

#ifdef NVL_QT
NVL_INLINE QColor Color::toQColor()
{
    return QColor(redI(), greenI(), blueI(), alphaI());
}
#endif

NVL_INLINE bool Color::operator<(const Color& color) const
{
    return pairwiseComparator(this->vRed, color.vRed, this->vGreen, color.vGreen, this->vBlue, color.vBlue, this->vAlpha, color.vAlpha);
}

NVL_INLINE bool Color::operator==(const Color& color) const
{
    return this->vRed == color.vRed && this->vGreen == color.vGreen && this->vBlue == color.vBlue && this->vAlpha == color.vAlpha;
}

NVL_INLINE bool Color::operator!=(const Color& color) const
{
    return !(*this == color);
}


}
