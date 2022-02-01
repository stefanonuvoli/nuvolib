/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_animation_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_model_animation_widget.h"

namespace nvl {

NVL_INLINE ModelAnimationWidget::ModelAnimationWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget* parent) :
    QFrame(parent),
    ui(new Ui::ModelAnimationWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);
    ui->animationTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connectSignals();

    updateAnimationList();
    updateSelection();
    updateOptions();
}

NVL_INLINE Index ModelAnimationWidget::selectedAnimation() const
{
    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr) {
        return selectedDrawer->loadedAnimation();
    }
    return NULL_ID;
}

NVL_INLINE Index ModelAnimationWidget::currentAnimationFrame() const
{
    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr && selectedDrawer->loadedAnimation() != NULL_ID) {
        return selectedDrawer->currentFrameId();
    }
    return NULL_ID;
}

NVL_INLINE void ModelAnimationWidget::selectAnimation(const Index& id)
{
    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr) {
        if (id == NULL_ID)
            selectedDrawer->unloadAnimation();
        else
            selectedDrawer->loadAnimation(id);
    }
    updateAnimationList();
    updateSelection();
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::unselectAnimation()
{
    selectAnimation(NULL_ID);
}

NVL_INLINE void ModelAnimationWidget::updateView()
{
    updateAnimationList();
    updateSelection();
    updateOptions();
}

NVL_INLINE ModelAnimationWidget::~ModelAnimationWidget()
{
    delete ui;
}

NVL_INLINE ModelDrawer<Model3d>* ModelAnimationWidget::getSelectedModelDrawer() const
{
    ModelDrawer<Model3d>* selectedDrawer = nullptr;
    const std::unordered_set<Index>& selectedItems = vDrawableListWidget->selectedDrawables();
    if (selectedItems.size() == 1) {
        Index firstItem = *selectedItems.begin();
        Drawable* drawable = vCanvas->drawable(firstItem);

        ModelDrawer<Model3d>* modelDrawer = dynamic_cast<ModelDrawer<Model3d>*>(drawable);
        if (modelDrawer != nullptr) {
            selectedDrawer = modelDrawer;
        }
    }
    return selectedDrawer;
}

