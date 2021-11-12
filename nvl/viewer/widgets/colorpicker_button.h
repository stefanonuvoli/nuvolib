#ifndef NVL_VIEWER_COLORPICKER_BUTTON_H
#define NVL_VIEWER_COLORPICKER_BUTTON_H

#include <nvl/nuvolib.h>

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

#include "colorpicker_button.cpp"

#endif // NVL_VIEWER_COLORPICKER_BUTTON_H
