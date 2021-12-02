#ifndef NVL_VIEWER_MODEL_ANIMATION_WIDGET_H
#define NVL_VIEWER_MODEL_ANIMATION_WIDGET_H

#include <nvl/nuvolib.h>

#include <QFrame>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/model_drawer.h>

#include <nvl/models/model_3d.h>

namespace Ui {
class ModelAnimationWidget;
}

namespace nvl {

class ModelAnimationWidget : public QFrame
{
    Q_OBJECT

public:

    explicit ModelAnimationWidget(
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget* parent = nullptr);
    ~ModelAnimationWidget();

    Index selectedAnimation() const;
    void selectAnimation(const Index& id);
    void unselectAnimation();
    void updateView();

    ModelDrawer<Model3d>* getSelectedModelDrawer() const;


public Q_SLOTS:

    void slot_selectedDrawableUpdated();
    void slot_drawableSelectionChanged();
    void slot_animationStarted();
    void slot_animationPaused();
    void slot_animationStopped();


private Q_SLOTS:

    void on_animationTreeWidget_itemSelectionChanged();

    void on_LBSButton_clicked();
    void on_DQSButton_clicked();
    void on_speedSpinBox_valueChanged(double arg1);
    void on_loopCheckBox_stateChanged(int arg1);
    void on_fpsSpinBox_valueChanged(double arg1);
    void on_blendCheckBox_stateChanged(int arg1);
    void on_keepKeyframesCheckBox_stateChanged(int arg1);
    void on_keyframeSlider_valueChanged(int value);

private:

    void updateAnimationList();
    void updateOptions();
    void updateSelection();

    void reloadAnimation();

    void addTreeItem(const Model3d::Animation& joint);

    void connectSignals();

    Ui::ModelAnimationWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    bool vHandleUpdate;

};

}

#include "model_animation_widget.cpp"

#endif // NVL_VIEWER_MODEL_ANIMATION_WIDGET_H