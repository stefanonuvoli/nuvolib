#ifndef NVL_VIEWER_DRAWABLE_WIDGET_H
#define NVL_VIEWER_DRAWABLE_WIDGET_H

#include <nvl/nuvolib.h>

#include <QFrame>

#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>

namespace Ui {
class DrawableWidget;
}

namespace nvl {

class DrawableWidget : public QFrame
{
    Q_OBJECT

public:

    explicit DrawableWidget(
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget* parent = nullptr);
    ~DrawableWidget();


public Q_SLOTS:

    void slot_drawableListChanged();
    void slot_drawableSelectionChanged();


private Q_SLOTS:

    void on_visibleCheckBox_stateChanged(int arg1);


    void on_resetFrameButton_clicked();

private:

    void updateView();

    void connectSignals();

    Ui::DrawableWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    bool vHandleUpdate;

};

}

#include "drawable_widget.cpp"

#endif // NVL_VIEWER_DRAWABLE_WIDGET_H
