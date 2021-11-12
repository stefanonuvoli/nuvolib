#ifndef NVL_UTILITIES_COLOR_H
#define NVL_UTILITIES_COLOR_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#include <QColor>
#endif

namespace nvl {

class Color
{

public:

    /* Typedefs */

    Color();
    Color(int redI, int greenI, int blueI, int alphaI = 255);
    Color(float redF, float greenF, float blueF, float alphaI = 1.0f);
    Color(double redF, double greenF, double blueF, double alphaI = 1.0);

#ifdef NVL_QT
    Color(const QColor& color);
#endif

    int redI() const;
    void setRedI(int redF);

    int greenI() const;
    void setGreenI(int greenF);

    int blueI() const;
    void setBlueI(int blueF);

    int alphaI() const;
    void setAlphaI(int alphaI);

    float redF() const;
    void setRedF(float redF);

    float greenF() const;
    void setGreenF(float greenF);

    float blueF() const;
    void setBlueF(float blueF);

    float alphaF() const;
    void setAlphaF(float alphaI);

    void setHsv(int h, int s, int v, int alphaI = 255);
    void setHsvF(float h, float s, float v, float alphaI = 1.0);

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
