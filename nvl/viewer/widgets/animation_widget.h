#ifndef NVL_VIEWER_ANIMATION_WIDGET_H
#define NVL_VIEWER_ANIMATION_WIDGET_H

#include <nvl/nuvolib.h>

#include <QFrame>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/interfaces/drawable.h>

namespace Ui {
class AnimationWidget;
}

namespace nvl {

class AnimationWidget : public QFrame
{
    Q_OBJECT

public:

    explicit AnimationWidget(
            Canvas* canvas,
            QWidget* parent = nullptr);
    ~AnimationWidget();


public Q_SLOTS:

    void slot_animationStarted();
    void slot_animationPaused();
    void slot_animationStopped();


private Q_SLOTS:


    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_fpsSpinBox_valueChanged(double arg1);


private:

    void updateView();

    void connectSignals();

    Ui::AnimationWidget *ui;

    Canvas* vCanvas;

    bool vHandleUpdate;

};

}

#include "animation_widget.cpp"

#endif // NVL_VIEWER_ANIMATION_WIDGET_H
