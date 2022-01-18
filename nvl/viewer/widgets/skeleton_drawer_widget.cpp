#include "skeleton_drawer_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED
#include "ui_skeleton_drawer_widget.h"

#include <nvl/models/model_3d.h>

namespace nvl {

NVL_INLINE SkeletonDrawerWidget::SkeletonDrawerWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SkeletonDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE SkeletonDrawerWidget::~SkeletonDrawerWidget()
{
    delete ui;
}

NVL_INLINE void SkeletonDrawerWidget::slot_drawableSelectionChanged()
{
    getSelectedDrawers();

    updateView();
}

NVL_INLINE void SkeletonDrawerWidget::slot_selectedDrawableUpdated()
{
    updateView();
}

NVL_INLINE void SkeletonDrawerWidget::updateView()
{
    vHandleUpdate = false;

    updateSkeletonView();

    vHandleUpdate = true;
}

NVL_INLINE void SkeletonDrawerWidget::updateSkeletonView()
{
    assert(!vHandleUpdate);
    bool tabEnabled = !vSkeletonDrawers.empty() && vCanvas != nullptr;
    bool skeletonVisibleChecked = tabEnabled;
    bool jointVisibleChecked = tabEnabled;
    bool boneVisibleChecked = tabEnabled;
    int jointSize = 1;
    int boneSize = 1;
    Color jointColor(255,255,255);
    Color rootColor(255,255,255);
    Color boneColor(255,255,255);
    bool transparencyChecked = tabEnabled;

    if (tabEnabled) {
        bool first = true;

        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonVisibleChecked &= skeletonDrawer->skeletonVisible();
            jointVisibleChecked &= skeletonDrawer->jointVisible();
            boneVisibleChecked &= skeletonDrawer->boneVisible();
            jointSize = (first || jointSize == skeletonDrawer->jointSize() ? skeletonDrawer->jointSize() : 1);
            boneSize = (first || boneSize == skeletonDrawer->boneSize() ? skeletonDrawer->boneSize() : 1);
            jointColor = (first || jointColor == skeletonDrawer->jointColor() ? skeletonDrawer->jointColor() : Color(255,255,255));
            rootColor = (first || rootColor == skeletonDrawer->rootColor() ? skeletonDrawer->rootColor() : Color(255,255,255));
            boneColor = (first || boneColor == skeletonDrawer->boneColor() ? skeletonDrawer->boneColor() : Color(255,255,255));
            transparencyChecked &= skeletonDrawer->transparency();

            first = false;
        }
    }

    this->setEnabled(tabEnabled);
    ui->skeletonVisibleCheckBox->setChecked(skeletonVisibleChecked);
    ui->skeletonJointVisibleCheckBox->setChecked(jointVisibleChecked);
    ui->skeletonBoneVisibleCheckBox->setChecked(boneVisibleChecked);
    ui->skeletonJointSlider->setValue(jointSize);
    ui->skeletonBoneSlider->setValue(boneSize);
    ui->skeletonJointColorButton->setColor(jointColor.toQColor());
    ui->skeletonRootColorButton->setColor(rootColor.toQColor());
    ui->skeletonBoneColorButton->setColor(boneColor.toQColor());
    ui->skeletonTransparencyCheckBox->setChecked(transparencyChecked);
}

NVL_INLINE void SkeletonDrawerWidget::getSelectedDrawers()
{
    vSkeletonDrawers.clear();

    if (vCanvas != nullptr) {
        for (const Index drawableId : vDrawableListWidget->selectedDrawables()) {
            Drawable* drawable = vCanvas->drawable(drawableId);

            Drawable* skeletonDrawerWrapper;
            ModelDrawer<Model3d>* modelDrawer = dynamic_cast<ModelDrawer<Model3d>*>(drawable);
            if (modelDrawer != nullptr) {
                skeletonDrawerWrapper = &modelDrawer->skeletonDrawer();
            }
            else {
                skeletonDrawerWrapper = drawable;
            }

            SkeletonDrawerBase* skeletonDrawer = dynamic_cast<SkeletonDrawerBase*>(skeletonDrawerWrapper);
            if (skeletonDrawer != nullptr) {
                vSkeletonDrawers.push_back(skeletonDrawer);
            }
        }
    }
}

NVL_INLINE void SkeletonDrawerWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &SkeletonDrawerWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setSkeletonVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonJointVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonJointSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonJointColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}
NVL_INLINE void SkeletonDrawerWidget::on_skeletonRootColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setRootColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonBoneVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonBoneSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void SkeletonDrawerWidget::on_skeletonBoneColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}

}

#endif
#endif
