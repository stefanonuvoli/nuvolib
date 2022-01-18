#include "viewerwindow.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_viewerwindow.h"

#include <nvl/viewer/widgets/qglviewer_canvas.h>

#include <QVBoxLayout>

namespace nvl {

NVL_INLINE ViewerWindow::ViewerWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    vCanvas(nullptr)
{
    ui->setupUi(this);

    hideLeftDock();
    hideRightDock();
}

NVL_INLINE ViewerWindow::~ViewerWindow()
{
    delete ui;
}

NVL_INLINE void ViewerWindow::addCanvas(QCanvas* canvas)
{
    removeCanvas();
    vCanvas = canvas;

    ui->centralwidget->layout()->addWidget(vCanvas);

    initCanvas();
}

NVL_INLINE void ViewerWindow::removeCanvas()
{
    if (vCanvas != nullptr) {
        disconnect(vCanvas, &QCanvas::signal_fpsDisplayedChanged, this, &ViewerWindow::slot_fpsDisplayedChanged);
        disconnect(vCanvas, &QCanvas::signal_axisDisplayedChanged, this, &ViewerWindow::slot_axisDisplayedChanged);
        disconnect(vCanvas, &QCanvas::signal_backgroundColorChanged, this, &ViewerWindow::slot_backgroundColorChanged);

        ui->centralwidget->layout()->removeWidget(vCanvas);
        vCanvas = nullptr;
    }
}

NVL_INLINE QCanvas* ViewerWindow::canvas()
{
    return vCanvas;
}

NVL_INLINE void ViewerWindow::showLeftDock()
{
    ui->leftDockWidget->show();
    ui->actionLeftDock->setChecked(true);
}

NVL_INLINE void ViewerWindow::showRightDock()
{
    ui->rightDockWidget->show();
    ui->actionRightDock->setChecked(true);
}

NVL_INLINE void ViewerWindow::hideLeftDock()
{
    ui->leftDockWidget->hide();
    ui->actionLeftDock->setChecked(false);
}

NVL_INLINE void ViewerWindow::hideRightDock()
{
    ui->rightDockWidget->hide();
    ui->actionRightDock->setChecked(false);
}

NVL_INLINE void ViewerWindow::addToLeftDock(QWidget* widget)
{
    ui->leftDockVerticalLayout->addWidget(widget);
}

NVL_INLINE void ViewerWindow::addToRightDock(QWidget* widget)
{
    ui->rightDockVerticalLayout->addWidget(widget);
}

NVL_INLINE void ViewerWindow::initCanvas()
{
    assert(vCanvas != nullptr);

    vCanvas->setFPSDisplayed(ui->actionShowFPS->isChecked());
    vCanvas->setAxisDisplayed(ui->actionShowAxis->isChecked());

    connect(vCanvas, &QCanvas::signal_fpsDisplayedChanged, this, &ViewerWindow::slot_fpsDisplayedChanged);
    connect(vCanvas, &QCanvas::signal_axisDisplayedChanged, this, &ViewerWindow::slot_axisDisplayedChanged);
    connect(vCanvas, &QCanvas::signal_backgroundColorChanged, this, &ViewerWindow::slot_backgroundColorChanged);
}

NVL_INLINE void ViewerWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    ui->actionFullScreen->setChecked(isFullScreen());
}

NVL_INLINE void ViewerWindow::slot_fpsDisplayedChanged()
{
    ui->actionShowFPS->setChecked(vCanvas->isFPSDisplayed());
}

NVL_INLINE void ViewerWindow::slot_axisDisplayedChanged()
{
    ui->actionShowAxis->setChecked(vCanvas->isAxisDisplayed());
}

NVL_INLINE void ViewerWindow::slot_backgroundColorChanged()
{
    //TODO!
}

NVL_INLINE void ViewerWindow::on_actionQuit_triggered()
{
    close();
}

NVL_INLINE void ViewerWindow::on_actionShowFPS_triggered(bool checked)
{
    if (vCanvas != nullptr) {
        vCanvas->setFPSDisplayed(checked);
    }
}

NVL_INLINE void ViewerWindow::on_actionShowAxis_triggered(bool checked)
{
    if (vCanvas != nullptr) {
        vCanvas->setAxisDisplayed(checked);
    }
}

NVL_INLINE void ViewerWindow::on_actionFitScene_triggered()
{
    if (vCanvas != nullptr) {
        vCanvas->fitScene();
    }
}

NVL_INLINE void ViewerWindow::on_actionFullScreen_triggered(bool checked)
{
    if (checked) {
        showFullScreen();
    }
    else {
        showNormal();
        showMaximized();
    }
}

NVL_INLINE void ViewerWindow::on_actionLeftDock_triggered(bool checked)
{
    if (checked) {
        showLeftDock();
    }
    else {
        hideLeftDock();
    }
}

NVL_INLINE void ViewerWindow::on_actionRightDock_triggered(bool checked)
{
    if (checked) {
        showRightDock();
    }
    else {
        hideRightDock();
    }
}

}

#endif
#endif
