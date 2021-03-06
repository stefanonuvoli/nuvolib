/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_drawer_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_model_drawer_widget.h"

namespace nvl {

NVL_INLINE ModelDrawerWidget::ModelDrawerWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ModelDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget)
{
    ui->setupUi(this);

    vFaceMeshDrawerWidget = new FaceMeshDrawerWidget(vCanvas, vDrawableListWidget);
    vPolylineMeshDrawerWidget = new PolylineMeshDrawerWidget(vCanvas, vDrawableListWidget);
    vVertexMeshDrawerWidget = new VertexMeshDrawerWidget(vCanvas, vDrawableListWidget);
    vSkeletonDrawerWidget = new SkeletonDrawerWidget(vCanvas, vDrawableListWidget);
    this->addTab(vFaceMeshDrawerWidget, tr("Surface"));
    this->addTab(vPolylineMeshDrawerWidget, tr("Polylines"));
    this->addTab(vVertexMeshDrawerWidget, tr("Vertices"));
    this->addTab(vSkeletonDrawerWidget, tr("Skeleton"));
}

NVL_INLINE ModelDrawerWidget::~ModelDrawerWidget()
{
    delete ui;
}

}

#endif
#endif