NVL_INLINE void ModelAnimationWidget::slot_selectedDrawableUpdated()
{
    updateAnimationList();
    updateSelection();
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::slot_drawableSelectionChanged()
{
    updateAnimationList();
    updateSelection();
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::slot_animationStarted()
{
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::slot_animationStopped()
{
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::slot_animationPaused()
{
    updateOptions();
}

NVL_INLINE void ModelAnimationWidget::updateAnimationList()
{
    vHandleUpdate = false;

    ui->animationTreeWidget->clear();

    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    if (vCanvas != nullptr && selectedDrawer != nullptr) {
        ui->animationTreeWidget->setEnabled(true);

        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, QString("None"));
        ui->animationTreeWidget->addTopLevelItem(item);

        for (typename Model3d::Animation animation : selectedDrawer->model()->animations) {
            addTreeItem(animation);
        }
    }
    else {
        ui->animationTreeWidget->setEnabled(false);
    }

    ui->animationTreeWidget->update();
    ui->animationTreeWidget->repaint();

    vHandleUpdate = true;
}

NVL_INLINE void ModelAnimationWidget::updateOptions()
{
    vHandleUpdate = false;

    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    bool drawableSelected = vCanvas != nullptr && selectedDrawer != nullptr;
    bool animationRunning = drawableSelected && vCanvas->animationsRunning();
    bool loadedAnimation = drawableSelected && selectedDrawer->loadedAnimation() != NULL_ID;

    double animationSpeed = 1.0;
    bool skinningDQ = false;
    bool skinningLB = false;
    bool loop = false;
    Index currentKeyframe = NULL_ID;
    int fps = 30;
    bool blend = true;
    bool keepKeyframe = true;
    double currentTime = 0.0;

    if (drawableSelected) {
        animationSpeed = selectedDrawer->animationSpeed();
        skinningDQ = selectedDrawer->animationSkinningMode() == ModelDrawerBase::SKINNING_DUAL_QUATERNIONS;
        skinningLB = selectedDrawer->animationSkinningMode() == ModelDrawerBase::SKINNING_LINEAR_BLENDING;
        loop = selectedDrawer->animationLoop();
        if (selectedDrawer->isAnimationLoaded()) {
            currentKeyframe = selectedDrawer->currentFrameId();
            currentTime = selectedDrawer->currentFrame().time();
        }
        fps = selectedDrawer->animationTargetFPS();
        blend = selectedDrawer->animationBlend();
        keepKeyframe = selectedDrawer->animationKeepKeyframes();
    }

    ui->animationTreeWidget->setEnabled(drawableSelected);
    ui->settingFrame->setEnabled(drawableSelected);
    ui->fpsSpinBox->setEnabled(blend);
    ui->keepKeyframesCheckBox->setEnabled(blend);

    ui->speedSpinBox->setValue(animationSpeed);
    ui->DQSButton->setChecked(skinningDQ);
    ui->LBSButton->setChecked(skinningLB);
    ui->loopCheckBox->setChecked(loop);
    ui->fpsSpinBox->setValue(fps);
    ui->blendCheckBox->setChecked(blend);
    ui->keepKeyframesCheckBox->setChecked(keepKeyframe);

    ui->keyframeSlider->setEnabled(loadedAnimation && !animationRunning);
    ui->keyframeSlider->setMinimum(0);
    if (drawableSelected) {
        ui->keyframeSlider->setMaximum(selectedDrawer->frameNumber() - 1);
    }
    else {
        ui->keyframeSlider->setMaximum(0);
    }
    ui->keyframeSlider->setValue(currentKeyframe);

    if (currentKeyframe != NULL_ID) {
        const std::string frameInfo = std::to_string(currentKeyframe) + std::string(" - ") + std::to_string(currentTime);
        ui->frameInfoLabel->setText(frameInfo.c_str());
    }
    else {
        ui->frameInfoLabel->setText("None");
    }

    vHandleUpdate = true;
}

NVL_INLINE void ModelAnimationWidget::updateSelection()
{    
    vHandleUpdate = false;

    ui->animationTreeWidget->clearSelection();
    ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

    if (selectedDrawer != nullptr) {
        if (selectedDrawer->loadedAnimation() != NULL_ID) {
            ui->animationTreeWidget->topLevelItem(selectedDrawer->loadedAnimation() + 1)->setSelected(true);
        }
        else {
            ui->animationTreeWidget->topLevelItem(0)->setSelected(true);
        }
    }

    ui->animationTreeWidget->update();
    ui->animationTreeWidget->repaint();

    vHandleUpdate = true;
}

NVL_INLINE void ModelAnimationWidget::addTreeItem(const Model3d::Animation& animation)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString(animation.name().c_str()));
    ui->animationTreeWidget->addTopLevelItem(item);
}

NVL_INLINE void ModelAnimationWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &ModelAnimationWidget::slot_drawableSelectionChanged);
        connect(vCanvas, &QCanvas::signal_animationStarted, this, &ModelAnimationWidget::slot_animationStarted);
        connect(vCanvas, &QCanvas::signal_animationStopped, this, &ModelAnimationWidget::slot_animationStopped);
        connect(vCanvas, &QCanvas::signal_animationPaused, this, &ModelAnimationWidget::slot_animationPaused);
    }
}

NVL_INLINE void ModelAnimationWidget::on_animationTreeWidget_itemSelectionChanged()
{
    if (vHandleUpdate) {
        QList<QTreeWidgetItem*> items = ui->animationTreeWidget->selectedItems();

        Index vSelectedAnimation = NULL_ID;

        if (items.size() >= 1) {
            QTreeWidgetItem* item = items[0];
            int id = ui->animationTreeWidget->indexOfTopLevelItem(item);

            if (id > 0) {
                vSelectedAnimation = static_cast<Index>(id - 1);
            }
        }

        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();
        if (vSelectedAnimation != NULL_ID) {
            selectedDrawer->loadAnimation(vSelectedAnimation);
        }
        else {
            selectedDrawer->unloadAnimation();
        }

        vCanvas->updateGL();
        updateOptions();

        emit signal_animationSelectionChanged(vSelectedAnimation);
    }
}

NVL_INLINE void ModelAnimationWidget::on_LBSButton_clicked()
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationSkinningMode(ModelDrawerBase::SKINNING_LINEAR_BLENDING);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}
NVL_INLINE void ModelAnimationWidget::on_DQSButton_clicked()
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationSkinningMode(ModelDrawerBase::SKINNING_DUAL_QUATERNIONS);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelAnimationWidget::on_speedSpinBox_valueChanged(double arg1)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationSpeed(arg1);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelAnimationWidget::on_loopCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationLoop(arg1 == Qt::Checked);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelAnimationWidget::on_keyframeSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setCurrentFrameId(value);

        updateOptions();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelAnimationWidget::on_fpsSpinBox_valueChanged(double arg1)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationTargetFPS(arg1);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}


NVL_INLINE void ModelAnimationWidget::on_blendCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationBlend(arg1 == Qt::Checked);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelAnimationWidget::on_keepKeyframesCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        ModelDrawer<Model3d>* selectedDrawer = getSelectedModelDrawer();

        selectedDrawer->setAnimationKeepKeyframes(arg1 == Qt::Checked);
        selectedDrawer->update();

        updateOptions();
        vCanvas->updateGL();
    }
}

}

#endif
#endif

