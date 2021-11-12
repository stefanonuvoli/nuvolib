#include "skeletonjoint_list_widget.h"
#include "ui_skeletonjoint_list_widget.h"

#include <nvl/viewer/drawables/model_drawer.h>
#include <nvl/models/model.h>

namespace nvl {

NVL_INLINE SkeletonJointListWidget::SkeletonJointListWidget(
        Canvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget* parent) :
    QFrame(parent),
    ui(new Ui::SkeletonJointListWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);
    ui->skeletonJointListTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    updateJointList();

    connectSignals();
}

NVL_INLINE SkeletonJointListWidget::~SkeletonJointListWidget()
{
    delete ui;
}

NVL_INLINE void SkeletonJointListWidget::updateJointList()
{
    vHandleUpdate = false;

    ui->skeletonJointListTreeWidget->clear();

    SkeletonDrawer<Skeleton3d>* selectedDrawer = getSelectedSkeletonDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr) {
        ui->skeletonJointListTreeWidget->setEnabled(true);

        for (const typename Skeleton3d::Joint& joint : selectedDrawer->skeleton()->joints()) {
            addTreeItem(joint);
        }

        for (const Index& id : vSelectedJoints) {
            ui->skeletonJointListTreeWidget->topLevelItem(id)->setSelected(true);
        }
    }
    else {
        ui->skeletonJointListTreeWidget->setEnabled(false);
    }

    ui->skeletonJointListTreeWidget->update();
    ui->skeletonJointListTreeWidget->repaint();

    vHandleUpdate = true;
}

NVL_INLINE Size SkeletonJointListWidget::selectedJointNumber() const
{
    return vSelectedJoints.size();
}

NVL_INLINE const std::unordered_set<Index>& SkeletonJointListWidget::selectedJoints() const
{
    return vSelectedJoints;
}

NVL_INLINE void SkeletonJointListWidget::setSelectedJoints(const std::unordered_set<Index>& selectedJoints)
{
    vSelectedJoints = selectedJoints;
    updateSelection();

    emit signal_jointSelectionChanged(vSelectedJoints);
}

NVL_INLINE bool SkeletonJointListWidget::isJointSelected(const Index& id) const
{
    return vSelectedJoints.find(id) != vSelectedJoints.end();
}

NVL_INLINE void SkeletonJointListWidget::selectJoint(const Index& id)
{
    vSelectedJoints.insert(id);
    updateSelection();

    emit signal_jointSelectionChanged(vSelectedJoints);
}

NVL_INLINE void SkeletonJointListWidget::unselectJoint(const Index& id)
{
    vSelectedJoints.erase(id);
    updateSelection();

    emit signal_jointSelectionChanged(vSelectedJoints);
}

NVL_INLINE void SkeletonJointListWidget::unselectAllJoints()
{
    vSelectedJoints.clear();
    emit signal_jointSelectionChanged(vSelectedJoints);
}

NVL_INLINE SkeletonDrawer<Skeleton3d>* SkeletonJointListWidget::getSelectedSkeletonDrawer()
{
    SkeletonDrawer<Skeleton3d>* selectedSkeletonDrawer = nullptr;

    const std::unordered_set<Index>& selectedDrawables = vDrawableListWidget->selectedDrawables();
    if (selectedDrawables.size() == 1) {
        Index firstItem = *selectedDrawables.begin();
        Drawable* drawable = vCanvas->drawable(firstItem);

        ModelDrawer<Model3d>* modelDrawer = dynamic_cast<ModelDrawer<Model3d>*>(drawable);
        if (modelDrawer != nullptr) {
            selectedSkeletonDrawer = &modelDrawer->skeletonDrawer();
        }
        else {
            SkeletonDrawer<Skeleton3d>* skeletonDrawer = dynamic_cast<SkeletonDrawer<Skeleton3d>*>(drawable);
            if (skeletonDrawer != nullptr) {
                selectedSkeletonDrawer = skeletonDrawer;
            }
        }
    }

    return selectedSkeletonDrawer;
}

NVL_INLINE void SkeletonJointListWidget::slot_selectedDrawableUpdated()
{
    vSelectedJoints.clear();
    updateJointList();
}

NVL_INLINE void SkeletonJointListWidget::slot_drawableSelectionChanged(const std::unordered_set<Index>& selectedDrawables)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(selectedDrawables);

    vSelectedJoints.clear();
    updateJointList();
}

NVL_INLINE void SkeletonJointListWidget::updateSelection()
{
    vHandleUpdate = false;

    SkeletonDrawer<Skeleton3d>* selectedDrawer = getSelectedSkeletonDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr) {
        for (Index i = 0; i < selectedDrawer->skeleton()->jointNumber(); ++i) {
            bool check = vSelectedJoints.find(i) != vSelectedJoints.end();
            ui->skeletonJointListTreeWidget->topLevelItem(i)->setSelected(check);
        }

        ui->skeletonJointListTreeWidget->update();
        ui->skeletonJointListTreeWidget->repaint();
    }
    else {
        updateJointList();
    }

    vHandleUpdate = true;
}

NVL_INLINE void SkeletonJointListWidget::addTreeItem(const Skeleton3d::Joint& joint)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString(std::to_string(joint.id()).c_str()));
    item->setText(1, QString(joint.name().c_str()));
    ui->skeletonJointListTreeWidget->addTopLevelItem(item);
}

NVL_INLINE void SkeletonJointListWidget::connectSignals()
{
    if (vDrawableListWidget != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &SkeletonJointListWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void SkeletonJointListWidget::on_skeletonJointListTreeWidget_itemSelectionChanged()
{
    if (vHandleUpdate) {
        QList<QTreeWidgetItem*> items = ui->skeletonJointListTreeWidget->selectedItems();

        vSelectedJoints.clear();

        for (int i = 0; i < items.size(); ++i) {
            QTreeWidgetItem* item = items[i];
            int id = ui->skeletonJointListTreeWidget->indexOfTopLevelItem(item);

            if (id >= 0) {
                vSelectedJoints.insert(static_cast<Index>(id));
            }
        }

        emit signal_jointSelectionChanged(vSelectedJoints);
    }
}

}

