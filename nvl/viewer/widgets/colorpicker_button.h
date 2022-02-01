/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_COLORPICKER_BUTTON_H
#define NVL_VIEWER_COLORPICKER_BUTTON_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT

#include <QPushButton>
#include <QColor>

namespace nvl {

class ColorPickerButton : public QPushButton
{
    Q_OBJECT

public:

    ColorPickerButton(QWidget* parent);

    void setColor(const QColor& color);
    const QColor& color();
    void updateView();


private Q_SLOTS:

    void changeColor();


Q_SIGNALS:

    void colorChanged(QColor);


private:

    QColor vColor;

};

}

#endif

#include "colorpicker_button.cpp"

#endif // NVL_VIEWER_COLORPICKER_BUTTON_H
