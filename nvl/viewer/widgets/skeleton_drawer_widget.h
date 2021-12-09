#ifndef NVL_VIEWER_SKELETON_DRAWER_WIDGET_H
#define NVL_VIEWER_SKELETON_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/qcanvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/skeleton_drawer.h>

#include <QFrame>

namespace Ui {
class SkeletonDrawerWidget;
}

namespace nvl {

class SkeletonDrawerWidget : public QFrame
{

    Q_OBJECT

public:

    explicit SkeletonDrawerWidget(
            QCanvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~SkeletonDrawerWidget();

public Q_SLOTS:

    void slot_drawableSelectionChanged();
    void slot_selectedDrawableUpdated();


private Q_SLOTS:

    void on_skeletonVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonTransparencyCheckBox_stateChanged(int arg1);
    void on_skeletonJointVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonJointSlider_valueChanged(int value);
    void on_skeletonJointColorButton_colorChanged(QColor color);
    void on_skeletonRootColorButton_colorChanged(QColor color);
    void on_skeletonBoneSlider_valueChanged(int value);
    void on_skeletonBoneVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonBoneColorButton_colorChanged(QColor color);

private:

    void updateView();

    void updateSkeletonView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::SkeletonDrawerWidget *ui;

    QCanvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<SkeletonDrawerBase*> vSkeletonDrawers;

    bool vHandleUpdate;
};

}

#include "skeleton_drawer_widget.cpp"

#endif // NVL_VIEWER_SKELETON_DRAWER_WIDGET_H
