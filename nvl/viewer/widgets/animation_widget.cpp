/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_animation_widget.h"

namespace nvl {

NVL_INLINE AnimationWidget::AnimationWidget(QCanvas* canvas, QWidget* parent) :
    QFrame(parent),
    ui(new Ui::AnimationWidget),
    vCanvas(canvas),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    updateView();

    connectSignals();
}

NVL_INLINE AnimationWidget::~AnimationWidget()
{
    delete ui;
}

NVL_INLINE void AnimationWidget::slot_animationStarted()
{
    updateView();
}

NVL_INLINE void AnimationWidget::slot_animationStopped()
{
    updateView();
}

NVL_INLINE void AnimationWidget::slot_animationPaused()
{
    updateView();
}

NVL_INLINE void AnimationWidget::updateView()
{
    vHandleUpdate = false;

    bool enabled = vCanvas != nullptr;
    bool animationStarted = enabled && vCanvas->animationsRunning();
    bool animationPaused = enabled && vCanvas->animationsPaused();

    ui->playButton->setEnabled(enabled && (!animationStarted || animationPaused));
    ui->pauseButton->setEnabled(enabled && (animationStarted && !animationPaused));
    ui->stopButton->setEnabled(enabled && animationStarted);
    ui->fpsSpinBox->setEnabled(enabled && !animationStarted);

    vHandleUpdate = true;
}

NVL_INLINE void AnimationWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vCanvas, &QCanvas::signal_animationStarted, this, &AnimationWidget::slot_animationStarted);
        connect(vCanvas, &QCanvas::signal_animationStopped, this, &AnimationWidget::slot_animationStopped);
        connect(vCanvas, &QCanvas::signal_animationPaused, this, &AnimationWidget::slot_animationPaused);
    }
}

NVL_INLINE void AnimationWidget::on_playButton_clicked()
{
    vCanvas->startAnimations();
}

NVL_INLINE void AnimationWidget::on_pauseButton_clicked()
{
    vCanvas->pauseAnimations();
}

NVL_INLINE void AnimationWidget::on_stopButton_clicked()
{
    vCanvas->stopAnimations();
}

NVL_INLINE void AnimationWidget::on_fpsSpinBox_valueChanged(double arg1)
{
    if (vHandleUpdate) {
        vCanvas->setTargetFPS(arg1);
    }
}

}

#endif
#endif
