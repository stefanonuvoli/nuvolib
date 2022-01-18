#include "colorpicker_button.h"

#ifdef NVL_QT

#include <QColorDialog>

namespace nvl {

NVL_INLINE ColorPickerButton::ColorPickerButton(QWidget* parent)
    : vColor(255, 255, 255)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(parent);

    updateView();

    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

NVL_INLINE void ColorPickerButton::updateView()
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Button, vColor);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
    this->update();
}

NVL_INLINE void ColorPickerButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(vColor, parentWidget(), "Pick color", QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
    if (newColor.isValid() && newColor != vColor) {
        setColor(newColor);
        updateView();
        emit colorChanged(newColor);
    }
}

NVL_INLINE void ColorPickerButton::setColor(const QColor& color)
{
    this->vColor = color;
    updateView();
}

NVL_INLINE const QColor& ColorPickerButton::color()
{
    return vColor;
}

}

#endif

