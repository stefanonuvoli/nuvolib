/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "drawable_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_drawable_widget.h"

namespace nvl {

NVL_INLINE DrawableWidget::DrawableWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget* parent) :
    QFrame(parent),
    ui(new Ui::DrawableWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE DrawableWidget::~DrawableWidget()
{
    delete ui;
}

NVL_INLINE void DrawableWidget::updateView()
{
    vHandleUpdate = false;

    bool interfaceEnabled = false;
    bool visibleChecked = false;
    bool isFrameable = false;

    if (vCanvas != nullptr) {
        const std::unordered_set<Index>& selectedDrawables = vDrawableListWidget->selectedDrawables();

        interfaceEnabled = !selectedDrawables.empty();

        visibleChecked = interfaceEnabled;
        isFrameable = interfaceEnabled;

        for (const Index& id : selectedDrawables) {
            visibleChecked &= vCanvas->drawable(id)->isVisible();
            isFrameable &= vCanvas->isFrameable(id);
        }
    }

    ui->visibleCheckBox->setEnabled(interfaceEnabled);
    ui->visibleCheckBox->setChecked(visibleChecked);

    ui->resetFrameButton->setEnabled(isFrameable);

    vHandleUpdate = true;
}

NVL_INLINE void DrawableWidget::slot_drawableListChanged()
{
    updateView();
}

NVL_INLINE void DrawableWidget::slot_drawableSelectionChanged()
{
    updateView();
}

NVL_INLINE void DrawableWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals from the viewer
        connect(vCanvas, &QCanvas::signal_drawableListChanged, this, &DrawableWidget::slot_drawableListChanged);
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &DrawableWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void DrawableWidget::on_visibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (const Index& id : vDrawableListWidget->selectedDrawables()) {
            vCanvas->drawable(id)->setVisible(arg1);
        }

        vCanvas->updateGL();
    }
}

NVL_INLINE void DrawableWidget::on_resetFrameButton_clicked()
{
    if (vCanvas != nullptr) {
        for (const Index& id : vDrawableListWidget->selectedDrawables()) {
            assert(vCanvas->frameable(id) != nullptr);
            vCanvas->frameable(id)->setFrame(Affine3d::Identity());
        }

        vCanvas->updateGL();
    }
}

}

#endif
#endif
