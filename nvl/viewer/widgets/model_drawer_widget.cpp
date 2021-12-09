#include "model_drawer_widget.h"
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
