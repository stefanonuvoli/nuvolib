#ifndef NVL_VIEWER_SKELETONJOINT_LIST_WIDGET_H
#define NVL_VIEWER_SKELETONJOINT_LIST_WIDGET_H

#include <nvl/nuvolib.h>

#include <QFrame>

#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/interfaces/drawable.h>
#include <nvl/viewer/drawables/skeleton_drawer.h>
#include <nvl/viewer/drawables/model_drawer.h>

#include <nvl/models/skeleton.h>

namespace Ui {
class SkeletonJointListWidget;
}

namespace nvl {

class SkeletonJointListWidget : public QFrame
{
    Q_OBJECT

public:

    explicit SkeletonJointListWidget(
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget* parent = nullptr);
    ~SkeletonJointListWidget();

    void updateJointList();

    Size selectedJointNumber() const;
    const std::unordered_set<Index>& selectedJoints() const;
    virtual void setSelectedJoints(const std::unordered_set<Index>& value);
    bool isJointSelected(const Index& id) const;
    virtual void selectJoint(const Index& id);
    virtual void unselectJoint(const Index& id);
    virtual void unselectAllJoints();

    SkeletonDrawer<Skeleton3d>* getSelectedSkeletonDrawer();


Q_SIGNALS:

    void signal_jointSelectionChanged(const std::unordered_set<Index>& value);


public Q_SLOTS:

    void slot_selectedDrawableUpdated();
    void slot_drawableSelectionChanged(const std::unordered_set<Index>& selectedDrawables);
private Q_SLOTS:

    void on_skeletonJointListTreeWidget_itemSelectionChanged();


private:

    void updateSelection();
    void addTreeItem(const Skeleton3d::Joint& joint);

    void connectSignals();


    Ui::SkeletonJointListWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::unordered_set<Index> vSelectedJoints;

    bool vHandleUpdate;

};

}

#include "skeletonjoint_list_widget.cpp"

#endif // NVL_VIEWER_SKELETONJOINT_LIST_WIDGET_H
