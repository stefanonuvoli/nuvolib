/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTILITIES_COLOR_H
#define NVL_UTILITIES_COLOR_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#include <QColor>
#endif

namespace nvl {

/**
 * @brief A generic color.
 */
class Color
{

public:

    /* Typedefs */

    Color();
    Color(const int redI, const int greenI, const int blueI, const int alphaI = 255);
    Color(const float redF, const float greenF, const float blueF, const float alphaI = 1.0f);
    Color(const double redF, const double greenF, const double blueF, const double alphaI = 1.0);

#ifdef NVL_QT
    Color(const QColor& color);
#endif

    int redI() const;
    void setRedI(const int redF);

    int greenI() const;
    void setGreenI(const int greenF);

    int blueI() const;
    void setBlueI(const int blueF);

    int alphaI() const;
    void setAlphaI(const int alphaI);

    float redF() const;
    void setRedF(const float redF);

    float greenF() const;
    void setGreenF(const float greenF);

    float blueF() const;
    void setBlueF(const float blueF);

    float alphaF() const;
    void setAlphaF(const float alphaI);

    void setHsv(int h, int s, int v, int alphaI = 255);
    void setHsvF(const float h, const float s, const float v, const float alphaI = 1.0f);

#ifdef NVL_QT
    QColor toQColor();
#endif

    bool operator<(const Color& color) const;
    bool operator==(const Color& color) const;
    bool operator!=(const Color& color) const;

private:

    int vRed;
    int vGreen;
    int vBlue;
    int vAlpha;

};

const Color WHITE = Color(255,255,255);
const Color BLACK = Color(0, 0, 0);
const Color RED = Color(255, 0, 0);
const Color GREEN = Color(0, 255, 0);
const Color BLUE = Color(0, 0, 255);
const Color YELLOW = Color(1, 255, 0);
const Color GREY = Color(127, 127, 127);
const Color LIGHTGREY = Color(191, 191, 191);

}

#include "color.cpp"

#endif // NVL_UTILITIES_COLOR_H
